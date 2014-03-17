/**
 * \file    ECTS_updater.h
 * \author  kasen1
 * \date    2014-03-12
 *
 * \version 1.0
 *  import from template by wht4 (2014-03-12)
 *
 * \brief   tasks to handle the conveyer and ECTS updater
 *
 * @{
 */

/* Includes -------------------------------------------------------------------*/
#include "ECTS_updater.h"
#include "carme_io1.h"                   /* CARMEIO1 Board Support Package     */

#include <stdio.h>                       /* Standard Input/Output              */
#include <stdlib.h>                      /* General Utilities                  */
#include <string.h>                      /* String handling                    */
#include <stdint.h>                      /* Integer typedefs                   */

#include <FreeRTOS.h>                    /* All freeRTOS headers               */
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <memPoolService.h>

/* private macro --------------------------------------------------------------*/
/* Common */
#define EVER        ;;                   /* For forever loop: for(;;)          */
/* CAN IDs */
#define GET_STATUS_L (0x110)             /* Status request for left conveyer   */
#define GET_STATUS_C (0x120)             /* Status request for center conveyer */
#define GET_STATUS_R (0x130)             /* Status request for right conveyer  */
#define STATUS_L     (0x111)             /* Status response for left conveyer  */
#define STATUS_C     (0x121)             /* Status response for center conveyer*/
#define STATUS_R     (0x131)             /* Status response for right conveyer */
#define CMD_L        (0x112)             /* Command for left conveyer          */
#define CMD_C        (0x122)             /* Command for center conveyer        */
#define CMD_R        (0x132)             /* Command for right conveyer         */
#define CMD_STATUS_L (0x113)             /* Command status for left conveyer   */
#define CMD_STATUS_C (0x123)             /* Command status for center conveyer */
#define CMD_STATUS_R (0x133)             /* Command status for right conveyer  */
#define RESET_L      (0x11F)             /* Reset for left conveyer            */
#define RESET_C      (0x12F)             /* Reset for center conveyer          */
#define RESET_R      (0x13F)             /* Reset for right conveyer           */
/* CAN messages */
#define ERROR        (0x00)
#define STATUS       (0x01)
#define START        (0x01)
#define STOP         (0x02)
#define STOP_AT      (0x03)
#define DONE         (0x04)



/* data types -----------------------------------------------------------------*/
xTaskHandle xECTS_updater_handle = NULL;

/* function prototypes --------------------------------------------------------*/
static void  vECTS_updater_task(void *pvData);

/* data -----------------------------------------------------------------------*/

/* implementation -------------------------------------------------------------*/

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
void  init_ECTS_updater_tasks(void) {

	xTaskCreate(vECTS_updater_task, (signed char *) ECTS_UPDATER_TASK_NAME, ECTS_UPDATER_STACK_SIZE, NULL, ECTS_UPDATER_TASK_PRIORITY, &xECTS_updater_handle);
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
static void vECTS_updater_task(void *pvData) {

	/* We need to initialise xLastFlashTime prior to the first call to vTaskDelayUntil() */
    portTickType xLastFlashTime;
	xLastFlashTime = xTaskGetTickCount();

	uint8_t CAN_msg;

	for(EVER) {

		//TODO: CAN_Status_Abfrage()




		/* Delay until defined time passed */
		vTaskDelayUntil( &xLastFlashTime, 200 / portTICK_RATE_MS);
	}
}
