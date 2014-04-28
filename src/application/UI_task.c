/******************************************************************************/
/*! \file UI_task.c
******************************************************************************
* \brief File to draw the state of the robo model
*
* Function : This file draws the position of the ECTS on the display,
* animate the conveyors and shows the status of the switches used for UART msg's
*
* Procedures : 	vLCDTask(void*)
* 				InitLCDTask()
*              	updateECTS()
*              	drawECTS()
*              	updateConveyorLeft()
*              	updateConveyorCenter()
*              	updateConveyorRight()
*              	uartPrintHandler()
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
/* Robo model 																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/

#include <memPoolService.h>		/* Memory pool manager service */
#include "stm32f4xx.h"			/* uC includes */

/* application */
#include "UI_task.h"			/* Own header include */
#include "ECTS_updater_task.h"	/* Include the ECTS */
#include "UART_task.h"			/* Include the UART task for msg printing */

#include  "inc/lcd.h"			/* Graphic Library */
#include <ff.h>					/* FatFs */
#include "carme_io1.h"			/* Carme IO include */
#include <stdio.h>				/* Include with sprintf */


/* ----------------------- module constant declaration -----------------------*/

/* Colors used for drawings: */
#define LCD_COLOR_CONVEYOR_GREEN	(0x03E0)	/* Color of the conveyors */
#define LCD_COLOR_LINE_GREEN		(0x05E0)	/* Color of the lines for the animation */
#define LCD_COLOR_ECTS_BLACK		(0x0000)	/* Color of the ECTS */

/* Positions for the text on the LCD: */
#define LCD_TITLE_Y_POS			(6)		/* Y position of the title  */
#define LCD_TEXT_X_POS			(10)	/* X position of the text on top of the LCD */
#define LCD_TEXT_Y_POS			(24)	/* Y position of the text on top of the LCD */
#define LCD_TEXT_OFFSET			(15)	/* Offset in pixel between the text lines */

/* Text definitons on the LCD: */
#define LCD_TITLE_TEXT			("Robot Model")
#define LCD_SWITCH_0_ON_TEXT	("ECTS 1 status msg on ")
#define LCD_SWITCH_0_OFF_TEXT	("ECTS 1 status msg off")
#define LCD_SWITCH_1_ON_TEXT	("ECTS 2 status msg on ")
#define LCD_SWITCH_1_OFF_TEXT	("ECTS 2 status msg off")
#define LCD_SWITCH_2_ON_TEXT	("ECTS 3 status msg on ")
#define LCD_SWITCH_2_OFF_TEXT	("ECTS 3 status msg off")

/* Offsets used to draw the ECTS on the conveyors */
#define CONVEYOR_L_X_OFFSET		(137)
#define CONVEYOR_L_Y_OFFSET		(192)
#define CONVEYOR_R_X_OFFSET		(183)
#define CONVEYOR_R_Y_OFFSET		(210)
#define CONVEYOR_C_X_OFFSET		(152)
#define CONVEYOR_C_Y_OFFSET		(91)

/* Defines for the drawing: */
#define PIXEL_PER_STEP			(4)		/* Number of pixels per update setp of the conveyor lines */
#define ECTS_PIXEL_PER_STEP		(2)		/* Factor to calculate the pos of an ECTS on the conveyor graphic */

/* Defines for the animation of the conveyors */
#define LINE_LEFT_START			(135)	/* Start position of the lines on the left conveyor */
#define LINE_LEFT_STOP			(31)	/* End position of the lines on the left conveyor */
#define LINE_RIGHT_START		(186)	/* Start position of the lines on the right conveyor */
#define LINE_RIGHT_STOP			(290)	/* End position of the lines on the right conveyor */
#define LINE_CENTER_START		(96)	/* Start position of the lines on the center conveyor */
#define LINE_CENTER_STOP		(200)	/* End position of the lines on the center conveyor */


/* ------------------------- module data declaration -------------------------*/

FATFS lcd_fs;		/*!< FatFs variable */

ects oldECTS[3];	/*!< Array of 3 ECTS-structs to save the last drawn position */

/* Variables for the left conveyor animation */
int xLine1Left = 135;
int xLine2Left = 99;
int xLine3Left = 63;
int yLinesLeft = 191;

/* Variables for the right conveyor animation */
int xLine1Right = 186;
int xLine2Right = 222;
int xLine3Right = 258;
int yLinesRight = 191;

/* Variables for the middle conveyor animation */
int yLine1Center = 96;
int yLine2Center = 132;
int yLine3Center = 168;
int xLinesCenter = 150;

