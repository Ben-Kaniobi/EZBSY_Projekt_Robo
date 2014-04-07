/******************************************************************************/
/*! \file UART_task.c
******************************************************************************
* \brief Set the string to send in a queue and write it over the UART interface
*
* Function : Manages the UART communication
*
* Procedures : 	createUARTMessage(void*)
* 				InitUARTTask()
*
* \author heimg1
*
* \version 0.0.1
*
* \history 07.04.2014 File Created
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
#include "uart.h"				/*UART imports*/
#include <stdio.h>              /* Standard Input/Output*/

/* application */
#include "UART_task.h"		/* Own header include */


/* ------------------------- module data declaration -------------------------*/

static xQueueHandle qUARTTx;

/* ----------------------- module procedure declaration ----------------------*/

static void uart_init(void);
static void vUARTTx(void*);


/* ****************************************************************************/
/* End Header : UART_task.c													  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  InitUARTTask													  */
/******************************************************************************/
/*! \brief Init the UART HW and data
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
* \bug Description of the bug
*
*******************************************************************************/

void InitUARTTask(void)
{
	/* UART HW init's */
	uart_init();

    /* create the queue */
    qUARTTx = xQueueCreate(UART_QUEUE_LENGTH,sizeof(char)*UART_MSG_LENGTH); /* TX-Message Queue */

    /* create tasks */
    xTaskCreate( vUARTTx, ( signed char * ) UART_TX_TASK_NAME, UART_STACK_SIZE, NULL, UART_TASK_PRIORITY, NULL );
}

/* ****************************************************************************/
/* End      :  InitUARTTask													  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  uart_init														  */
/******************************************************************************/
/*! \brief init the HW-layer
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
* \bug Description of the bug
*
*******************************************************************************/
static void uart_init(void) {

    USART_InitTypeDef USART_InitStruct;
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 9600;
    CARME_UART_Init(CARME_UART0, &USART_InitStruct);
}

/* ****************************************************************************/
/* End      :  uart_init													  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  createUARTMessage												  */
/******************************************************************************/
/*! \brief this function create a UART-message and send it to the message-queue
*
* \param[in]   string array of length UART_MSG_LENGTH
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
* \bug Description of the bug
*
*******************************************************************************/
void createUARTMessage(char data[UART_MSG_LENGTH])
{

    xQueueSendToBack(qUARTTx, data,0);
}
/* ****************************************************************************/
/* End      :  createUARTMessage											  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  vUARTTx														  */
/******************************************************************************/
/*! \brief Transmit task for UART-messages
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
* \bug Description of the bug
*
*******************************************************************************/
static void vUARTTx(void* pvParameters )
{
	char tx_Message[UART_MSG_LENGTH]; /* buffer for a received UART-message */
    portTickType tx_wait_time;

    /* We need to initialise tx_wait_time prior to the first call to vTaskDelayUntil(). */
    tx_wait_time = xTaskGetTickCount();

    /* for ever */
    for(;;)
    {
        /* if a new UART-Message wait for transmitting */
        if(xQueueReceive(qUARTTx,(void *)&tx_Message,portMAX_DELAY) == pdTRUE)
        {

            /* transmit the message */
        	printf("%s\n", &tx_Message);

            vTaskDelayUntil( &tx_wait_time, 10 / portTICK_RATE_MS); /* wait for 10ms */
        }
    }
}
/* ****************************************************************************/
/* End      :  vUARTTx														  */
/* ****************************************************************************/
