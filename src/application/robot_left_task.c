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
//	{ 0x02, 0x80, 0x91, 0xD5, 0x70, 0xAA, 0x00, 0x00 },	/* LiftEctsUp */
	{ 0x02, 0x80, 0x86, 0xDC, 0x85, 0xAA, 0x00, 0x00 },	/* LiftEctsUp */
	{ 0x02, 0xCD, 0x86, 0xDC, 0x85, 0xAA, 0x00, 0x00 },	/* RotateToUnloadTray */
	{ 0x02, 0xCD, 0x9F, 0xC3, 0x85, 0xAA, 0x00, 0x00 },	/* PrepareToUnloadEcts */
	{ 0x02, 0xCD, 0x9C, 0xBB, 0x85, 0x87, 0x00, 0x00 },	/* UnloadEctsToTray */
	{ 0x02, 0xCD, 0x9C, 0xBB, 0x85, 0x87, 0x00, 0x00 },	/* RemoveArmFromUnloadTray */
	{ 0x02, 0x80, 0x9C, 0xBB, 0x85, 0x87, 0x00, 0x00 }	/* RotateToConveyor */
};


uint8_t test = 0;

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

    /* Give unloading tray semaphore free */
    xSemaphoreGive( xSemaphoreEctsTray );

    /* Release semaphore that locks the conveyor from moving */
	xSemaphoreGive( xSemaphoreLeftConveyor );

	/* Prepare robot to take up an ECTS */
	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ PrepareToTakeEcts ] );
    vTaskDelay( 1500 / portTICK_RATE_MS );

    for( ;; )
    {

    	/* Prepare robot to take up an ECTS */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ PrepareToTakeEcts ] );
        vTaskDelay( 500 / portTICK_RATE_MS );


    	/* Wait for an ECTS that can be taken */
        xSemaphoreTake( xSemaphoreLeftECTS, portMAX_DELAY );


        /* Check which ECTS to handle */
        ects *ECTS = NULL;
        find_ECTS(&ECTS, conveyor_L);

        /* Initialize standard grab gesture */
        uint8_t xEctsGrabPosition[ 8 ] = { 0x02, 0x80, 0xA0, 0xD5, 0x70, 0x87, 0x00, 0x00 };

        test = ECTS->y;

        /* Choose which position the ECTS actually has and adjust grab gesture accordingly */
        switch( ECTS->y )
//        switch( 0 )
        {
        	case 0:
        		xEctsGrabPosition[2] = 0x9A;
        		xEctsGrabPosition[3] = 0xDA;
        		xEctsGrabPosition[4] = 0x73;
				break;

        	case 1:
				xEctsGrabPosition[2] = 0x9A;
				xEctsGrabPosition[3] = 0xD4;
				xEctsGrabPosition[4] = 0x78;
				break;

        	case 2:
				xEctsGrabPosition[2] = 0x9D;
				xEctsGrabPosition[3] = 0xD0;
				xEctsGrabPosition[4] = 0x7A;
				break;

        	case 3:
				xEctsGrabPosition[2] = 0x9F;
				xEctsGrabPosition[3] = 0xCB;
				xEctsGrabPosition[4] = 0x7C;
				break;

        	case 4:
				xEctsGrabPosition[2] = 0xA1;
				xEctsGrabPosition[3] = 0xC6;
				xEctsGrabPosition[4] = 0x7F;
				break;

        	case 5:
				xEctsGrabPosition[2] = 0xA4;
				xEctsGrabPosition[3] = 0xC1;
				xEctsGrabPosition[4] = 0x81;
				break;

        	case 6:
				xEctsGrabPosition[2] = 0xA7;
				xEctsGrabPosition[3] = 0xBC;
				xEctsGrabPosition[4] = 0x84;
				break;

        	case 7:
				xEctsGrabPosition[2] = 0xAC;
				xEctsGrabPosition[3] = 0xAF;
				xEctsGrabPosition[4] = 0x8B;
				break;

			default:	/* If the position is not valid */

				/* Let the conveyor move ahead (ignoring this ECTS) */
				xSemaphoreGive( xSemaphoreLeftConveyor );
				continue;	/* for */
        }

    	/* Prepare to grab the ECTS */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, xEctsGrabPosition );
        vTaskDelay( 600 / portTICK_RATE_MS );

        /* Grab the ECTS */
        xEctsGrabPosition[5] = 0xAA;
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, xEctsGrabPosition );

    	/* Update ECTS position */
    	ECTS->z = robo_L;
		ECTS->x = 0;

        vTaskDelay( 800 / portTICK_RATE_MS );




    	/* Lift the ECTS off the conveyor */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ LiftEctsUp ] );
        vTaskDelay( 400 / portTICK_RATE_MS );


    	/* Release semaphore that locks the conveyor from moving */
    	xSemaphoreGive( xSemaphoreLeftConveyor );


    	/* Rotate to the unload tray */
    	createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ RotateToUnloadTray ] );
        vTaskDelay( 1500 / portTICK_RATE_MS );


    	/* Claim the drop off tray */
    	xSemaphoreTake( xSemaphoreEctsTray, portMAX_DELAY );
    	{

    		/* Prepare to unload ECTS */
			createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ PrepareToUnloadEcts ] );
		    vTaskDelay( 1200 / portTICK_RATE_MS );


			/* Unload ECTS */
			createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ UnloadEctsToTray ] );
		    vTaskDelay( 600 / portTICK_RATE_MS );

		    /* Update ECTS location */
		    ECTS->z = conveyor_C;
			ECTS->x = 0;

			/* Remove robot arm from the drop off tray by rotating back to the conveyor */
			createCANMessage( CAN_ID_LEFT_ROBOT, CAN_INSTRUCTION_DLC, payloadLeft[ RotateToConveyor ] );
		    vTaskDelay( 1200 / portTICK_RATE_MS );

    	}
    	xSemaphoreGive( xSemaphoreEctsTray );
    }
}

/* ****************************************************************************/
/* End      :  vRobotLeftTask												  */
/* ****************************************************************************/
