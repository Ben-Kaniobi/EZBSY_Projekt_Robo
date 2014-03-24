#ifndef __APP_CAN_GATEKEEPER_TASK_H_
#define __APP_CAN_GATEKEEPER_TASK_H_
/******************************************************************************/
/*! \file CAN_gatekeeper_task.h
******************************************************************************
* \brief Short description of the files function
*
* Function : More detailed description of the files function
*
* Procedures : InitCANGatekeeperTask()
*
* \author heimg1
*
* \version 0.0.1
*
* \history 24.03.2014 File Created
*
*
* \ingroup <group name> [<group name 2> <group name 3>]
*
* \todo If u have some todo's for the h-file, add it here
*
*/
/* ****************************************************************************/
/* Project name																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* CAN gatekeeper task RTOS configuration */
#define CAN_GATEKEEPER_TASK_NAME		"CAN gatekeeper Task"			/*!< Name of the task */
#define CAN_GATEKEEPER_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define CAN_GATEKEEPER_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitCANGatekeeperTask(void);

/* ****************************************************************************/
/* End Header : CAN_gatekeeper_task.h										  */
/* ****************************************************************************/
#endif /* __APP_CAN_GATEKEEPER_TASK_H_ */
