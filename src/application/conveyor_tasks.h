/******************************************************************************/
/*! \file conveyor_tasks.h
******************************************************************************
* \brief Tasks to handle the left and right conveyors
*
* Procedures : 	vConveyorL_task(void*)
*               vConveyorR_task(void*)
* 				init_ConveyorL_task()
* 				init_ConveyorL_task()
*
* \author kasen1
*
* \version 0.0.1
*
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
extern xSemaphoreHandle xSemaphoreLeftECTS;				/* Left ECTS is ready */
extern xSemaphoreHandle xSemaphoreRightECTS;			/* Right ECTS is ready */


/* ----------------------- module procedure declaration ----------------------*/

extern void InitDefaultTask(void);

/* ****************************************************************************/
/* End Header : default_task.h												  */
/* ****************************************************************************/
#endif /* __APP_CONVEYOR_TASKS_H_ */
