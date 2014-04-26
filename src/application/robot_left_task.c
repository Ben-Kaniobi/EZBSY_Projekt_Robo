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
#include "ECTS_updater_task.h"


/* ------------------------- module data declaration -------------------------*/

uint8_t payloadLeft[ ][ 8 ] =
{
	{ 0x02, 0x80, 0x5B, 0xC9, 0xD0, 0xAA, 0x00, 0x00 },	/* Waiting position */
	{ 0x02, 0x80, 0xA0, 0xD5, 0x70, 0x87, 0x00, 0x00 },	/* PrepareToTakeEcts */
	{ 0x02, 0x80, 0x91, 0xD5, 0x70, 0xAA, 0x00, 0x00 },	/* LiftEctsUp */
	{ 0x02, 0xCD, 0x91, 0xC3, 0x85, 0xAA, 0x00, 0x00 },	/* RotateToUnloadTray */
	{ 0x02, 0xCD, 0x9F, 0xC3, 0x85, 0xAA, 0x00, 0x00 },	/* PrepareToUnloadEcts */
	{ 0x02, 0xCD, 0x9F, 0xC3, 0x85, 0x87, 0x00, 0x00 },	/* UnloadEctsToTray */
	{ 0x02, 0xCD, 0x91, 0xC3, 0x85, 0xAA, 0x00, 0x00 },	/* RemoveArmFromUnloadTray */
	{ 0x02, 0x80, 0x9C, 0xBB, 0x85, 0x87, 0x00, 0x00 }	/* RotateToConveyor */
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

void vRobotLeftTask( void *pvParameters )
{
	/* Bring robot to a defined position */
	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ WaitingPosition ] );

	/* Wait 2000ms before starting to work */
    vTaskDelay( 2000 / portTICK_RATE_MS );

	/* Prepare robot to take up an ECTS */
	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ PrepareToTakeEcts ] );
    vTaskDelay( 1000 / portTICK_RATE_MS );

    for( ;; )
    {

    	/* Prepare robot to take up an ECTS */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ PrepareToTakeEcts ] );
        vTaskDelay( 1000 / portTICK_RATE_MS );


    	/* Wait for an ECTS that can be taken */
        xSemaphoreTake( xSemaphoreLeftECTS, portMAX_DELAY );


        /* Check which ECTS to handle */
        ects *ECTS = NULL;
        find_ECTS(ECTS, conveyor_L);

        /* Initialize standard grab gesture */
        uint8_t xEctsGrabPosition[ 8 ] = { 0x02, 0x80, 0xA0, 0xD5, 0x70, 0xAA, 0x00, 0x00 };

        /* Choose which position the ECTS actually has and adjust grab gesture accordingly */
        switch( ECTS->y )
        {
        	case 0:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 1:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 2:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 3:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 4:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 5:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 6:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

        	case 7:
				xEctsGrabPosition[2] = 0xA0;
				xEctsGrabPosition[3] = 0xD5;
				xEctsGrabPosition[4] = 0x70;
				break;

			default:	/* If the position is not valid */

				/* Let the conveyor move ahead (ignoring this ECTS) */
				xSemaphoreGive( xSemaphoreLeftConveyor );
				continue;	/* for */
        }

    	/* Grab the ECTS */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, xEctsGrabPosition );

    	/* Update ECTS position */
    	update_ECTS_z( robo_L );

        vTaskDelay( 1000 / portTICK_RATE_MS );




    	/* Lift the ECTS off the conveyor */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ LiftEctsUp ] );
        vTaskDelay( 1000 / portTICK_RATE_MS );


    	/* Release semaphore that locks the conveyor from moving */
    	xSemaphoreGive( xSemaphoreLeftConveyor );


    	/* Rotate to the unload tray */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ RotateToUnloadTray ] );
        vTaskDelay( 2000 / portTICK_RATE_MS );


    	/* Claim the drop off tray */
    	xSemaphoreTake( xSemaphoreEctsTray, portMAX_DELAY );
    	{

    		/* Prepare to unload ECTS */
			createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ PrepareToUnloadEcts ] );
		    vTaskDelay( 1000 / portTICK_RATE_MS );


			/* Unload ECTS */
			createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ UnloadEctsToTray ] );
		    vTaskDelay( 1000 / portTICK_RATE_MS );

		    /* Update ECTS location */
		    update_ECTS_z( conveyor_C );

			/* Remove robot arm from the drop off tray */
			createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ RemoveArmFromUnloadTray ] );
		    vTaskDelay( 1000 / portTICK_RATE_MS );

    	}
    	xSemaphoreGive( xSemaphoreEctsTray );


    	/* Rotate back to the conveyor */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ RotateToConveyor ] );
        vTaskDelay( 2000 / portTICK_RATE_MS );
    }
}

/* ****************************************************************************/
/* End      :  vRobotLeftTask												  */
/* ****************************************************************************/
