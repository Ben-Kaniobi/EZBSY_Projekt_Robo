#ifndef __APP_CAN_GATEKEEPER_TASK_H_
#define __APP_CAN_GATEKEEPER_TASK_H_
/******************************************************************************/
/*! \file CAN_gatekeeper_task.h
******************************************************************************
* \brief header for the CAN gatekeeper
*
* Procedures : 	InitCANGatekeeperTask()
* 				createCANMessage(uint16_t, uint8_t, uint8_t *);
* 				setFunctionCANListener(CAN_function_listener_t, uint16_t);
*
* \author heimg1
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
/* EZBSY Project Robo														  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/
#include "can.h"				/*CAN imports*/

/* ----------------------- module constant declaration -----------------------*/

#define CAN_TX_TASK_NAME            "CAN Tx"
#define CAN_RX_TASK_NAME            "CAN Rx"
#define CAN_QUEUE_LENGTH            20 /*!< Size of the message queues */
#define CAN_STACK_SIZE              configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define CAN_TASK_PRIORITY           (configMAX_PRIORITIES - 1UL) /*!< priority (6) of the receive and transmit task */
#define CAN_LISTENER_BUFFER_SIZE    20 /*!< adjust to the needed size */
#define CAN_TX_MAX_WAIT_TIME        5 /*!< max waiting for empty tx-mailbox (* 10 ms) */


/* ------------------------- module type declaration -------------------------*/

/*function pointer for CAN-listeners*/
typedef void (*CAN_function_listener_t) (CARME_CAN_MESSAGE*);


/* struct for listener-database*/
typedef struct
{
    uint16_t id; /*!< id of the CAN-message */
    CAN_function_listener_t function; /*!< id to a callback function */
}CAN_listener_t;

/* ------------------------- module data declaration -------------------------*/


/* ----------------------- module procedure declaration ----------------------*/

extern void InitCANGatekeeperTask(void);
extern void createCANMessage(uint16_t, uint8_t, uint8_t *);
extern void setFunctionCANListener(CAN_function_listener_t, uint16_t);

/* ****************************************************************************/
/* End Header : CAN_gatekeeper_task.h										  */
/* ****************************************************************************/
#endif /* __APP_CAN_GATEKEEPER_TASK_H_ */
