/******************************************************************************/
/*! \file conveyor_tasks.c
******************************************************************************
* \brief Tasks to handle the left and right conveyors
*
* Procedures : 	vConveyorL_task(void*)
*               vConveyorR_task(void*)
* 				InitConveyorTasks()
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

/* --------------------------------- imports ---------------------------------*/
#include <memPoolService.h>                 /* Memory pool manager service */
#include "stm32f4xx.h"                      /* uC includes */

/* application */
#include "conveyor_tasks.h"                 /* Own header include */
#include "CAN_gatekeeper_task.h"            /* CAN communication */
#include "ECTS_updater_task.h"

/* ----------------------- private constant declaration ----------------------*/
/* Common */
#define EVER         ;;                 /* For forever loop: for(;;)          */
#define CONVEYOR_STOP_X (16)            /* Stop x position in cm              */
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
#define DB_ECTS      (2)
#define DB_POS_X_h   (3)
#define DB_POS_X_l   (4)
#define DB_POS_Y_h   (5)
#define DB_POS_Y_l   (6)

/* macros --------------------------------------------------------------------*/
#define CM_TO_X_DATA(x) (1.0f / 0.0048f * (x))    /* Convert X data from CAN to cm (30cm -> 0x186A) */

/* ------------------------- module data declaration -------------------------*/
xSemaphoreHandle xSemaphoreLeftECTS = NULL;
xSemaphoreHandle xSemaphoreRightECTS = NULL;
uint8_t CAN_buffer[8];

/* ----------------------- module procedure declaration ----------------------*/
static void vConveyorL_task(void*);
static void vConveyorR_task(void*);



/*******************************************************************************
 *  function :    InitConveyorTasks
 ******************************************************************************/
/** \brief        Initialisation for task
 *
 *  \type         global
 *
 *  \return
 *
 ******************************************************************************/
void InitConveyorTasks(void)
{
	/* Create and take the ECTS semaphores */
	xSemaphoreLeftECTS = xSemaphoreCreateBinary();
	xSemaphoreRightECTS = xSemaphoreCreateBinary();

	/* Create the tasks */
	xTaskCreate(vConveyorL_task, ( signed char * ) CONVEYOR_L_TASK_NAME, CONVEYOR_TASK_STACK_SIZE, NULL, CONVEYOR_TASK_PRIORITY, NULL);
	xTaskCreate(vConveyorR_task, ( signed char * ) CONVEYOR_R_TASK_NAME, CONVEYOR_TASK_STACK_SIZE, NULL, CONVEYOR_TASK_PRIORITY, NULL);
}
/* ****************************************************************************/
/* End      :  InitConveyorTasks											  */
/* ****************************************************************************/

/*******************************************************************************
 *  function :    vConveyorL_task
 ******************************************************************************/
/** \brief        Handles the left conveyor
 *
 *  \type         global
 *
 *  \param[in]	  pvParameters    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void vConveyorL_task(void* pvParameters )
{
	portTickType xLastFlashTime;

	/* We need to initialise xLastFlashTime prior to the first call to
	vTaskDelayUntil(). */
	xLastFlashTime = xTaskGetTickCount();

	/* Take the ECTS semaphore */
	xSemaphoreTake(xSemaphoreLeftECTS, portMAX_DELAY);

	for(;;)
	{
		/* Wait for the conveyor semaphore */
		if(xSemaphoreTake(xSemaphoreLeftECTS, portMAX_DELAY) == pdTRUE) {

			/* Send "stop the conveyor at position" message */
			CAN_buffer[0] = MSG_STOP;
			CAN_buffer[1] = ((uint8_t)CM_TO_X_DATA(CONVEYOR_STOP_X) >> 8) & 0xFF;  /* High byte */
			CAN_buffer[2] = ((uint8_t)CM_TO_X_DATA(CONVEYOR_STOP_X)) & 0xFF;  /* Low byte */
			createCANMessage(CMD_L, 3, CAN_buffer);

			/* Start the conveyor */
			CAN_buffer[0] = MSG_START;
			CAN_buffer[1] = 0;
			CAN_buffer[2] = 0;
			createCANMessage(CMD_L, 3, CAN_buffer);

			/* Wait while conveyor still moving */
			while(conveyor_L_state != STOPPED) {

				vTaskDelay(50 / portTICK_RATE_MS);
			}

			/* Send finish command */
			CAN_buffer[0] = MSG_DONE;
			CAN_buffer[1] = 0;
			createCANMessage(CMD_L, 2, CAN_buffer);

			/* Conveyor stopped, give ECTS semaphore */
			xSemaphoreGive(xSemaphoreLeftECTS);

			/* Update the ECTS */
			update_ECTS_z(robo_L);
		}

		vTaskDelayUntil(&xLastFlashTime, 200 / portTICK_RATE_MS);
	}
}
/* ****************************************************************************/
/* End      :  vConveyorL_task												  */
/* ****************************************************************************/

/*******************************************************************************
 *  function :    vConveyorR_task
 ******************************************************************************/
/** \brief        Handles the right conveyor
 *
 *  \type         global
 *
 *  \param[in]	  pvParameters    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void vConveyorR_task(void* pvParameters )
{
	portTickType xLastFlashTime;

	/* We need to initialise xLastFlashTime prior to the first call to
	vTaskDelayUntil(). */
	xLastFlashTime = xTaskGetTickCount();

	/* Take the ECTS semaphore */
	xSemaphoreTake(xSemaphoreRightECTS, portMAX_DELAY);

	for(;;)
	{
		/* Wait for the conveyor semaphore */
		if(xSemaphoreTake(xSemaphoreRightECTS, portMAX_DELAY) == pdTRUE) {

			/* Send "stop the conveyor at position" message */
			CAN_buffer[0] = MSG_STOP;
			CAN_buffer[1] = ((uint8_t)CM_TO_X_DATA(CONVEYOR_STOP_X) >> 8) & 0xFF;  /* High byte */
			CAN_buffer[2] = ((uint8_t)CM_TO_X_DATA(CONVEYOR_STOP_X)) & 0xFF;  /* Low byte */
			createCANMessage(CMD_R, 3, CAN_buffer);

			/* Start the conveyor */
			CAN_buffer[0] = MSG_START;
			CAN_buffer[1] = 0;
			CAN_buffer[2] = 0;
			createCANMessage(CMD_R, 3, CAN_buffer);

			/* Wait while conveyor still moving */
			while(conveyor_R_state != STOPPED) {

				vTaskDelay(50 / portTICK_RATE_MS);
			}

			/* Send finish command */
			CAN_buffer[0] = MSG_DONE;
			CAN_buffer[1] = 0;
			createCANMessage(CMD_R, 2, CAN_buffer);

			/* Conveyor stopped, give ECTS semaphore */
			xSemaphoreGive(xSemaphoreRightECTS);

			/* Update the ECTS */
			update_ECTS_z(robo_R);
		}

		vTaskDelayUntil(&xLastFlashTime, 200 / portTICK_RATE_MS);
	}
}
/* ****************************************************************************/
/* End      :  vConveyorL_task												  */
/* ****************************************************************************/

