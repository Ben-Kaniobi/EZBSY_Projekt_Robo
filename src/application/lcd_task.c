/******************************************************************************/
/*! \file lcd_task.c
******************************************************************************
* \brief File to draw the state of the robo model
*
* Function : More detailed description of the files function
*
* Procedures : 	vLCDTask(void*)
* 				InitLCDTask()
*              	function3()...
*
* \author Name
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
#include "lcd_task.h"		/* Own header include */
#include "ECTS_updater_task.h"

#include  "inc/lcd.h"		/* Graphic Library */
#include <ff.h>				/* FatFs */


/* ------------------------- module data declaration -------------------------*/

FATFS lcd_fs;		/*!< FatFs variable */

ects oldECTS[3];

/* Variables for the left conveyor animation */
int xLine1Left = 135;
int xLine2Left = 101;
int xLine3Left = 65;
int yLinesLeft = 191;

/* Variables for the right conveyor animation */
int xLine1Right = 185;
int xLine2Right = 221;
int xLine3Right = 255;
int yLinesRight = 191;

/* Variables for the middle conveyor animation */
int yLine1Center = 93;
int yLine2Center = 129;
int yLine3Center = 163;
int xLinesCenter = 150;

/* ----------------------- module procedure declaration ----------------------*/

static void vLCDTask(void*);
static void updateECTS(ects myECTS);
static void drawECTS(uint16_t Xpos, uint16_t Ypos, LCDCOLOR Color);
static void updateConveyorLeft(void);
static void updateConveyorCenter(void);
static void updateConveyorRight(void);


/* ****************************************************************************/
/* End Header : lcd_task.c													  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  InitLCDTask													  */
/******************************************************************************/
/*! \brief Init function of the LCD task
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/

void InitLCDTask(void)
{
	/* Init of the LCD */
	LCD_Init();

//	/* FatFs mount */
//	if (f_mount(&lcd_fs, "0:", 1) != FR_OK) {
//		//Default_Handler(); //TODO remove if not used
//	}
//
//	/* Draw background of the screen */
//    LCD_BMP_DrawBitmap("Back.bmp", 0, 0);

    /* Display title*/
    LCD_SetTextColor(GUI_COLOR_BLACK);
    LCD_SetBackColor(GUI_COLOR_WHITE);
    LCD_SetFont(&font_9x15B);
    LCD_DisplayStringCenter(6, "Hello World");

    /* create the task */
    xTaskCreate( vLCDTask, ( signed char * ) LCD_TASK_NAME, LCD_STACK_SIZE, NULL, LCD_TASK_PRIORITY, NULL );
}

/* ****************************************************************************/
/* End      :  InitLCDTask													  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  vLCDTask														  */
/******************************************************************************/
/*! \brief The LCD function itself
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author meert1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/

static void vLCDTask(void* pvParameters )
{
    portTickType xLastFlashTime;

    /* We need to initialise xLastFlashTime prior to the first call to
    vTaskDelayUntil(). */
    xLastFlashTime = xTaskGetTickCount();

    /* wait */
    vTaskDelayUntil( &xLastFlashTime, 10 );

    for(;;)
    {
    	/* Code here */

    	updateConveyorLeft();
    	updateConveyorRight();
    	updateConveyorCenter();
    	updateECTS(ECTS_1);
    	updateECTS(ECTS_2);
    	updateECTS(ECTS_3);

        vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
    }
}

/* ****************************************************************************/
/* End      :  vLCDTask														  */
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
			x = CONVEYOR_L_X_OFFSET - PIXEL_PER_STEP * myECTS.x;
			y = CONVEYOR_L_Y_OFFSET + PIXEL_PER_STEP * myECTS.y;
			break;

		/* If the ECTS is on the middle conveyor */
		case conveyor_C:

			/* Calculate the pixel position of the ECTS */
			x = CONVEYOR_C_X_OFFSET + PIXEL_PER_STEP * myECTS.y;
			y = CONVEYOR_C_Y_OFFSET + PIXEL_PER_STEP * myECTS.x;
			break;

		/* If the ECTS is on the right conveyor */
		case conveyor_R:

			/* Calculate the pixel position of the ECTS */
			x = CONVEYOR_R_X_OFFSET + PIXEL_PER_STEP * myECTS.x;
			y = CONVEYOR_R_Y_OFFSET - PIXEL_PER_STEP * myECTS.y;
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

	/* Clear the old position of the ECTS (unless it's 0:0) */
	if(oldECTS[myECTS.id].x != 0 && oldECTS[myECTS.id].y != 0)
	{
		drawECTS(oldECTS[myECTS.id].x, oldECTS[myECTS.id].y, LCD_COLOR_CONVEYOR_GREEN);
	}

	/* Draw the new position of the ECTS (unless it's 0:0) */
	if(x != 0 && y != 0)
	{
		drawECTS(x, y, LCD_COLOR_ECTS_BLACK);
		//TODO Print draw msg
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
* \date 31.03.2014 File Created
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
* \date 31.03.2014 File Created
*
*******************************************************************************/

static void updateConveyorLeft(void)
{

	int running = 1;

	if(running)
	{
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
* \date 31.03.2014 File Created
*
*******************************************************************************/

static void updateConveyorRight(void)
{

	int running = 1;

	if(running)
	{
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
* \date 31.03.2014 File Created
*
*******************************************************************************/

static void updateConveyorCenter(void)
{

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
