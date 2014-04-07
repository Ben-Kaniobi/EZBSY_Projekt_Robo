#ifndef __APP_UART_TASK_H_
#define __APP_UART_TASK_H_
/******************************************************************************/
/*! \file UART_task.h
******************************************************************************
* \brief Short description of the files function
*
* Function : More detailed description of the files function
*
* Procedures : InitUARTTask()
*
* \author heimg1
*
* \version 0.0.1
*
* \history 07.04.2014 File Created
*
*/
/* ****************************************************************************/
/* Project name																  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/

/* ----------------------- module constant declaration -----------------------*/

#define UART_TX_TASK_NAME            "UART Tx"
#define UART_MSG_NUMBER				 10 /*!Number of UART messages*/
#define UART_MSG_LENGTH				 20 /*!Length of UART messages*/
#define UART_QUEUE_LENGTH            20 /*!< Size of the message queues */
#define UART_STACK_SIZE              configMINIMAL_STACK_SIZE /*!< size of the receive and transmit task */
#define UART_TASK_PRIORITY           (configMAX_PRIORITIES - 1UL) /*!< priority (6) of the receive and transmit task */

/* ------------------------- module type declaration -------------------------*/

/* ------------------------- module data declaration -------------------------*/

/* ----------------------- module procedure declaration ----------------------*/

extern void InitUARTTask(void);
extern uint8_t createUARTMessage(char *);
/* ****************************************************************************/
/* End Header : UART_task.h													  */
/* ****************************************************************************/
#endif /* __APP_UART_TASK_H_ */
