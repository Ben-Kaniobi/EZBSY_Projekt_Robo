/******************************************************************************/
/*! \file robot_right_task.c
******************************************************************************
* \brief Task that controls the right robot arm
*
* Function : More detailed description of the files function
*
* Procedures : 	vRobotRightTask(void*)
* 				InitRobotRightTask()
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
#include "robot_right_task.h"		/* Own header include */


/* ------------------------- module data declaration -------------------------*/

//int testVar = 0;				/*!< Detailed description after the member */

/* ----------------------- module procedure declaration ----------------------*/


/* ****************************************************************************/
/* End Header : robot_right_task.c											  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  vRobotRightTask												  */
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

void vRobotRightTask(void *pvParameters)
{
    portTickType xLastFlashTime;

    /* We need to initialize xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    for(;;)
    {
    	/* Code here */



    	// Hand bereitmachen

    	// Auf Semaphore "ECTS links rdy" warten

    	// ECTS greifen

    	// Hand anheben

    	// Semaphore "band links frei" geben

    	// hand bereitmachen zum ablegen

    	// auf semaphore "ablage frei" warten

    	// ECTS ablegen

    	// Hand aus Ablagebereich entfernen

    	// Semaphore "ablage frei" freigeben









        vTaskDelayUntil(&xLastFlashTime, 20 / portTICK_RATE_MS);
    }
}

/* ****************************************************************************/
/* End      :  vRobotRightTask												  */
/* ****************************************************************************/
