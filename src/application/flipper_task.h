/******************************************************************************/
/*! \file flipper_task.h
******************************************************************************
* \brief Task to handle the flipper and conveyor
*
* Procedures : 	InitFlipperTask()
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
#ifndef __APP_FLIPPER_TASK_H_
#define __APP_FLIPPER_TASK_H_

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* RTOS configuration */
#define FLIPPER_TASK_NAME			"Flipper task"
#define FLIPPER_TASK_STACK_SIZE	configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define FLIPPER_TASK_PRIORITY		(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitFlipperTask(void);

/* ****************************************************************************/
/* End Header : default_task.h												  */
/* ****************************************************************************/
#endif /* __APP_FLIPPER_TASK_H_ */
