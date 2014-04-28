/******************************************************************************/
/*! \file flipper_task.h
******************************************************************************
* \brief Task to handle the flipper and conveyor
*
* Procedures : 	InitFlipperTask()
*
* \author kasen1
*
* \version 0.0.1
*
* \history 12.03.2014 Import from template by wht4
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* Robo model                                                                 */
/* ****************************************************************************/

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef __APP_FLIPPER_TASK_H_
#define __APP_FLIPPER_TASK_H_

/* --------------------------------- imports ---------------------------------*/


/* ----------------------- module constant declaration -----------------------*/

/* RTOS configuration */
#define FLIPPER_TASK_NAME			"Flipper task"
#define FLIPPER_TASK_STACK_SIZE	configMINIMAL_STACK_SIZE 		/*!< size of the task's stack */
#define FLIPPER_TASK_PRIORITY		(configMAX_PRIORITIES - 1UL) 	/*!< priority of the task */

/* CAN IDs */
#define CMD_FLIPPER  (0x142)            /* CAN id for flipper command         */
#define CMD_C        (0x122)            /* CAN id for center conveyor command */
#define RESET_C      (0x12F)            /* Reset for center conveyer          */
/* CAN messages */
#define MSG_START    (0x01)             /* CAN message to start conveyor      */
#define MSG_DONE     (0x04)
#define MSG_FLIP     (0x01)             /* CAN message to move flipper        */
/* CAN DB values */
#define DB_SPEED     (0x50)             /* Speed for the flipper, 5-255, 0 for default */
#define DB_POS_L     (150)              /* Left flipper position              */
#define DB_POS_R     (105)              /* Right flipper position             */

/* ------------------------- module type declaration -------------------------*/


/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitFlipperTask(void);

/* ****************************************************************************/
/* End Header : default_task.h												  */
/* ****************************************************************************/
#endif /* __APP_FLIPPER_TASK_H_ */
