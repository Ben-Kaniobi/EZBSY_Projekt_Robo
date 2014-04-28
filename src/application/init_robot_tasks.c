/******************************************************************************/
/*! \file init_robot_tasks.c
******************************************************************************
* \brief Initiates the robot tasks
*
* Function : This file provides a function to initialize the robot tasks for
* both arms
*
* Procedures : 	InitRobotTasks()
*
* \author plats1
*
* \version 1.0.0
*
* \history 28.04.2014 File Created
*
*
* \ingroup application
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
#include "init_robot_tasks.h"		/* Own .h file */
#include "robot_left_task.h"		/* Header of the left robot task */
#include "robot_right_task.h"		/* Header of the right robot task */


/* ------------------------- module data declaration -------------------------*/

xSemaphoreHandle xSemaphoreLeftConveyor;
xSemaphoreHandle xSemaphoreRightConveyor;
xSemaphoreHandle xSemaphoreEctsTray;

/* ----------------------- module procedure declaration ----------------------*/


/* ****************************************************************************/
/* End Header : init_robot_tasks.c											  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  InitRobotTasks												  */
/******************************************************************************/
/*! \brief Function to initialize the robot tasks
*
* Function : This function creates the required semaphores for task synchronisation
* between the two robot arms and the conveyors. Just after the semaphores have been
* created, both robots tasks are created.
*
* \return None
*
* \author plats1
*
* \version 1.0.0
*
* \date 28.04.2014 File Created
*
*******************************************************************************/

void InitRobotTasks()
{

	/* Initialize semaphore for the left conveyor (it is NOT available after initialization) */
	xSemaphoreLeftConveyor = xSemaphoreCreateBinary();
	xSemaphoreRightConveyor = xSemaphoreCreateBinary();
	xSemaphoreEctsTray = xSemaphoreCreateBinary();

    /* Create a task for the left robot arm */
    xTaskCreate(vRobotLeftTask, (signed char *)ROBOT_LEFT_TASK_NAME, ROBOT_LEFT_STACK_SIZE, NULL, ROBOT_LEFT_TASK_PRIORITY, NULL);

    /* Create a task for the right robot arm */
//    xTaskCreate(vRobotRightTask, (signed char *)ROBOT_RIGHT_TASK_NAME, ROBOT_RIGHT_STACK_SIZE, NULL, ROBOT_RIGHT_TASK_PRIORITY, NULL);

}

/* ****************************************************************************/
/* End      :  InitRobotTasks												  */
/* ****************************************************************************/
