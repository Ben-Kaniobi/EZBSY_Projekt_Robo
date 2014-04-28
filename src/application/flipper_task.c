/******************************************************************************/
/*! \file conveyor_tasks.c
******************************************************************************
* \brief Task to handle the flipper and conveyor
*
* Procedures : 	InitFlipperTask()
*				vFlipper_task()
*
* \author kasen1
*
* \version 0.1.0
*
* \history 28.04.2014 Version 0.1.0
* \history 12.03.2014 Import from template by wht4
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* Robo model                                                                 */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/
#include <memPoolService.h>                 /* Memory pool manager service */
#include "stm32f4xx.h"                      /* uC includes */

/* application */
#include "flipper_task.h"                   /* Own header include */
#include "CAN_gatekeeper_task.h"            /* CAN communication */
#include "ECTS_updater_task.h"

/* ----------------------- private constant declaration ----------------------*/
/* Common */
#define EVER         ;;                 /* For forever loop: for(;;)          */
/* Flipper */
#define FLIPPER_X_MIN (37)              /* Min. x pos. in cm for ECTS to be in flipper area */

/* ------------------------- module data declaration -------------------------*/
uint8_t CAN_buffer[8];

/* ----------------------- module procedure declaration ----------------------*/
static void vFlipper_task(void*);


/*******************************************************************************
*  function :    InitFlipperTask
*******************************************************************************/
/*! \brief Initialisation for the task
*
* \param[in] None
*
* \return None
*
* \author kasen1
*
* \version 0.1.0
*
* \date 28.04.2014 Version 0.1.0
*
*******************************************************************************/
void InitFlipperTask(void)
{
	/* Create the task */
	xTaskCreate(vFlipper_task, ( signed char * ) FLIPPER_TASK_NAME, FLIPPER_TASK_STACK_SIZE, NULL, FLIPPER_TASK_PRIORITY, NULL);
}
/* ****************************************************************************/
/* End      :  InitFlipperTask                                                */
/* ****************************************************************************/


/*******************************************************************************
*  function :    vFlipper_task
*******************************************************************************/
/*! \brief Handles the flipper and center conveyor
*
* \param[in] pvParameters    not used
*
* \return None
*
* \author kasen1
*
* \version 0.1.0
*
* \date 28.04.2014 Version 0.1.0
*
*******************************************************************************/
static void vFlipper_task(void* pvParameters )
{
	portTickType xLastFlashTime;

	/* We need to initialise xLastFlashTime prior to the first call to
	vTaskDelayUntil(). */
	xLastFlashTime = xTaskGetTickCount();

//	/* Send reset command */
//	createCANMessage(RESET_C, 0, CAN_buffer);

	/* Send finish command to fix problems */
	CAN_buffer[0] = MSG_DONE;
	CAN_buffer[1] = 0;
	CAN_buffer[2] = 0;
	createCANMessage(CMD_C, 3, CAN_buffer);
	vTaskDelay(50 / portTICK_RATE_MS);

	/* Start the center conveyor */
	CAN_buffer[0] = MSG_START;
	CAN_buffer[1] = 0;
	CAN_buffer[2] = 0;
	createCANMessage(CMD_C, 3, CAN_buffer);
	vTaskDelay(50 / portTICK_RATE_MS);

	/* Create shadow variable for flipper */
	enum {FLIPPER_LEFT=DB_POS_L, FLIPPER_RIGHT=DB_POS_R} flipper_shadow = FLIPPER_RIGHT;
	/* Set the flipper start position */
	CAN_buffer[0] = MSG_FLIP;
	CAN_buffer[1] = DB_POS_L;
	CAN_buffer[2] = DB_SPEED;
	createCANMessage(CMD_FLIPPER, 3, CAN_buffer);
	vTaskDelay(50 / portTICK_RATE_MS);

	ects *ECTS_p = NULL;

	for(;;)
	{
		find_ECTS(&ECTS_p, conveyor_C);
		if(ECTS_p != NULL) {

			if(ECTS_p->x > FLIPPER_X_MIN) {

				/* ECTS in reach, toggle flipper */
				if(flipper_shadow == FLIPPER_LEFT) {
					flipper_shadow = FLIPPER_RIGHT;

					/* Update position */
					ECTS_p->z = conveyor_R;
					ECTS_p->x = 0;
				}
				else {
					flipper_shadow = FLIPPER_LEFT;

					/* Update z position */
					ECTS_p->z = conveyor_L;
					ECTS_p->x = 0;
				}

				/* Set the flipper position */
				CAN_buffer[0] = MSG_FLIP;
				CAN_buffer[1] = flipper_shadow;
				CAN_buffer[2] = DB_SPEED;
				createCANMessage(CMD_FLIPPER, 3, CAN_buffer);
				vTaskDelay(50 / portTICK_RATE_MS);
			}
		}

		vTaskDelayUntil(&xLastFlashTime, 50 / portTICK_RATE_MS);
	}
}
/* ****************************************************************************/
/* End      :  vFlipper_task                                                  */
/* ****************************************************************************/


/* ****************************************************************************/
/* End Header : Flipper_task.c                                                */
/* ****************************************************************************/
