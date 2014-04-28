/******************************************************************************/
/*! \file main.c
******************************************************************************
* \brief Project to handle the Stellaris robot model
*
* Procedures : 	main()
*
* \author plats1,meert1,kasen1,heimg1
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

#include  "inc/lcd.h"
#include <ff.h>


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

/* ****************************************************************************/
/* End Header : main.c														  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  main															  */
/******************************************************************************/
/*! \brief Initialize GUI, BSP and OS
*
* \return None
*
* \author plats1,meert1,kasen1,heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 Function Created
*
*******************************************************************************/
int  main(void) {

    /* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* module initialisation */
	btn_initInterrupt();
	InitUITask();
	InitUARTTask();
	InitCANGatekeeperTask();
	InitECTSUpdaterTask();
	//Flipper handled by button for presentation
	//InitFlipperTask();
	InitConveyorTasks();
	InitRobotTasks();

    vTaskStartScheduler();

    /* code never reached */
    while (1)
    {
    }

}

/* ****************************************************************************/
/* End      :  main															  */
/* ****************************************************************************/
