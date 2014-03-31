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

#include  "inc/lcd.h"		/* Graphic Library */
#include <ff.h>				/* FatFs */


/* ------------------------- module data declaration -------------------------*/

FATFS lcd_fs;		/*!< FatFs variable */

/* Variables for the left conveyor animation */
int xLine1Left = 135;
int xLine2Left = 100;
int xLine3Left = 65;
int yLinesLeft = 191;

/* Variables for the right conveyor animation */
int xLine1Right = 185;
int xLine2Right = 220;
int xLine3Right = 255;
int yLinesRight = 191;

/* Variables for the middle conveyor animation */
int yLine1Center = 93;
int yLine2Center = 128;
int yLine3Center = 163;
int xLinesCenter = 150;

/* ----------------------- module procedure declaration ----------------------*/

static void vLCDTask(void*);
static void drawECTS(uint8_t Xpos, uint16_t Ypos, LCDCOLOR Color);
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

        vTaskDelayUntil( &xLastFlashTime, 100 / portTICK_RATE_MS);
    }
}

/* ****************************************************************************/
/* End      :  vLCDTask														  */
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

static void drawECTS(uint8_t Xpos, uint16_t Ypos, LCDCOLOR Color)
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
    	if(xLine1Left <= 30 )
    	{
    		xLine1Left = 135;
    		LCD_DrawLine(30, yLinesLeft, 30, yLinesLeft+21, LCD_COLOR_DARK_GREEN);
    	}
    	else
    	{
    		xLine1Left-=5;
        	LCD_DrawLine(xLine1Left+5, yLinesLeft, xLine1Left+5, yLinesLeft+21, LCD_COLOR_DARK_GREEN);
        	LCD_DrawLine(xLine1Left, yLinesLeft, xLine1Left, yLinesLeft+21, LCD_COLOR_LINE_GREEN);
    	}

    	if(xLine2Left <= 30 )
    	{
    		xLine2Left = 135;
    		LCD_DrawLine(30, yLinesLeft, 30, yLinesLeft+21, LCD_COLOR_DARK_GREEN);
    	}
    	else
    	{
    		xLine2Left-=5;
        	LCD_DrawLine(xLine2Left+5, yLinesLeft, xLine2Left+5, yLinesLeft+21, LCD_COLOR_DARK_GREEN);
        	LCD_DrawLine(xLine2Left, yLinesLeft, xLine2Left, yLinesLeft+21, LCD_COLOR_LINE_GREEN);
    	}

    	if(xLine3Left <= 30 )
    	{
    		xLine3Left = 135;
    		LCD_DrawLine(30, yLinesLeft, 30, yLinesLeft+21, LCD_COLOR_DARK_GREEN);
    	}
    	else
    	{
    		xLine3Left-=5;
        	LCD_DrawLine(xLine3Left+5, yLinesLeft, xLine3Left+5, yLinesLeft+21, LCD_COLOR_DARK_GREEN);
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
    	if(xLine1Right >= 290 )
    	{
    		xLine1Right = 185;
    		LCD_DrawLine(290, yLinesRight, 290, yLinesRight+21, LCD_COLOR_DARK_GREEN);
    	}
    	else
    	{
    		xLine1Right+=5;
        	LCD_DrawLine(xLine1Right-5, yLinesRight, xLine1Right-5, yLinesRight+21, LCD_COLOR_DARK_GREEN);
        	LCD_DrawLine(xLine1Right, yLinesRight, xLine1Right, yLinesRight+21, LCD_COLOR_LINE_GREEN);
    	}

    	if(xLine2Right >= 290 )
    	{
    		xLine2Right = 185;
    		LCD_DrawLine(290, yLinesRight, 290, yLinesRight+21, LCD_COLOR_DARK_GREEN);
    	}
    	else
    	{
    		xLine2Right+=5;
        	LCD_DrawLine(xLine2Right-5, yLinesRight, xLine2Right-5, yLinesRight+21, LCD_COLOR_DARK_GREEN);
        	LCD_DrawLine(xLine2Right, yLinesRight, xLine2Right, yLinesRight+21, LCD_COLOR_LINE_GREEN);
    	}

    	if(xLine3Right >= 290 )
    	{
    		xLine3Right = 185;
    		LCD_DrawLine(290, yLinesRight, 290, yLinesRight+21, LCD_COLOR_DARK_GREEN);
    	}
    	else
    	{
    		xLine3Right+=5;
        	LCD_DrawLine(xLine3Right-5, yLinesRight, xLine3Right-5, yLinesRight+21, LCD_COLOR_DARK_GREEN);
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

	if(yLine1Center >= 198 )
	{
		yLine1Center = 93;
		LCD_DrawLine(xLinesCenter, 198, xLinesCenter+21, 198, LCD_COLOR_DARK_GREEN);
	}
	else
	{
		yLine1Center += 5;
		LCD_DrawLine(xLinesCenter, yLine1Center-5, xLinesCenter+21, yLine1Center-5, LCD_COLOR_DARK_GREEN);
		LCD_DrawLine(xLinesCenter, yLine1Center, xLinesCenter+21, yLine1Center, LCD_COLOR_LINE_GREEN);
	}

	if(yLine2Center >= 198 )
	{
		yLine2Center = 93;
		LCD_DrawLine(xLinesCenter, 198, xLinesCenter+21, 198, LCD_COLOR_DARK_GREEN);
	}
	else
	{
		yLine2Center += 5;
		LCD_DrawLine(xLinesCenter, yLine2Center-5, xLinesCenter+21, yLine2Center-5, LCD_COLOR_DARK_GREEN);
		LCD_DrawLine(xLinesCenter, yLine2Center, xLinesCenter+21, yLine2Center, LCD_COLOR_LINE_GREEN);
	}

	if(yLine3Center >= 198 )
	{
		yLine3Center = 93;
		LCD_DrawLine(xLinesCenter, 198, xLinesCenter+21, 198, LCD_COLOR_DARK_GREEN);
	}
	else
	{
		yLine3Center += 5;
		LCD_DrawLine(xLinesCenter, yLine3Center-5, xLinesCenter+21, yLine3Center-5, LCD_COLOR_DARK_GREEN);
		LCD_DrawLine(xLinesCenter, yLine3Center, xLinesCenter+21, yLine3Center, LCD_COLOR_LINE_GREEN);
	}

}

/* ****************************************************************************/
/* End      :  updateConveyorCenter											  */
/* ****************************************************************************/
