/******************************************************************************/
/*! \file ECTS_updater.h
******************************************************************************
* \brief Task to update the position of the ECTS structs
*
* Procedures :  vECTS_updater_task(void*)
*               InitECTSUpdaterTask()
*               CAN_conveyorL_status_response(CARME_CAN_MESSAGE*)
*               CAN_conveyorR_status_response(CARME_CAN_MESSAGE*)
*               CAN_conveyorC_status_response(CARME_CAN_MESSAGE*)
*               CAN_conveyor_status_response(uint8_t, uint8_t)
*
* \author kasen1
*
* \version 0.0.1
*
* \history 12.03.2014 Import from template by wht4
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* Robo model                                                                 */
/* ****************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "ECTS_updater_task.h"
#include "carme_io1.h"                  /* CARMEIO1 Board Support Package     */
#include "CAN_gatekeeper_task.h"        /* CAN communication                  */

#include <stdio.h>                      /* Standard Input/Output              */
#include <stdlib.h>                     /* General Utilities                  */
#include <string.h>                     /* String handling                    */
#include <stdint.h>                     /* Integer typedefs                   */

#include <FreeRTOS.h>                   /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <memPoolService.h>

/* private macro --------------------------------------------------------------*/
/* Common */
#define EVER         ;;                 /* For forever loop: for(;;)          */
#define CONV_SPEED   (0.0124f)          /* Speed in cm/ms of the conveyor (evaluated) */
#define TIME_STEP    (200)              /* Time delay in ms for updater task  */
#define X_STEP       ((CONV_SPEED)*(TIME_STEP)) /* Resulting x step in cm         */
/* CAN IDs */
#define GET_STATUS_L (0x110)            /* Status request for left conveyer   */
#define GET_STATUS_C (0x120)            /* Status request for center conveyer */
#define GET_STATUS_R (0x130)            /* Status request for right conveyer  */
#define STATUS_L     (0x111)            /* Status response for left conveyer  */
#define STATUS_C     (0x121)            /* Status response for center conveyer*/
#define STATUS_R     (0x131)            /* Status response for right conveyer */
#define CMD_L        (0x112)            /* Command for left conveyer          */
#define CMD_C        (0x122)            /* Command for center conveyer        */
#define CMD_R        (0x132)            /* Command for right conveyer         */
#define CMD_STATUS_L (0x113)            /* Command status for left conveyer   */
#define CMD_STATUS_C (0x123)            /* Command status for center conveyer */
#define CMD_STATUS_R (0x133)            /* Command status for right conveyer  */
#define RESET_L      (0x11F)            /* Reset for left conveyer            */
#define RESET_C      (0x12F)            /* Reset for center conveyer          */
#define RESET_R      (0x13F)            /* Reset for right conveyer           */
/* CAN messages */
#define MSG_ERROR    (0x00)
#define MSG_STATUS   (0x01)
#define MSG_START    (0x01)
#define MSG_STOP     (0x02)
#define MSG_STOP_AT  (0x03)
#define MSG_DONE     (0x04)
/* CAN DB values */
#define DB_STATUS    (1)
#define DB_ECTS_INFO (2)
#define DB_POS_X_h   (3)
#define DB_POS_X_l   (4)
#define DB_POS_Y_h   (5)
#define DB_POS_Y_l   (6)
/* Position */
#define X_STEP_MAX   (37)                /* Max. position in cm for x direction */
#define X_SENS_LR    (9)                 /* Position in cm for the left/right sensor */
#define X_SENS_C     (23)                /* Position in cm for the center sensor */

/* macros --------------------------------------------------------------------*/
#define X_DATA_TO_CM(x) (0.0048f * (x))    /* Convert X data from CAN to cm (0x186A -> 30cm) */

/* data types ----------------------------------------------------------------*/

/* function prototypes -------------------------------------------------------*/
static void  vECTS_updater_task(void *pvData);
void CAN_conveyorL_status_response(CARME_CAN_MESSAGE *rx_message);
void CAN_conveyorR_status_response(CARME_CAN_MESSAGE *rx_message);
void CAN_conveyorC_status_response(CARME_CAN_MESSAGE *rx_message);
void CAN_conveyor_status_handler(z_pos conveyor, uint8_t data[]);
void find_ECTS(ects **ECTS_p, z_pos conveyor);

