#ifndef __APP_LCD_TASK_H_
#define __APP_LCD_TASK_H_
/******************************************************************************/
/*! \file lcd_task.h
******************************************************************************
* \brief File to draw the state of the robo model
*
* Function : More detailed description of the files function
*
* Procedures : InitLCDTask()
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
* \todo Add functionality
*
*/
/* ****************************************************************************/
/* Robo model																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* default-task RTOS configuration */
#define LCD_TASK_NAME		"LCD Task"						/*!< Name of the task */
#define LCD_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define LCD_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitLCDTask(void);

/* ****************************************************************************/
/* End Header : lcd_task.h													  */
/* ****************************************************************************/
#endif /* __APP_LCD_TASK_H_ */
