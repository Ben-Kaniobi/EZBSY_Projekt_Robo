/******************************************************************************/
/*! \file UART_task.c
******************************************************************************
* \brief Set the string into a queue and write it over the UART interface
*
* Function : Manages the UART communication
*
* Procedures : 	uart_init()
* 				vUARTTx(void*)
* 				InitUARTTask()
* 				createUARTMessage(char data[])
*
* \author heimg1
*
* \version 0.0.1
*
* \history 07.04.2014 File Created
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* EZBSY Project Robo														  */
/* ****************************************************************************/

/* --------------------------------- imports ---------------------------------*/

#include <memPoolService.h>		/* Memory pool manager service */
#include "stm32f4xx.h"			/* uC includes */
#include "uart.h"				/*UART imports*/
#include <stdio.h>              /* Standard Input/Output*/
#include <string.h>				/* String handling*/

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
/*! \brief Init the UART HW, queue and create the task
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
*******************************************************************************/

void InitUARTTask(void)
{
	/* UART HW init's */
	uart_init();

    /* create the TX queue */
    qUARTTx = xQueueCreate(UART_QUEUE_LENGTH,sizeof(char)*UART_MSG_LENGTH);

    /* create tasks */
    xTaskCreate( vUARTTx, ( signed char * ) UART_TX_TASK_NAME, UART_STACK_SIZE, NULL, UART_TASK_PRIORITY, NULL );
}

/* ****************************************************************************/
/* End      :  InitUARTTask													  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  uart_init														  */
/******************************************************************************/
/*! \brief init the HW-layer: Baud 9600, Stop 1, Parity none
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
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
/*! \brief this function creates a UART-message and sends it to the message-queue
*
* \param[in]   string array of length UART_MSG_LENGTH
*
* \return -1 if the message is to long, 1 if the message was sent
*
* \author heimg1
*
* \version 0.0.1
*
* \date 07.04.2014 File Created
*
*******************************************************************************/
uint8_t createUARTMessage(char data[])
{
	/*check if the message is to long*/
	if(strlen(data) <= UART_MSG_LENGTH){

		/*send message to the queue*/
		xQueueSendToBack(qUARTTx, data,0);
		return 1;
	}
	else{

		return -1;
	}
}
/* ****************************************************************************/
/* End      :  createUARTMessage											  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  vUARTTx														  */
/******************************************************************************/
/*! \brief Transmit task for the UART-messages
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
*******************************************************************************/
static void vUARTTx(void* pvParameters )
{
	/* buffer for a received UART-message */
	char tx_Message[UART_MSG_LENGTH];
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

        	/* wait for 100ms */
            vTaskDelayUntil( &tx_wait_time, 100 / portTICK_RATE_MS);
        }
    }
}
/* ****************************************************************************/
/* End      :  vUARTTx														  */
/* ****************************************************************************/