/* data ----------------------------------------------------------------------*/
ects ECTS_1 = {0, 0, 1, conveyor_R};
ects ECTS_2 = {1, 0, 2, conveyor_C};
ects ECTS_3 = {2, 0, 3, conveyor_L};
conveyorState conveyor_L_state = STOPPED;
conveyorState conveyor_C_state = STOPPED;
conveyorState conveyor_R_state = STOPPED;
uint8_t CAN_buffer[8];
xSemaphoreHandle xMutexEditECTS = NULL;

/* implementation ------------------------------------------------------------*/

/*******************************************************************************
 *  function :    InitECTSUpdaterTask
 ******************************************************************************/
/** \brief        Initialisation for task
 *
 *  \type         global
 *
 *  \return
 *
 ******************************************************************************/
void  InitECTSUpdaterTask(void) {

	/* Set a CAN listener functions for conveyor status response */
	setFunctionCANListener((CAN_function_listener_t)CAN_conveyorL_status_response, STATUS_L);
	setFunctionCANListener((CAN_function_listener_t)CAN_conveyorR_status_response, STATUS_R);
	setFunctionCANListener((CAN_function_listener_t)CAN_conveyorC_status_response, STATUS_C);

	/* Create the mutex for ECTS access */
	xMutexEditECTS = xSemaphoreCreateMutex();

	xTaskCreate(vECTS_updater_task, (signed char *) ECTS_UPDATER_TASK_NAME, ECTS_UPDATER_STACK_SIZE, NULL, ECTS_UPDATER_TASK_PRIORITY, NULL);
}
/* ****************************************************************************/
/* End      :  InitECTSUpdaterTask                                            */
/* ****************************************************************************/

/*******************************************************************************
 *  function :    vECTS_updater_task
 ******************************************************************************/
