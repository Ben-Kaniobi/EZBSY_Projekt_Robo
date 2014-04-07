/******************************************************************************/
/*! \file robot_left_task.c
******************************************************************************
* \brief Task that controls the left robot arm
*
* Function : More detailed description of the files function
*
* Procedures : 	vRobotLeftTask(void*)
* 				InitRobotLeftTask()
*              	function3()...
*
* \author plats1
*
* \version 0.0.1
*
* \history 07.04.2014 File Created
*
*
* \ingroup <group name> [<group name 2> <group name 3>]
*
* \todo If u have some todo's for the c-file, add it here
*
* \bug Description of the bug
*
*/
/* ****************************************************************************/
/* Project name																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/

#include <memPoolService.h>			/* Memory pool manager service */
#include "stm32f4xx.h"				/* uC includes */

#include <FreeRTOS.h> 				/* All freeRTOS headers */
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <memPoolService.h>

/* application */
#include "init_robot_tasks.h"		/* Own header include */
#include "robot_left_task.h"
#include "CAN_gatekeeper_task.h"
#include "conveyor_tasks.h"


/* ------------------------- module data declaration -------------------------*/

uint8_t payloadLeft[][8] = {
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* PrepareToTakeEcts */
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* LiftEctsUp */
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* RotateToUnloadTray */
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* PrepareToUnloadEcts */
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* UnloadEctsToTray */
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},	/* RemoveArmFromUnloadTray */
		{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}	/* RotateToConveyor */
};


/* ----------------------- module procedure declaration ----------------------*/


/* ****************************************************************************/
/* End Header : robot_left_task.c											  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  vRobotLeftTask												  */
/******************************************************************************/
/*! \brief Short description of the function2
*
* Function : More detailed description of the function2
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author plats1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
*
* \todo If u have some todo's for the function, add it here
*
* \bug Description of the bug
*
*******************************************************************************/

void vRobotLeftTask(void *pvParameters)
{
    portTickType xLastFlashTime;

    /* We need to initialize xLastFlashTime prior to the first call to vTaskDelayUntil() */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    for(;;)
    {

    	/* Prepare robot to take up an ECTS */
    	createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[PrepareToTakeEcts]);
        vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);


    	/* Wait for an ECTS that can be taken */
    	while(xSemaphoreTake(xSemaphoreLeftECTS, SEMAPHORE_TAKE_DELAY) == pdFALSE);


    	// ECTS greifen
    	uint8_t temp[] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    	createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, temp);


    	/* Lift the ECTS off the conveyor */
    	createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[LiftEctsUp]);
        vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);


    	/* Release semaphore that locks the conveyor from moving */
    	xSemaphoreGive(xSemaphoreLeftConveyor);


    	/* Rotate to the unload tray */
    	createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[RotateToUnloadTray]);
        vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);


    	/* Wait for the drop off tray to be free of the other robot */
    	while(xSemaphoreTake(xSemaphoreEctsTray, SEMAPHORE_TAKE_DELAY) == pdFALSE);

    	/* Here we hold the semaphore */
    	{

    		/* Prepare to unload ECTS */
			createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[PrepareToUnloadEcts]);
			vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);

			/* Unload ECTS */
			createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[UnloadEctsToTray]);
			vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);


			/* Remove robot arm from the drop off tray */
			createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[RemoveArmFromUnloadTray]);
			vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);

    	}

    	/* Release the semaphore that kept the drop off tray free of the other robot */
    	xSemaphoreGive(xSemaphoreEctsTray);


    	/* Rotate back to the conveyor */
    	createCANMessage(CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[RotateToConveyor]);
        vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);

    }
}

/* ****************************************************************************/
/* End      :  vRobotLeftTask												  */
/* ****************************************************************************/
