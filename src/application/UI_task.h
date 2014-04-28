#ifndef __APP_UI_TASK_H_
#define __APP_UI_TASK_H_
/******************************************************************************/
/*! \file UI_task.h
******************************************************************************
* \brief File to draw the state of the robo model
*
* Function : his file draws the position of the ECTS on the display,
* animate the conveyors and shows the status of the switches used for UART msg's
*
* Procedures : InitUITask()
*
* \author meert1
*
* \version 0.0.1
*
* \history 24.03.2014 File Created
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* Robo model																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* default-task RTOS configuration */
#define UI_TASK_NAME		"UI Task"						/*!< Name of the task */
#define UI_STACK_SIZE		configMINIMAL_STACK_SIZE * 2 		/*!< size of the task's stack */
#define UI_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitUITask(void);

/* ****************************************************************************/
/* End Header : UI_task.h													  */
/* ****************************************************************************/
#endif /* __APP_UI_TASK_H_ */
