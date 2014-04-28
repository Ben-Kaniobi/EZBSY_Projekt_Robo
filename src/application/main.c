/******************************************************************************/
/** \file       main.c
 *******************************************************************************
 *
 *  \brief      Project to handle the Stellaris robot model
 *
 *  \author     plats1,meert1,kasen1,heimg1
 *
 *  \date       19.01.2014
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
#include "UI_task.h"
#include "CAN_gatekeeper_task.h"
#include "UART_task.h"
#include "ECTS_updater_task.h"
#include "flipper_task.h"
#include "conveyor_tasks.h"
#include "init_robot_tasks.h"
#include "btn.h"

#include  "inc/lcd.h"                          /* Graphic Library                */
#include <ff.h>

//----- Macros -----------------------------------------------------------------
//----- Data types -------------------------------------------------------------
//----- Function prototypes ----------------------------------------------------
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
int  main(void) {

    /* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* module initialisation */
	btn_initInterrupt();
	InitUITask();
	InitUARTTask();
	InitCANGatekeeperTask();
	InitECTSUpdaterTask();
	//InitFlipperTask();
	InitConveyorTasks();
	InitRobotTasks();

    vTaskStartScheduler();

    /* code never reached */
    while (1)   {


    }

    void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
    {
    	( void ) pcTaskName;
    	( void ) pxTask;

    	/* Run time stack overflow checking is performed if
    	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook
    	function is called if a stack overflow is detected. */
    	taskDISABLE_INTERRUPTS();
    	for( ;; );
    }
}
