#ifndef __APP_INIT_ROBOTS_H_
#define __APP_INIT_ROBOTS_H_
/******************************************************************************/
/*! \file init_robot_tasks.h
******************************************************************************
* \brief Initiates the robot tasks
*
* Function : This file provides a function to initialize the robot tasks for
* both arms
*
* Procedures : InitRobotTasks()
*
* \author plats1
*
* \version 1.0.0
*
* \history 28.04.2014 File Created
*
*
* \ingroup application
*/
/* ****************************************************************************/
/* Project name																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* Robot left task RTOS configuration */
#define ROBOT_LEFT_TASK_NAME		"LeftRobot"						/*!< Name of the task */
#define ROBOT_LEFT_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< Size of the task's stack */
#define ROBOT_LEFT_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< Priority of the task */

/* Robot right task RTOS configuration */
#define ROBOT_RIGHT_TASK_NAME		"RightRobot"					/*!< Name of the task */
#define ROBOT_RIGHT_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< Size of the task's stack */
#define ROBOT_RIGHT_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< Priority of the task */

/* Can IDs of both robot arms (only one ID per arm, as we send nothing but instructions) */
#define CAN_ID_LEFT_ROBOT			( 0x152 )
#define CAN_ID_RIGHT_ROBOT			( 0x162 )

#define CAN_INSTRUCTION_DLC			( 6 )

/* ------------------------- module type declaration -------------------------*/

/*! \enum robotPosition
* \brief Enumeration of the different robot gestures
*
* This enumeration provides names for the different robot gestures.
*/
enum robotPosition
{
	WaitingPosition,
	PrepareToTakeEcts,
	LiftEctsUp,
	RotateToUnloadTray,
	PrepareToUnloadEcts,
	UnloadEctsToTray1,
	UnloadEctsToTray2,
	RemoveArmFromUnloadTray,
	RotateToConveyor
};

/* ------------------------- module data declaration -------------------------*/

extern xSemaphoreHandle xSemaphoreLeftConveyor;				/*!< Left conveyor is free */
extern xSemaphoreHandle xSemaphoreRightConveyor;			/*!< Right conveyor is free */

extern xSemaphoreHandle xSemaphoreEctsTray;					/*!< Drop off tray is free */

/* ----------------------- module procedure declaration ----------------------*/

extern void InitRobotTasks();

/* ****************************************************************************/
/* End Header : init_robot_tasks.h												  */
/* ****************************************************************************/
#endif /* __APP_INIT_ROBOTS_H_ */
