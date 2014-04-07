/******************************************************************************/
/*! \file ECTS_updater.h
******************************************************************************
* \brief Task to update the position of the ECTS structs
*
* Procedures : 	vECTS_updater_task(void*)
* 				init_ECTS_updater_tasks()
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
#define X_STEP       (1)                /* TODO                               */
#define TIME_STEP    (200)              /* Time delay in ms for updater task  */
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
#define X_STEP_MAX   (47)                /* Max. position in cm for x direction */ //TODO

/* data types ----------------------------------------------------------------*/

/* function prototypes -------------------------------------------------------*/
static void  vECTS_updater_task(void *pvData);
void CAN_conveyorL_status_response(CARME_CAN_MESSAGE *rx_message);
void CAN_conveyorR_status_response(CARME_CAN_MESSAGE *rx_message);
void CAN_conveyorC_status_response(CARME_CAN_MESSAGE *rx_message);
void CAN_conveyor_status_response(uint8_t conveyor, uint8_t data[]);

/* data ----------------------------------------------------------------------*/
ects ECTS_1 = {0, 0, 0, conveyor_L};
ects ECTS_2 = {1, 0, 0, conveyor_C};
ects ECTS_3 = {2, 0, 0, conveyor_R};
conveyorState conveyor_L_state = STOPPED;
conveyorState conveyor_C_state = STOPPED;
conveyorState conveyor_R_state = STOPPED;
uint8_t CAN_buffer[8];

/* implementation ------------------------------------------------------------*/

/*******************************************************************************
 *  function :    init_ECTS_updater_tasks
 ******************************************************************************/
/** \brief        Initialisation for task
 *
 *  \type         global
 *
 *  \return
 *
 ******************************************************************************/
void  init_ECTS_updater_task(void) {

	/* Set a CAN listener functions for conveyor status response */
	setFunctionCANListener((CAN_function_listener_t)CAN_conveyorL_status_response, STATUS_L);
	setFunctionCANListener((CAN_function_listener_t)CAN_conveyorR_status_response, STATUS_R);
	setFunctionCANListener((CAN_function_listener_t)CAN_conveyorC_status_response, STATUS_C);

	xTaskCreate(vECTS_updater_task, (signed char *) ECTS_UPDATER_TASK_NAME, ECTS_UPDATER_STACK_SIZE, NULL, ECTS_UPDATER_TASK_PRIORITY, NULL);
}
/* ****************************************************************************/
/* End      :  init_ECTS_updater_tasks										  */
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

		if(ECTS_1.x >= X_STEP_MAX) {
			
			ECTS_1.x = 0;
		}
		if(ECTS_2.x >= X_STEP_MAX) {
			
			ECTS_2.x = 0;
		}
		if(ECTS_3.x >= X_STEP_MAX) {
			
			ECTS_3.x = 0;
		}
		ECTS_1.x += X_STEP;
		ECTS_2.x += X_STEP;
		ECTS_3.x += X_STEP;

		/* Conveyor status request */
		CAN_buffer[0] = DB_STATUS;
		createCANMessage(GET_STATUS_L, 1, CAN_buffer);
		createCANMessage(GET_STATUS_R, 1, CAN_buffer);

		/* Delay until defined time passed */
		vTaskDelayUntil(&xLastFlashTime, TIME_STEP / portTICK_RATE_MS);
	}
}
/* ****************************************************************************/
/* End      :  init_ECTS_updater_tasks										  */
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
		CAN_conveyor_status_response(conveyor_L, rx_message->data);
	}
	/* Else: Error message received */
}
/* ****************************************************************************/
/* End      :  CAN_conveyorL_status_response								  */
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
		CAN_conveyor_status_response(conveyor_R, rx_message->data);
	}
	/* Else: Error message received */
}
/* ****************************************************************************/
/* End      :  CAN_conveyorR_status_response								  */
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
		CAN_conveyor_status_response(conveyor_C, rx_message->data);
	}
	/* Else: Error message received */
}
/* ****************************************************************************/
/* End      :  CAN_conveyorC_status_response								  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  CAN_conveyor_status_response                                   */
/******************************************************************************/
/*! \brief Function to handle conveyor status responses
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
void CAN_conveyor_status_response(uint8_t conveyor, uint8_t data[]) {

	/* Update conveyor state */
	conveyor_L_state = data[DB_STATUS];

	/* Only continue if there is at least information about the x position available */
	if(data[DB_ECTS_INFO] < 2) {
		return;
	}

	ects *ECTS_p = NULL;

	/* Find correct ECTS_p */
	if(ECTS_1.z == conveyor) {

		/* Use ECTS_1 */
		ECTS_p = &ECTS_1;
	}
	if(ECTS_2.z == conveyor) {

		if(!ECTS_p) {

			/* ECTS_p hasn't been set before, set it now */
			ECTS_p = &ECTS_1;
		}
		else {

			/* ECTS_p has been set before, compare x values */
			if(ECTS_2.x > ECTS_p->x) {

				//TODO: Handle 2>p aber 2 schon hinter Schranke

				/* ECTS_2 is further, use this */
				ECTS_p = &ECTS_2;
			}
		}
	}
	if(ECTS_3.z == conveyor) {

		if(!ECTS_p) {

			/* ECTS_p hasn't been set before, set it now */
			ECTS_p = &ECTS_1;
		}
		else {

			/* ECTS_p has been set before, compare x values */
			if(ECTS_3.x > ECTS_p->x) {

				/* ECTS_2 is further, use this */
				ECTS_p = &ECTS_3;
			}
		}
	}
	if(ECTS_p) {

		/* Concat the two bytes */
		uint16_t raw_pos_x = (data[DB_POS_X_h]<<8 & 0xFF00) | (data[DB_POS_X_l] & 0xFF);
		/* Convert to our format */
		//raw_pos_x = TODO raw_pos_x;
		/* Finally set the position for the correct ECTS */
		ECTS_p->x = raw_pos_x;

		/* If theres data for the y position available, update this too */
		if(data[DB_ECTS_INFO] == 3) {

			/* Concat the two bytes */
			uint16_t raw_pos_y = (data[DB_POS_Y_h]<<8 & 0xFF00) | (data[DB_POS_Y_l] & 0xFF);
			/* Convert to our format */
			//raw_pos_y = TODO raw_pos_y;
			/* Finally set the position for the correct ECTS */
			ECTS_p->y = raw_pos_y;
		}
	}
	else {

		/* ECTS_p hasn't been set before --> no ECTS was on the conveyor but was now detected*/
		//TODO: Handle
	}
}
/* ****************************************************************************/
/* End      :  CAN_conveyor_status_response									  */
/* ****************************************************************************/
