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
#include "lcd_task.h"
#include "CAN_gatekeeper_task.h"

#include  "inc/lcd.h"                          /* Graphic Library                */
#include <ff.h>

//----- Macros -----------------------------------------------------------------
//----- Data types -------------------------------------------------------------
//----- Function prototypes ----------------------------------------------------
//----- Data -------------------------------------------------------------------
uint8_t data[8] = {0x01,0x32,0xFF};	/*Example for CAN usage*/
//----- Implementation ---------------------------------------------------------

/*Example for CAN usage*/
void CANTest(CARME_CAN_MESSAGE *rx_message){
	uint8_t rxbuf[8];
	uint8_t i = 0;
	for(i = 0; i< rx_message->dlc; i++){
		rxbuf[i] = rx_message->data[i];
	}
	LCD_DisplayStringCenter(6, "Flipper");
}

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
	InitLCDTask();
	InitCANGatekeeperTask();

	/*Example for CAN usage*/
	setFunctionCANListener((CAN_function_listener_t)CANTest, 0x143);
	createCANMessage(0x142, 3, data);

    vTaskStartScheduler();

    /* code never reached */
    while (1)   {


    }
}
