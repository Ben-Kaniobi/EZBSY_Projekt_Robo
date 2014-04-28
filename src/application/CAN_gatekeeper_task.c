/******************************************************************************/
/*! \file CAN_gatekeeper_task.c
******************************************************************************
* \brief Manages the CAN communication
*
* Procedures : 	createCANMessage(uint16_t id, uint8_t dlc, uint8_t data[8])
* 				InitCANGatekeeperTask(void)
* 				setFunctionCANListener(CAN_function_listener_t, uint16_t)
* 				can_init(void)
* 				vCANRx(void*)
* 				vCANTx(void*)
* 				can_receivedMsgISR(void);
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

#include <memPoolService.h>		/* Memory pool manager service */
#include "stm32f4xx.h"			/* uC includes */
#include "can.h"				/*CAN imports*/

/* application */
#include "CAN_gatekeeper_task.h"		/* Own header include */


/* ------------------------- module data declaration -------------------------*/

static xQueueHandle qCANRx;
static xQueueHandle qCANTx;
static CAN_listener_t can_listener_buffer[CAN_LISTENER_BUFFER_SIZE];
static uint8_t current_buffer_size = 0;

/* ----------------------- module procedure declaration ----------------------*/

static void can_init(void);
static void vCANRx(void*);
static void vCANTx(void*);
static void can_receivedMsgISR(void);


/* ****************************************************************************/
/* End Header : CAN_gatekeeper_task.c										  */
/* ****************************************************************************/



/******************************************************************************/
/* Function:  InitCANGatekeeperTask											  */
/******************************************************************************/
/*! \brief Calls the HW init's, create the queues and create the CAN tasks
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/

void InitCANGatekeeperTask(void)
{
	/* CAN HW init's */
	can_init();

    /* create the queues */
    qCANRx = xQueueCreate(CAN_QUEUE_LENGTH,sizeof(CARME_CAN_MESSAGE)); /* RX-Message Queue */
    qCANTx = xQueueCreate(CAN_QUEUE_LENGTH,sizeof(CARME_CAN_MESSAGE)); /* TX-Message Queue */

    /* create the tasks */
    xTaskCreate( vCANRx, ( signed char * ) CAN_RX_TASK_NAME, CAN_STACK_SIZE, NULL, CAN_TASK_PRIORITY, NULL );
    xTaskCreate( vCANTx, ( signed char * ) CAN_TX_TASK_NAME, CAN_STACK_SIZE, NULL, CAN_TASK_PRIORITY, NULL );
}

/* ****************************************************************************/
/* End      :  InitCANGatekeeperTask										  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  can_init														  */
/******************************************************************************/
/*! \brief init the CAN hardware
*
* Function : CAN baud: 250k, Tx and Rx mode, register CAN IRQ
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/
static void can_init(void) {

	CARME_CAN_InitI(CARME_CAN_BAUD_250K,
			CARME_CAN_DF_NORMAL,
			CARME_CAN_INT_TX | CARME_CAN_INT_RX);

	CARME_CAN_RegisterIRQCallback(CARME_CAN_IRQID_RX_INTERRUPT, can_receivedMsgISR);
}

/* ****************************************************************************/
/* End      :  can_init														  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  setFunctionCANListener										  */
/******************************************************************************/
/*! \brief set a new listener to a CAN-message
*
* \param[in]   function    pointer to a function
* \param[in]   id          corresponding CAN-message ID
*
* \return None
*
* \author gross10,heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/
void setFunctionCANListener(CAN_function_listener_t function, uint16_t id)
{
    if(current_buffer_size < CAN_LISTENER_BUFFER_SIZE)
    {
        can_listener_buffer[current_buffer_size].id = id;
        can_listener_buffer[current_buffer_size].function = function;
        current_buffer_size++;
    }
}
/* ****************************************************************************/
/* End      :  setFunctionCANListener										  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  createCANMessage												  */
/******************************************************************************/
/*! \brief this function create a CAN-message and send it to the message-queue
*
* \param[in]   id Id of the CAN-message
* \param[in]   dlc Number of data-packet
* \param[in]   data Data-packet of the CAN-message
*
* \return None
*
* \author gross10,heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/
void createCANMessage(uint16_t id, uint8_t dlc, uint8_t data[8])
{
	CARME_CAN_MESSAGE tx_message;

    tx_message.id = id;
    tx_message.ext = 0;
    tx_message.rtr = CAN_RTR_Data;
    tx_message.dlc = dlc;

    /* write the data in to the CAN-message */
    while(dlc > 0 ){
        tx_message.data[dlc-1] = data[dlc-1];
        dlc--;
    }

    /*send the data to the queue*/
    xQueueSendToBack(qCANTx, &tx_message,0);
}
/* ****************************************************************************/
/* End      :  createCANMessage												  */
/* ****************************************************************************/


/******************************************************************************/
/* Function:  vCANRx														  */
/******************************************************************************/
/*! \brief Receive task for CAN-messages
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author gross10,heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/
static void vCANRx(void* pvParameters )
{
    /* Number of the function/queue entry */
    uint8_t i = 0;

    /* buffers for a received CAN-message */
    CARME_CAN_MESSAGE rx_message;

    /* for ever */
    for(;;)
    {
        /* if a new CAN-Message is received, call the rx-function or push the data to a queue */
        if(xQueueReceive(qCANRx,&rx_message,portMAX_DELAY) == pdTRUE)
        {
            for(i = 0; i < current_buffer_size; i++)
            {
                if(can_listener_buffer[i].id == rx_message.id)
                {
                    /* call the registered rx-function */
                    if(can_listener_buffer[i].function != 0)
                    {
                        (can_listener_buffer[i].function)(&rx_message);
                    }
                }
            }
        }
    }
}

/* ****************************************************************************/
/* End      :  vCANRx														  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  vCANTx														  */
/******************************************************************************/
/*! \brief Transmit task for CAN-messages
*
* \param[in] pvParameters  Pointer for given parameter
*
* \return None
*
* \author gross10,heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/
static void vCANTx(void* pvParameters )
{
	CARME_CAN_MESSAGE tx_Message; /* buffer for a received CAN-message */
    portTickType tx_wait_time;

    /* We need to initialise tx_wait_time prior to the first call to vTaskDelayUntil(). */
    tx_wait_time = xTaskGetTickCount();

    /* for ever */
    for(;;)
    {
        /* if a new CAN-Message wait for transmitting */
        if(xQueueReceive(qCANTx,(void *)&tx_Message,portMAX_DELAY) == pdTRUE)
        {

            /* transmit the message */
            CARME_CAN_Write(&tx_Message);

            /* wait for 10ms */
            vTaskDelayUntil( &tx_wait_time, 10 / portTICK_RATE_MS);

        }
    }
}
/* ****************************************************************************/
/* End      :  vCANTx														  */
/* ****************************************************************************/

/******************************************************************************/
/* Function:  can_receivedMsgISR											  */
/******************************************************************************/
/*! \brief called by the CAN-RX interrupt handler
*
* \param[in] None
*
* \return None
*
* \author heimg1
*
* \version 0.0.1
*
* \date 24.03.2014 File Created
*
*******************************************************************************/
static void can_receivedMsgISR(void) {

	CARME_CAN_MESSAGE RX_Message;

    /* receive CAN-message */
    CARME_CAN_Read(&RX_Message);

    /*Send received data into queue*/
    xQueueSendToBack(qCANRx,&RX_Message,0);
}

/* ****************************************************************************/
/* End      :  can_receivedMsgISR											  */
/* ****************************************************************************/
