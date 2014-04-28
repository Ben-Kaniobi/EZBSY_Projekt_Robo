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
#define UI_STACK_SIZE		configMINIMAL_STACK_SIZE * 2 		/*!< size of the task's stack */
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
#define LCD_TITLE_TEXT			("Robot Model")
#define LCD_SWITCH_0_ON_TEXT	("ECTS 1 status msg on ")
#define LCD_SWITCH_0_OFF_TEXT	("ECTS 1 status msg off")
#define LCD_SWITCH_1_ON_TEXT	("ECTS 2 status msg on ")
#define LCD_SWITCH_1_OFF_TEXT	("ECTS 2 status msg off")
#define LCD_SWITCH_2_ON_TEXT	("ECTS 3 status msg on ")
#define LCD_SWITCH_2_OFF_TEXT	("ECTS 3 status msg off")


#define CONVEYOR_L_X_OFFSET		(137)
#define CONVEYOR_L_Y_OFFSET		(192)
#define CONVEYOR_R_X_OFFSET		(183)
#define CONVEYOR_R_Y_OFFSET		(210)
#define CONVEYOR_C_X_OFFSET		(152)
#define CONVEYOR_C_Y_OFFSET		(91)

#define PIXEL_PER_STEP			(4)
#define ECTS_PIXEL_PER_STEP		(2)

#define LINE_LEFT_START			(135)
#define LINE_LEFT_STOP			(31)
#define LINE_RIGHT_START		(186)
#define LINE_RIGHT_STOP			(290)
#define LINE_CENTER_START		(96)
#define LINE_CENTER_STOP		(200)


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitUITask(void);

/* ****************************************************************************/
/* End Header : UI_task.h													  */
/* ****************************************************************************/
#endif /* __APP_UI_TASK_H_ */