/* ----------------------- module procedure declaration ----------------------*/

static void vUITask(void*);
static void updateECTS(ects myECTS);
static void drawECTS(uint16_t Xpos, uint16_t Ypos, LCDCOLOR Color);
static void updateConveyorLeft(void);
static void updateConveyorCenter(void);
static void updateConveyorRight(void);
static void uartPrintHandler(void);


/* ****************************************************************************/
/* End Header : UI_task.c													  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  InitUITask													  */
/******************************************************************************/
/*! \brief Init function of the UI task
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 24.03.2014 Function Created
*
*******************************************************************************/

void InitUITask(void)
{
	/* Init of the LCD */
	LCD_Init();

	/* init the carme IO's */
	CARME_IO1_Init();

	/* FatFs mount */
	if (f_mount(&lcd_fs, "0:", 1) != FR_OK) {
		//Default_Handler(); //TODO remove if not used
	}

	/* Draw background of the screen */
    LCD_BMP_DrawBitmap("Back.bmp", 0, 0);

    /* Display title*/
    LCD_SetTextColor(GUI_COLOR_BLACK);
    LCD_SetBackColor(GUI_COLOR_WHITE);
    LCD_SetFont(&font_9x15B);
    LCD_DisplayStringCenter(6, "Hello World");

    /* create the task */
    xTaskCreate( vUITask, ( signed char * ) UI_TASK_NAME, UI_STACK_SIZE, NULL, UI_TASK_PRIORITY, NULL );
}

/* ****************************************************************************/
/* End      :  InitUITask													  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  vUITask														  */
/******************************************************************************/
/*! \brief The Ui's function itself
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 24.03.2014 Function Created
*
*******************************************************************************/

static void vUITask(void* pvParameters )
{
    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    for(;;)
    {
    	/* Counter variable for the for control structure */
    	int counter;


    	for(counter = 0; counter < 3; counter++)
    	{
        	/* Clear the old position of the ECTS (unless it's 0:0) */
        	if(oldECTS[counter].x != 0 && oldECTS[counter].y != 0)
        	{
        		drawECTS(oldECTS[counter].x, oldECTS[counter].y, LCD_COLOR_CONVEYOR_GREEN);
        	}
    	}

//    	/* Test ECTS for positioning */
//    	ects TestECTS_1 = {0, 0, 7, conveyor_L};
//    	ects TestECTS_2 = {1, 0, 7, conveyor_C};
//    	ects TestECTS_3 = {2, 0, 7, conveyor_R};

    	/* Update the screen */
    	updateConveyorLeft();
    	updateConveyorRight();
    	updateConveyorCenter();
    	updateECTS(ECTS_1);
    	updateECTS(ECTS_2);
    	updateECTS(ECTS_3);

    	/* runthe UART print handler */
    	uartPrintHandler();

    	/* Wait for 200 ms */
        vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
    }
}

/* ****************************************************************************/
/* End      :  vUITask														  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  updateECTS													  */
/******************************************************************************/
/*! \brief Function to update the ECTS's on the LCD
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 7.04.2014 Function created
*
*******************************************************************************/

static void updateECTS(ects myECTS)
{
	uint16_t x,y;

	switch (myECTS.z)
	{
		/* If the ECTS is on the left conveyor */
		case conveyor_L:

			/* Calculate the pixel position of the ECTS */
			x = CONVEYOR_L_X_OFFSET - ECTS_PIXEL_PER_STEP * myECTS.x;
			y = CONVEYOR_L_Y_OFFSET + ECTS_PIXEL_PER_STEP * myECTS.y;
			break;

		/* If the ECTS is on the middle conveyor */
		case conveyor_C:

			/* Calculate the pixel position of the ECTS */
			x = CONVEYOR_C_X_OFFSET + ECTS_PIXEL_PER_STEP * myECTS.y;
			y = CONVEYOR_C_Y_OFFSET + ECTS_PIXEL_PER_STEP * myECTS.x;
			break;

		/* If the ECTS is on the right conveyor */
		case conveyor_R:

			/* Calculate the pixel position of the ECTS */
			x = CONVEYOR_R_X_OFFSET + ECTS_PIXEL_PER_STEP * myECTS.x;
			y = CONVEYOR_R_Y_OFFSET - ECTS_PIXEL_PER_STEP * myECTS.y;
			break;

		/* If the ECTS is lifted by the left roboter */
		case robo_L:

			/* Set the position of the ECTS to 0:0 */
			x = 0;
			y = 0;
			//TODO draw robo action
			break;

		/* If the ECTS is lifted by the right roboter */
		case robo_R:

			/* Set the position of the ECTS to 0:0 */
			x = 0;
			y = 0;
			//TODO draw robo action
			break;

		default:
			//TODO Print error msg
			break;
	}

	/* Draw the new position of the ECTS (unless it's 0:0) */
	if(x != 0 && y != 0)
	{
		drawECTS(x, y, LCD_COLOR_ECTS_BLACK);
	}

	/* Update the old ECTS position */
	oldECTS[myECTS.id].x = x;
	oldECTS[myECTS.id].y = y;
	oldECTS[myECTS.id].z = myECTS.z;

}

