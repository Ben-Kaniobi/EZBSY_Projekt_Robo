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
#include "FliessbandECTS.h"
#include "carme_io1.h"                  /* CARMEIO1 Board Support Package     */

#include <stdio.h>                      /* Standard Input/Output              */
#include <stdlib.h>                     /* General Utilities                  */
#include <string.h>                     /* String handling                    */

#include <FreeRTOS.h>                   /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <memPoolService.h>

//----- Macros -----------------------------------------------------------------
#define EVER ;;                         /* For forever loop: for(;;)          */
#define NO_MASK (0xFF)

//----- Data types -------------------------------------------------------------
xTaskHandle xFliessbandECTSHandle = NULL;

//----- Function prototypes ----------------------------------------------------
static void  vFliessbandECTSTask(void *pvData);

//----- Data -------------------------------------------------------------------

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
void  initFliessbandECTSTasks(void) {

	xTaskCreate(vFliessbandECTSTask, (signed char *) FLIESSBANDECTS_TASK_NAME, FLIESSBANDECTS_STACK_SIZE, NULL, FLIESSBANDECTS_TASK_PRIORITY, &xFliessbandECTSHandle);
}

/*******************************************************************************
 *  function :    AppTask1
 ******************************************************************************/
/** \brief        Increments a counter each cycle and displays it's value
 *                on the display
 *
 *  \type         local
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
static void vFliessbandECTSTask(void *pvData) {

	/* We need to initialise xLastFlashTime prior to the first call to vTaskDelayUntil() */
    portTickType xLastFlashTime;
	xLastFlashTime = xTaskGetTickCount();

	for(EVER) {

		/* Reset LEDs */
		CARME_IO1_LED_Set(0x00, NO_MASK);
		/* Delay until defined time passed */
		vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
		/* Set LEDs */
		CARME_IO1_LED_Set(0xFF, NO_MASK);
		/* Delay until defined time passed */
		vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
	}
}
