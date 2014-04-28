/******************************************************************************/
/*! \file conveyor_tasks.h
******************************************************************************
* \brief Tasks to handle the left and right conveyors
*
* Procedures : 	InitConveyorTasks()
*
* \author kasen1
*
* \version 0.1.0
*
* \history 28.04.2014 Version 0.1.0
* \history 12.03.2014 Import from template by wht4
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* Robo model                                                                 */
/* ****************************************************************************/

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __APP_CONVEYOR_TASKS_H_
#define __APP_CONVEYOR_TASKS_H_

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* RTOS configuration */
#define CONVEYOR_L_TASK_NAME		"Conveyor left task"
#define CONVEYOR_R_TASK_NAME		"Conveyor right task"
#define CONVEYOR_TASK_STACK_SIZE	configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define CONVEYOR_TASK_PRIORITY		(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/
extern xSemaphoreHandle xSemaphoreLeftECTS;       /*!< Sync. Semaphore to tell if left ECTS is ready */
extern xSemaphoreHandle xSemaphoreRightECTS;      /*!< Sync. Semaphore to tell if right ECTS is ready */


/* ----------------------- module procedure declaration ----------------------*/

extern void InitConveyorTasks(void);

/* ****************************************************************************/
/* End Header : conveyor_tasks.h                                              */
/* ****************************************************************************/
#endif /* __APP_CONVEYOR_TASKS_H_ */
