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

#define LCD_COLOR_CONVEYOR_GREEN	(0x03E0)
#define LCD_COLOR_LINE_GREEN		(0x05E0)
#define LCD_COLOR_ECTS_BLACK		(0x0000)

#define CONVEYOR_L_X_OFFSET		(137)
#define CONVEYOR_L_Y_OFFSET		(200)
#define CONVEYOR_R_X_OFFSET		(184)
#define CONVEYOR_R_Y_OFFSET		(200)
#define CONVEYOR_C_X_OFFSET		(160)
#define CONVEYOR_C_Y_OFFSET		(90)

#define PIXEL_PER_STEP			(2)

#define LINE_LEFT_START			(135)
#define LINE_LEFT_STOP			(31)
#define LINE_RIGHT_START		(185)
#define LINE_RIGHT_STOP			(291)
#define LINE_CENTER_START		(93)
#define LINE_CENTER_STOP		(199)


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitLCDTask(void);

/* ****************************************************************************/
/* End Header : lcd_task.h													  */
/* ****************************************************************************/
#endif /* __APP_LCD_TASK_H_ */
