/******************************************************************************/
/*! \file CAN_gatekeeper_task.c
******************************************************************************
* \brief Short description of the files function
*
* Function : Manages the CAN communication
*
* Procedures : 	vCANGatekeeperTask(void*)
* 				InitCANGatekeeperTask()
*
* \author heimg1
*
* \version 0.0.1
*
* \history 24.03.2014 File Created
*
*
* \bug Description of the bug
*
*/
/* ****************************************************************************/
/* Project name																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/

#include <memPoolService.h>		/* Memory pool manager service */
#include "stm32f4xx.h"			/* uC includes */

/* application */
#include "CAN_gatekeeper_task.h"		/* Own header include */


/* ------------------------- module data declaration -------------------------*/

//int testVar = 0;	/*!< Detailed description after the member */

/* ----------------------- module procedure declaration ----------------------*/

static void vCANGatekeeperTask(void*);


/* ****************************************************************************/
/* End Header : CAN_gatekeeper_task.c										  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  InitCANGatekeeperTask											  */
/******************************************************************************/
/*! \brief Short description of the function
*
* Function : More detailed description of the function
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*
* \todo If u have some todo's for the function, add it here
*
* \bug Description of the bug
*
*******************************************************************************/

void InitCANGatekeeperTask(void)
{
	/* Do some inits here */


    /* create the task */
	xTaskCreate( vCANGatekeeperTask, ( signed char * ) CAN_GATEKEEPER_TASK_NAME,
			CAN_GATEKEEPER_STACK_SIZE, NULL, CAN_GATEKEEPER_TASK_PRIORITY,
			NULL);
}

/* ****************************************************************************/
/* End      :  InitCANGatekeeperTask										  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  vCANGatekeeperTask											  */
/******************************************************************************/
/*! \brief Short description of the function2
*
* Function : More detailed description of the function2
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*
* \todo If u have some todo's for the function, add it here
*
* \bug Description of the bug
*
*******************************************************************************/

static void vCANGatekeeperTask(void* pvParameters )
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
/* End      :  vCANGatekeeperTask											  */
/* ****************************************************************************/

