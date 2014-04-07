/******************************************************************************/
/*! \file init_robot_tasks.c
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
#include "robot_left_task.h"
#include "robot_right_task.h"


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
/*! \brief Short description of the function
*
* Function : More detailed description of the function
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
