/******************************************************************************/
/** \file       main.c
 *******************************************************************************
 *
 *  \brief      This is the freeRTOS base project for CARME-M4.
 *
 *  \author     wht4
 *
 *  \date       22.08.2011
 *
 *  \remark     Last Modification
 *               \li wht4, 22.08.2011, Created
 *               \li wht4, 16.01.2014, Adapted to CARME-M4
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              main
 *  functions  local:
 *              vAppTask1
 *              vAppTask2
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
/* RTOS */
#include <memPoolService.h>
/* Application */
#include "ECTS_updater_task.h"
#include  "inc/lcd.h"                           /* Graphic Library                */
#include <ff.h>

//----- Macros -----------------------------------------------------------------
//----- Data types -------------------------------------------------------------
//----- Function prototypes ----------------------------------------------------
//----- Data -------------------------------------------------------------------
FATFS main_fs;
//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    main
 ******************************************************************************/
/** \brief        Initialize GUI, BSP and OS
 *
 *  \type         global
 *
 *  \return       error code
 *
 ******************************************************************************/
int  main(void) {

    /* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* module initialisation */
	init_ECTS_updater_tasks();

	LCD_Init();

	LCD_SetFont(&font_8x13);
	if (f_mount(&main_fs, "0:", 1) != FR_OK) {
		//Default_Handler();
	}

    LCD_BMP_DrawBitmap("sw.bmp", 0, 0);

    vTaskStartScheduler();

    /* code never reached */
    while (1)   {

    }
}