/* ****************************************************************************/
/* End      :  updateECTS													  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  drawECTS														  */
/******************************************************************************/
/*! \brief Function to draw an ECTS on the screen
*
* \param[in] Xpos  	Pointer The x position of the ECTS
* \param[in] Ypos  	Pointer The y position of the ECTS
* \param[in] Color 	The ECTS's color
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 31.03.2014 Function Created
*
*******************************************************************************/

static void drawECTS(uint16_t Xpos, uint16_t Ypos, LCDCOLOR Color)
{

	LCD_DrawRectF(Xpos-1, Ypos-3, 2, 6, Color);

	LCD_DrawLine(Xpos-3, Ypos-1, Xpos-3, Ypos+1, Color);
	LCD_DrawLine(Xpos-2, Ypos-2, Xpos-2, Ypos+2, Color);
	LCD_DrawLine(Xpos+2, Ypos-2, Xpos+2, Ypos+2, Color);
	LCD_DrawLine(Xpos+3, Ypos-1, Xpos+3, Ypos+1, Color);

}

/* ****************************************************************************/
/* End      :  drawECTS														  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  updateConveyorLeft 											  */
/******************************************************************************/
/*! \brief Update function for the left conveyor
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 31.03.2014 Function Created
*
*******************************************************************************/

static void updateConveyorLeft(void)
{
	/* Set the running flag manual for demonstration*/
	int running = 1;

	/* If the conveyor is running */
	if(running)
	{
		/* Animation of the conveyor: */

		/* Update the first line of the animation */
    	if(xLine1Left <= LINE_LEFT_STOP )
    	{
    		xLine1Left = LINE_LEFT_START;
    		LCD_DrawLine(LINE_LEFT_STOP, yLinesLeft, LINE_LEFT_STOP, yLinesLeft+21, LCD_COLOR_CONVEYOR_GREEN);
    	}
    	else
    	{
    		xLine1Left-=PIXEL_PER_STEP;
        	LCD_DrawLine(xLine1Left+PIXEL_PER_STEP, yLinesLeft, xLine1Left+PIXEL_PER_STEP, yLinesLeft+21, LCD_COLOR_CONVEYOR_GREEN);
        	LCD_DrawLine(xLine1Left, yLinesLeft, xLine1Left, yLinesLeft+21, LCD_COLOR_LINE_GREEN);
    	}

    	/* Update the second line of the animation */
    	if(xLine2Left <= LINE_LEFT_STOP )
    	{
    		xLine2Left = LINE_LEFT_START;
    		LCD_DrawLine(LINE_LEFT_STOP, yLinesLeft, LINE_LEFT_STOP, yLinesLeft+21, LCD_COLOR_CONVEYOR_GREEN);
    	}
    	else
    	{
    		xLine2Left-=PIXEL_PER_STEP;
        	LCD_DrawLine(xLine2Left+PIXEL_PER_STEP, yLinesLeft, xLine2Left+PIXEL_PER_STEP, yLinesLeft+21, LCD_COLOR_CONVEYOR_GREEN);
        	LCD_DrawLine(xLine2Left, yLinesLeft, xLine2Left, yLinesLeft+21, LCD_COLOR_LINE_GREEN);
    	}

    	/* Update the third line of the animation */
    	if(xLine3Left <= LINE_LEFT_STOP )
    	{
    		xLine3Left = LINE_LEFT_START;
    		LCD_DrawLine(LINE_LEFT_STOP, yLinesLeft, LINE_LEFT_STOP, yLinesLeft+21, LCD_COLOR_CONVEYOR_GREEN);
    	}
    	else
    	{
    		xLine3Left-=PIXEL_PER_STEP;
        	LCD_DrawLine(xLine3Left+PIXEL_PER_STEP, yLinesLeft, xLine3Left+PIXEL_PER_STEP, yLinesLeft+21, LCD_COLOR_CONVEYOR_GREEN);
        	LCD_DrawLine(xLine3Left, yLinesLeft, xLine3Left, yLinesLeft+21, LCD_COLOR_LINE_GREEN);
    	}
	}
}

