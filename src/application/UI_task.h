#ifndef __APP_UI_TASK_H_
#define __APP_UI_TASK_H_
/******************************************************************************/
/*! \file UI_task.h
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
#define UI_TASK_NAME		"UI Task"						/*!< Name of the task */
#define UI_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define UI_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */

#define LCD_COLOR_CONVEYOR_GREEN	(0x03E0)
#define LCD_COLOR_LINE_GREEN		(0x05E0)
#define LCD_COLOR_ECTS_BLACK		(0x0000)

/* Positions for the text on the LCD*/
#define LCD_TITLE_Y_POS			(6)
#define LCD_TEXT_X_POS			(10)
#define LCD_TEXT_Y_POS			(24)
#define LCD_TEXT_OFFSET			(15)

/* Text definitons on the LCD */
#define LCD_TITLE_TEXT			("Hello World")
#define LCD_SWITCH_0_ON_TEXT	("Switch S0 on ")
#define LCD_SWITCH_0_OFF_TEXT	("Switch S0 off")
#define LCD_SWITCH_1_ON_TEXT	("Switch S1 on ")
#define LCD_SWITCH_1_OFF_TEXT	("Switch S1 off")
#define LCD_SWITCH_2_ON_TEXT	("Switch S2 on ")
#define LCD_SWITCH_2_OFF_TEXT	("Switch S2 off")


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

extern void InitUITask(void);

/* ****************************************************************************/
/* End Header : UI_task.h													  */
/* ****************************************************************************/
#endif /* __APP_UI_TASK_H_ */