/** \brief        Updates values of the ECTS'
 *
 *  \type         global
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void vECTS_updater_task(void *pvData) {

	/* We need to initialise xLastFlashTime prior to the first call to vTaskDelayUntil() */
	portTickType xLastFlashTime;
	xLastFlashTime = xTaskGetTickCount();

	for(EVER) {
		/* Get mutex for ECTS access */
		if(xSemaphoreTake(xMutexEditECTS, (TIME_STEP/4) / portTICK_RATE_MS) == pdTRUE) {

			/* Updater x position */
			if(ECTS_1.x < X_STEP_MAX) {

				ECTS_1.x += X_STEP;
			}
			if(ECTS_2.x < X_STEP_MAX) {

				ECTS_2.x += X_STEP;
			}
			if(ECTS_3.x < X_STEP_MAX) {

				ECTS_3.x += X_STEP;
			}

			/* Release mutex */
			xSemaphoreGive(xMutexEditECTS);
		}

		/* Conveyor status request */
		CAN_buffer[0] = DB_STATUS;
		createCANMessage(GET_STATUS_L, 1, CAN_buffer);
		createCANMessage(GET_STATUS_R, 1, CAN_buffer);

		/* Delay until defined time passed */
		vTaskDelayUntil(&xLastFlashTime, TIME_STEP / portTICK_RATE_MS);
	}
}
/* ****************************************************************************/
/* End      :  init_ECTS_updater_tasks                                        */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  CAN_conveyorL_status_response                                   */
/******************************************************************************/
/*! \brief Function which is called by CAN gatekeeper when a status
*          response for the left conveyor was received.
*
* \param[in] *rx_message The received CAN message
*
* \return None
*
* \author kasen1
*
* \version 0.0.1
*
* \date 12.03.2014 Created
*
*******************************************************************************/
void CAN_conveyorL_status_response(CARME_CAN_MESSAGE *rx_message) {

	if(rx_message->data[0] == MSG_STATUS) {

		/* Redirect to combined function */
		CAN_conveyor_status_handler(conveyor_L, rx_message->data);
	}
	/* Else: Error message received */
}
/* ****************************************************************************/
/* End      :  CAN_conveyorL_status_response                                  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  CAN_conveyorR_status_response                                   */
/******************************************************************************/
/*! \brief Function which is called by CAN gatekeeper when a status
*          response for the left conveyor was received.
*
* \param[in] *rx_message The received CAN message
*
* \return None
*
* \author kasen1
*
* \version 0.0.1
*
* \date 12.03.2014 Created
*
*******************************************************************************/
void CAN_conveyorR_status_response(CARME_CAN_MESSAGE *rx_message) {

	if(rx_message->data[0] == MSG_STATUS) {

		/* Redirect to combined function */
		CAN_conveyor_status_handler(conveyor_R, rx_message->data);
	}
	/* Else: Error message received */
}
/* ****************************************************************************/
/* End      :  CAN_conveyorR_status_response                                  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  CAN_conveyorC_status_response                                   */
/******************************************************************************/
/*! \brief Function which is called by CAN gatekeeper when a status
*          response for the left conveyor was received.
*
* \param[in] *rx_message The received CAN message
*
* \return None
*
* \author kasen1
*
* \version 0.0.1
*
* \date 12.03.2014 Created
*
*******************************************************************************/
void CAN_conveyorC_status_response(CARME_CAN_MESSAGE *rx_message) {

	if(rx_message->data[0] == MSG_STATUS) {

		/* Redirect to combined function */
		CAN_conveyor_status_handler(conveyor_C, rx_message->data);
	}
	/* Else: Error message received */
}
/* ****************************************************************************/
/* End      :  CAN_conveyorC_status_response                                  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  CAN_conveyor_status_handler                                     */
/******************************************************************************/
/*! \brief Function to handle conveyor status responses
*
* \param[in] conveyor The conveyor for which the message was received
*
* \param[in] data[] The status data (8 bit max.)
*
* \return None
*
* \author kasen1
*
* \version 0.0.1
*
* \date 12.03.2014 Created
*
*******************************************************************************/
void CAN_conveyor_status_handler(z_pos conveyor, uint8_t data[]) {

	/* Update conveyor state */
	switch(conveyor) {

	case conveyor_L:
		conveyor_L_state = data[DB_STATUS];
		break;

	case conveyor_C:
		conveyor_C_state = data[DB_STATUS];
		break;

	case conveyor_R:
		conveyor_R_state = data[DB_STATUS];
		break;

	default:
		/* Invalid input, don't continue */
		return;
	}

	/* Only continue if there is at least information about the x position available */
	if(data[DB_ECTS_INFO] < 2) {
		return;
	}

	ects *ECTS_p = NULL;
	find_ECTS(&ECTS_p, conveyor);
	if(ECTS_p == NULL) {

		/* ECTS_p hasn't been set before --> no ECTS was on the conveyor but was now detected */
		//TODO: Handle?
	}
	else {

		/* Concat the two bytes */
		uint16_t pos_x = (data[DB_POS_X_h]<<8 & 0xFF00) | (data[DB_POS_X_l] & 0xFF);
		/* Convert to our format */
		pos_x = X_DATA_TO_CM(pos_x);

		/* Add sensor position offset */
		switch(conveyor) {
		case conveyor_L: /* Fall through */
		case conveyor_R:
			pos_x += X_SENS_LR;

		case conveyor_C:
			pos_x += X_SENS_C;

		default:
			/* Not possible, caught above */
			return;
		}

		/* Get mutex for ECTS access */
		if(xSemaphoreTake(xMutexEditECTS, portMAX_DELAY) == pdTRUE) {
			/* Finally set the position for the correct ECTS */
			ECTS_p->x = pos_x;
			/* Release mutex */
			xSemaphoreGive(xMutexEditECTS);
		}

		/* If theres data for the y position available, update this too */
		if(data[DB_ECTS_INFO] == 3) {

			/* Concat the two bytes */
			uint16_t pos_y = ((uint16_t)data[DB_POS_Y_h]<<8 & 0xFF00) | (data[DB_POS_Y_l] & 0xFF);
			/* Convert to our format (conveyor width divided in 8 sections/lines) */
			if(pos_y <= 868) /* between 800 and 868 */ {
				pos_y = 0;
			}
			else if(pos_y <= 1004) {
				pos_y = 1;
			}
			else if(pos_y <= 1139) {
				pos_y = 2;
			}
			else if(pos_y <= 1275) {
				pos_y = 3;
			}
			else if(pos_y <= 1411) {
				pos_y = 4;
			}
			else if(pos_y <= 1546) {
				pos_y = 5;
			}
			else if(pos_y <= 1682) {
				pos_y = 6;
			}
			else /* -> pos_y <= 1750 */ {
				pos_y = 7;
			}
			/* Reverse order if it's left conveyor, because the light barrier is the other way around */
			if(conveyor == conveyor_L) {
				pos_y = 7-pos_y;
			}
			/* Finally set the position for the correct ECTS */
			/* Get mutex for ECTS access */
			if(xSemaphoreTake(xMutexEditECTS, portMAX_DELAY) == pdTRUE) {
				/* Finally set the position for the correct ECTS */
				ECTS_p->y = pos_y;
				/* Release mutex */
				xSemaphoreGive(xMutexEditECTS);
			}
		}
	}
}
/* ****************************************************************************/
/* End      :  CAN_conveyor_status_response                                   */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  find_ECTS                                                       */
/******************************************************************************/
/*! \brief Finds the correct ECTS
*
* \param[out] *ECTS_p Pointer to the found ECTS, NULL if not found
* \param[in] _z_pos The conveyor to search
*
* \return None
*
* \author kasen1
*
* \version 0.0.1
*
* \date 12.03.2014 Created
*
*******************************************************************************/
void find_ECTS(ects **ECTS_p, z_pos _z_pos) {

	ects *ectsTemp[3] = {0, 0, 0};

	int k = 0;

	switch(_z_pos) {
	case robo_L: /* Fall through */
	case robo_R:
		/* Find the ECTS in this robo */
		if(ECTS_1.z == _z_pos) {
			/* Use ECTS_1 */
			*ECTS_p = &ECTS_1;
		}
		else if(ECTS_2.z == _z_pos) {
			/* Use ECTS_2 */
			*ECTS_p = &ECTS_2;
		}
		else if(ECTS_3.z == _z_pos) {
			/* Use ECTS_3 */
			*ECTS_p = &ECTS_3;
		}
		break;

	case conveyor_L: /* Fall through */
	case conveyor_R: /* Fall through */
	case conveyor_C:

		/* Find last ECTS */
		if(ECTS_1.z == _z_pos) {
			ectsTemp[k++] = &ECTS_1;
		}

		if(ECTS_2.z == _z_pos) {
			ectsTemp[k++] = &ECTS_2;
		}

		if(ECTS_3.z == _z_pos) {
			ectsTemp[k++] = &ECTS_3;
		}

		if(k == 3) {
		 if(ectsTemp[2]->x > ectsTemp[1]->x) {
			 ectsTemp[1] = ectsTemp[2];
		 }
		}


		if(k >= 2) {
			if(ectsTemp[1]->x > ectsTemp[0]->x) {
				ectsTemp[0] = ectsTemp[1];
			}
		}

		*ECTS_p = ectsTemp[0];

//OLD:
//		/* Find the last ECTS on this conveyor */
//		*ECTS_p = NULL;
//
//		if(ECTS_1.z == _z_pos) {
//
//			/* Use ECTS_1 */
//			*ECTS_p = &ECTS_1;
//		}
//		if(ECTS_2.z == _z_pos) {
//
//			if(*ECTS_p == NULL) {
//
//				/* ECTS_p hasn't been set before, set it now */
//				*ECTS_p = &ECTS_2;
//			}
//			else {
//
//				/* ECTS_p has been set before, compare x values */
//				if(ECTS_2.x > (*ECTS_p)->x ) {
//
//					//TODO: Handle ECTS_2 already behind light barrier
//
//					/* ECTS_2 is further, use this */
//					*ECTS_p = &ECTS_2;
//				}
//			}
//		}
//		if(ECTS_3.z == _z_pos) {
//
//			if(*ECTS_p == NULL) {
//
//				/* ECTS_p hasn't been set before, set it now */
//				*ECTS_p = &ECTS_3;
//			}
//			else {
//
//				/* ECTS_p has been set before, compare x values */
//				if(ECTS_3.x > (*(*ECTS_p)).x ) {
//
//					/* ECTS_3 is further, use this */
//					*ECTS_p = &ECTS_3;
//				}
//			}
//		}

		break;
	}
}
/* ****************************************************************************/
/* End      :  find_ECTS                                                      */
/* ****************************************************************************/
