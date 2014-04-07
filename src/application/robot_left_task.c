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
#include "init_robot_tasks.h"
#include "robot_left_task.h"		/* Own header include */


/* ------------------------- module data declaration -------------------------*/

//int testVar = 0;				/*!< Detailed description after the member */

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

    /* We need to initialize xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    for(;;)
    {

    	// Hand bereitmachen

    	// TODO
    	/* Wait for an ECTS that can be taken */
//    	while(xSemaphoreTake(semaphore_von_kani, SEMAPHORE_TAKE_DELAY) == pdFALSE);


    	// ECTS greifen


    	// Hand anheben


    	/* Give semaphore that locks the left conveyor */
    	xSemaphoreGive(xSemaphoreLeftConveyor);


    	// hand bereitmachen zum ablegen


    	/* Wait for the drop off tray to be free (of other robot arm) */
    	while(xSemaphoreTake(xSemaphoreEctsTray, SEMAPHORE_TAKE_DELAY) == pdFALSE);


    	// ECTS ablegen

    	// Hand aus Ablagebereich entfernen

    	/* Give semaphore that kept the drop off tray free of the other robot arm */
    	xSemaphoreGive(xSemaphoreEctsTray);








    	/* Delay the task for some time */
        vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);
    }
}

/* ****************************************************************************/
/* End      :  vRobotLeftTask												  */
/* ****************************************************************************/
