#ifndef __APP_INIT_ROBOTS_H_
#define __APP_INIT_ROBOTS_H_
/******************************************************************************/
/*! \file robot_left_task.h
******************************************************************************
* \brief Short description of the files function
*
* Function : More detailed description of the files function
*
* Procedures : InitRobotLeftTask()
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
* \todo If u have some todo's for the h-file, add it here
*
*/
/* ****************************************************************************/
/* Project name																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* Robot left task RTOS configuration */
#define ROBOT_LEFT_TASK_NAME		"Robot Left Task"				/*!< Name of the task */
#define ROBOT_LEFT_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define ROBOT_LEFT_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */

/* Robot right task RTOS configuration */
#define ROBOT_RIGHT_TASK_NAME		"Robot Right Task"				/*!< Name of the task */
#define ROBOT_RIGHT_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define ROBOT_RIGHT_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */

/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/
extern xSemaphoreHandle xSemaphoreLeftConveyor;				/* Left conveyor is free */
extern xSemaphoreHandle xSemaphoreRightConveyor;			/* Right conveyor is free */

extern xSemaphoreHandle xSemaphoreEctsTray;					/* Drop off tray is free */

/* ----------------------- module procedure declaration ----------------------*/

extern void InitRobotTasks();

/* ****************************************************************************/
/* End Header : robot_Left_task.h												  */
/* ****************************************************************************/
#endif /* __APP_INIT_ROBOTS_H_ */
