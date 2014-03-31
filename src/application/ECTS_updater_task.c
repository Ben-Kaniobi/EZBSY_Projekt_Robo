/**
 * \file    ECTS_updater.h
 * \author  kasen1
 * \date    2014-03-12
 *
 * \version 1.0
 *  import from template by wht4 (2014-03-12)
 *
 * \brief   tasks to handle the conveyer and ECTS updater
 *
 * @{
 */

/* Includes -------------------------------------------------------------------*/
#include "ECTS_updater_task.h"
#include "carme_io1.h"                   /* CARMEIO1 Board Support Package     */

#include <stdio.h>                       /* Standard Input/Output              */
#include <stdlib.h>                      /* General Utilities                  */
#include <string.h>                      /* String handling                    */
#include <stdint.h>                      /* Integer typedefs                   */

#include <FreeRTOS.h>                    /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <memPoolService.h>

/* private macro --------------------------------------------------------------*/
/* Common */
#define EVER         ;;                  /* For forever loop: for(;;)          */
#define X_STEP       (1)                 /* TODO                               */
/* CAN IDs */
#define GET_STATUS_L (0x110)             /* Status request for left conveyer   */
#define GET_STATUS_C (0x120)             /* Status request for center conveyer */
#define GET_STATUS_R (0x130)             /* Status request for right conveyer  */
#define STATUS_L     (0x111)             /* Status response for left conveyer  */
#define STATUS_C     (0x121)             /* Status response for center conveyer*/
#define STATUS_R     (0x131)             /* Status response for right conveyer */
#define CMD_L        (0x112)             /* Command for left conveyer          */
#define CMD_C        (0x122)             /* Command for center conveyer        */
#define CMD_R        (0x132)             /* Command for right conveyer         */
#define CMD_STATUS_L (0x113)             /* Command status for left conveyer   */
#define CMD_STATUS_C (0x123)             /* Command status for center conveyer */
#define CMD_STATUS_R (0x133)             /* Command status for right conveyer  */
#define RESET_L      (0x11F)             /* Reset for left conveyer            */
#define RESET_C      (0x12F)             /* Reset for center conveyer          */
#define RESET_R      (0x13F)             /* Reset for right conveyer           */
/* CAN messages */
#define MSG_ERROR    (0x00)
#define MSG_STATUS   (0x01)
#define MSG_START    (0x01)
#define MSG_STOP     (0x02)
#define MSG_STOP_AT  (0x03)
#define MSG_DONE     (0x04)
/* CAN DB values */
#define DB_STATUS    (1)
#define DB_ECTS      (2)
#define DB_POS_X_h   (3) //TODO: High or low byte?
#define DB_POS_X_l   (4) //TODO: High or low byte?
#define DB_POS_Y_h   (5) //TODO: High or low byte?
#define DB_POS_Y_l   (6) //TODO: High or low byte?

/* data types -----------------------------------------------------------------*/
xTaskHandle xECTS_updater_task_handle = NULL;

/* function prototypes --------------------------------------------------------*/
static void  vECTS_updater_task(void *pvData);
void CAN_conveyor_status_response(uint8_t conveyor, uint8_t DB[]);

/* data -----------------------------------------------------------------------*/
ects ECTS_1 = {0, 0, conveyor_C};
ects ECTS_2 = {0, 0, conveyor_C};
ects ECTS_3 = {0, 0, conveyor_C};
conveyorState conveyor_L_state = STOPPED;
conveyorState conveyor_C_state = STOPPED;
conveyorState conveyor_R_state = STOPPED;

/* implementation -------------------------------------------------------------*/

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
void  init_ECTS_updater_tasks(void) {

	xTaskCreate(vECTS_updater_task, (signed char *) ECTS_UPDATER_TASK_NAME, ECTS_UPDATER_STACK_SIZE, NULL, ECTS_UPDATER_TASK_PRIORITY, &xECTS_updater_task_handle);
}

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

		ECTS_1.x += X_STEP;
		ECTS_2.x += X_STEP;
		ECTS_3.x += X_STEP;

		//TODO: CAN_Status_Abfrage()




		/* Delay until defined time passed */
		vTaskDelayUntil(&xLastFlashTime, 200 / portTICK_RATE_MS);
	}
}

/*******************************************************************************
 *  function :    CAN_conveyor_status_response
 ******************************************************************************/
/** \brief        Function which is called by CAN gatekeeper when a conveyor
 *                status response was received.
 *
 *  \type         global
 *
 *  \param[in]	  conveyor    Number to identify from which conveyor the message
 *                            was. Use CONVEYOR_L, CONVEYOR_C, or CONVEYOR_M
 *
 *  \return       void
 *
 ******************************************************************************/
void CAN_conveyor_status_response(uint8_t conveyor, uint8_t DB[])
{
	if(DB[1] == MSG_STATUS) {
		/* ERROR */
		return;
	}

	/* Update conveyor state */
	switch(conveyor) {
	case CONVEYOR_L:
		conveyor_L_state = DB[DB_STATUS];
		break;

	case CONVEYOR_C:
		conveyor_C_state = DB[DB_STATUS];
		break;

	case CONVEYOR_R:
		conveyor_R_state = DB[DB_STATUS];
		break;

	default:
		/* Something went terribly wrong... */
		//TODO: Handle?
		return;
		break;
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

		/* Finally set the position for the correct ECTS */
		ECTS_p->x = (DB[DB_POS_X_h]<<8 & 0xFF00) | DB[DB_POS_X_l];
		ECTS_p->y = (DB[DB_POS_Y_h]<<8 & 0xFF00) | DB[DB_POS_Y_l];
	}
	else {

		/* ECTS_p hasn't been set before --> no ECTS was on the conveyor but was now detected*/
		//TODO: Handle
	}
}
