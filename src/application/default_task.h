#ifndef DEFAULT_TASK_H_
#define DEFAULT_TASK_H_
/******************************************************************************/
/*! \file default_task.h
******************************************************************************
* \brief Short description of the files function
*
* Function : More detailed description of the files function
*
* Procedures : InitDefaultTask()
*
* \author Name
*
* \version 0.0.1
*
* \history dd.mm.jjjj File Created
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

/* default-task RTOS configuration */
#define DEFAULT_TASK_NAME		"Default Task"					/*!< Name of the task */
#define DEFAULT_STACK_SIZE		configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define DEFAULT_TASK_PRIORITY	(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */


/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitDefaultTask(void);

/* ****************************************************************************/
/* End Header : default_task.h												  */
/* ****************************************************************************/
#endif /* DEFAULT_TASK_H_ */