/* ****************************************************************************/
/* End      :  updateConveyorLeft											  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  updateConveyorRight 											  */
/******************************************************************************/
/*! \brief Update function for the left conveyor
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 31.03.2014 Function Created
*
*******************************************************************************/

static void updateConveyorRight(void)
{
	/* Set the running flag manual for demonstration*/
	int running = 1;

	/* If the conveyor is running */
	if(running)
	{

		/* Update the first line of the animation */
    	if(xLine1Right >= LINE_RIGHT_STOP )
    	{
    		xLine1Right = LINE_RIGHT_START;
    		LCD_DrawLine(LINE_RIGHT_STOP, yLinesRight, LINE_RIGHT_STOP, yLinesRight+21, LCD_COLOR_CONVEYOR_GREEN);
    	}
    	else
    	{
    		xLine1Right+=PIXEL_PER_STEP;
        	LCD_DrawLine(xLine1Right-PIXEL_PER_STEP, yLinesRight, xLine1Right-PIXEL_PER_STEP, yLinesRight+21, LCD_COLOR_CONVEYOR_GREEN);
        	LCD_DrawLine(xLine1Right, yLinesRight, xLine1Right, yLinesRight+21, LCD_COLOR_LINE_GREEN);
    	}

    	/* Update the second line of the animation */
    	if(xLine2Right >= LINE_RIGHT_STOP )
    	{
    		xLine2Right = LINE_RIGHT_START;
    		LCD_DrawLine(LINE_RIGHT_STOP, yLinesRight, LINE_RIGHT_STOP, yLinesRight+21, LCD_COLOR_CONVEYOR_GREEN);
    	}
    	else
    	{
    		xLine2Right+=PIXEL_PER_STEP;
        	LCD_DrawLine(xLine2Right-PIXEL_PER_STEP, yLinesRight, xLine2Right-PIXEL_PER_STEP, yLinesRight+21, LCD_COLOR_CONVEYOR_GREEN);
        	LCD_DrawLine(xLine2Right, yLinesRight, xLine2Right, yLinesRight+21, LCD_COLOR_LINE_GREEN);
    	}

    	/* Update the third line of the animation */
    	if(xLine3Right >= LINE_RIGHT_STOP )
    	{
    		xLine3Right = LINE_RIGHT_START;
    		LCD_DrawLine(LINE_RIGHT_STOP, yLinesRight, LINE_RIGHT_STOP, yLinesRight+21, LCD_COLOR_CONVEYOR_GREEN);
    	}
    	else
    	{
    		xLine3Right+=PIXEL_PER_STEP;
        	LCD_DrawLine(xLine3Right-PIXEL_PER_STEP, yLinesRight, xLine3Right-PIXEL_PER_STEP, yLinesRight+21, LCD_COLOR_CONVEYOR_GREEN);
        	LCD_DrawLine(xLine3Right, yLinesRight, xLine3Right, yLinesRight+21, LCD_COLOR_LINE_GREEN);
    	}

	}
}

/* ****************************************************************************/
/* End      :  updateConveyorRight											  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  updateConveyorCenter 											  */
/******************************************************************************/
/*! \brief Update function for the middle conveyor
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 31.03.2014 Function Created
*
*******************************************************************************/

