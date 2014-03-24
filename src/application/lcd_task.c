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

/* ----------------------- module procedure declaration ----------------------*/

static void vLCDTask(void*);


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
//    LCD_BMP_DrawBitmap("sw.bmp", 0, 0);

    /* Display welcome */
    LCD_SetTextColor(GUI_COLOR_BLUE);
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

        vTaskDelayUntil( &xLastFlashTime, 500 / portTICK_RATE_MS);
    }
}

/* ****************************************************************************/
/* End      :  vLCDTask														  */
/* ****************************************************************************/