static void updateConveyorCenter(void)
{

	/* Update the first line of the animation */
	if(yLine1Center >= LINE_CENTER_STOP )
	{
		yLine1Center = LINE_CENTER_START;
		LCD_DrawLine(xLinesCenter, LINE_CENTER_STOP, xLinesCenter+21, LINE_CENTER_STOP, LCD_COLOR_CONVEYOR_GREEN);
	}
	else
	{
		yLine1Center += PIXEL_PER_STEP;
		LCD_DrawLine(xLinesCenter, yLine1Center-PIXEL_PER_STEP, xLinesCenter+21, yLine1Center-PIXEL_PER_STEP, LCD_COLOR_CONVEYOR_GREEN);
		LCD_DrawLine(xLinesCenter, yLine1Center, xLinesCenter+21, yLine1Center, LCD_COLOR_LINE_GREEN);
	}

	/* Update the second line of the animation */
	if(yLine2Center >= LINE_CENTER_STOP )
	{
		yLine2Center = LINE_CENTER_START;
		LCD_DrawLine(xLinesCenter, LINE_CENTER_STOP, xLinesCenter+21, LINE_CENTER_STOP, LCD_COLOR_CONVEYOR_GREEN);
	}
	else
	{
		yLine2Center += PIXEL_PER_STEP;
		LCD_DrawLine(xLinesCenter, yLine2Center-PIXEL_PER_STEP, xLinesCenter+21, yLine2Center-PIXEL_PER_STEP, LCD_COLOR_CONVEYOR_GREEN);
		LCD_DrawLine(xLinesCenter, yLine2Center, xLinesCenter+21, yLine2Center, LCD_COLOR_LINE_GREEN);
	}

	/* Update the third line of the animation */
	if(yLine3Center >= LINE_CENTER_STOP )
	{
		yLine3Center = LINE_CENTER_START;
		LCD_DrawLine(xLinesCenter, LINE_CENTER_STOP, xLinesCenter+21, LINE_CENTER_STOP, LCD_COLOR_CONVEYOR_GREEN);
	}
	else
	{
		yLine3Center += PIXEL_PER_STEP;
		LCD_DrawLine(xLinesCenter, yLine3Center-PIXEL_PER_STEP, xLinesCenter+21, yLine3Center-PIXEL_PER_STEP, LCD_COLOR_CONVEYOR_GREEN);
		LCD_DrawLine(xLinesCenter, yLine3Center, xLinesCenter+21, yLine3Center, LCD_COLOR_LINE_GREEN);
	}

}

/* ****************************************************************************/
/* End      :  updateConveyorCenter											  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  uartPrintHandler												  */
/******************************************************************************/
/*! \brief Function handle the information to print over uart
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 26.04.2014 Function Created
*
*******************************************************************************/

static void uartPrintHandler(void)
{
	/* Variable to hold the switch state */
	uint8_t switchState = 0;

	/* Char array for the print mesage */
	char ECTSString[50];

	/* get the switch state */
	CARME_IO1_SWITCH_Get(&switchState);

	/* Check if the switch S0 is set */
	if(switchState & 0x01)
	{
		/* Display the state of the switch on the LCD */
		LCD_DisplayStringXY(LCD_TEXT_X_POS, LCD_TEXT_Y_POS, LCD_SWITCH_0_ON_TEXT);

		/* Create a new mesage and send it via UART */
		sprintf(ECTSString, "ECTS 1 Position X:%i Y:%d", (int)ECTS_1.x, (int)ECTS_1.y);
		createUARTMessage(ECTSString);
	}
	else
	{
		/* Display the state of the switch on the LCD */
		LCD_DisplayStringXY(LCD_TEXT_X_POS, LCD_TEXT_Y_POS, LCD_SWITCH_0_OFF_TEXT);
	}

	/* Check if the switch S1 is set */
	if(switchState & 0x02)
	{
		/* Display the state of the switch on the LCD */
		LCD_DisplayStringXY(LCD_TEXT_X_POS, LCD_TEXT_Y_POS + LCD_TEXT_OFFSET, LCD_SWITCH_1_ON_TEXT);

		/* Create a new mesage and send it via UART */
		sprintf(ECTSString, "ECTS 2 Position X:%i Y:%d", (int)ECTS_2.x, (int)ECTS_2.y);
		createUARTMessage(ECTSString);
	}
	else
	{
		/* Display the state of the switch on the LCD */
		LCD_DisplayStringXY(LCD_TEXT_X_POS, LCD_TEXT_Y_POS + LCD_TEXT_OFFSET, LCD_SWITCH_1_OFF_TEXT);
	}

	/* Check if the switch S2 is set */
	if(switchState & 0x04)
	{
		/* Display the state of the switch on the LCD */
		LCD_DisplayStringXY(LCD_TEXT_X_POS, LCD_TEXT_Y_POS + 2 * LCD_TEXT_OFFSET, LCD_SWITCH_2_ON_TEXT);

		/* Create a new mesage and send it via UART */
		sprintf(ECTSString, "ECTS 3 Position X:%i Y:%d", (int)ECTS_3.x, (int)ECTS_3.y);
		createUARTMessage(ECTSString);
	}
	else
	{
		/* Display the state of the switch on the LCD */
		LCD_DisplayStringXY(LCD_TEXT_X_POS, LCD_TEXT_Y_POS + 2 * LCD_TEXT_OFFSET, LCD_SWITCH_2_OFF_TEXT);
	}

	/* Update the LED's */
	CARME_IO1_LED_Set(switchState, 0x07);
}

/* ****************************************************************************/
/* End      :  uartPrintHandler												  */
/* ****************************************************************************/
