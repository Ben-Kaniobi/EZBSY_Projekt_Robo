/******************************************************************************/
/** \file       btn.c
 *******************************************************************************
 *
 *  \brief      Initialize button T0-T3 interrupt and define External interrupt
 *              service routine for the corresponding lines.
 *              <p>
 *              T0 on GPIO C7
 *              T1 on GPIO B15
 *              T2 on GPIO B14
 *              T3 on GPIO I0
 *
 *  \author     wht4
 *
 *  \date       14.02.2014
 *
 *  \remark     Last Modification
 *               \li wht4, 14.02.2014, Created
 *               \li wht4, 12.03.2014, Added initialization/isr for T1-T3
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              btn_initInterrupt
 *              EXTI0_IRQHandler
 *              EXTI9_5_IRQHandler
 *              EXTI15_10_IRQHandler
 *  functions  local:
 *              .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include "stm32f4xx_exti.h"
#include "carme_io1.h"
#include "misc.h"
#include "can.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "btn.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    btn_initInterrupt
 ******************************************************************************/
/** \brief        Initialize Button T0, T1, T2 and T3 interrupt
 *                <p>
 *                T0 on GPIO C7
 *                T1 on GPIO B15
 *                T2 on GPIO B14
 *                T3 on GPIO I0
 *
 *  \type         global
 *
 *  \param[in]	  pvData    not used
 *
 *  \return       void
 *
 ******************************************************************************/
void btn_initInterrupt(void) {

    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Connect EXTI7  to C7 (Button0) */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);

    /* Configure EXTI7 line */
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set EXTI9_5 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    /* Connect EXTI15  to B15 (Button1) */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);
	/* Connect EXTI15  to B14 (Button2) */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);

	/* Configure EXTI15 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure EXTI14 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI15_10 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	 /* Connect EXTI0  to I0 (Button3) */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource0);

	/* Configure EXTI13 line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI0 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 *  function :    EXTI0_IRQHandler
 ******************************************************************************/
/** \brief        External interrupt on line 0
 *                <p> 
 *                Button T4 on line 0 (GPIO I0)
 *
 *  \type         global
 *
 *  \return       void
 *
 ******************************************************************************/
void EXTI0_IRQHandler(void) {

	static uint8_t u8Led = 0x00;

	/* Button T3 interrupt on Port I Pin 0 */
	EXTI_ClearITPendingBit(EXTI_Line0);

	u8Led ^= 0x08;
	CARME_IO1_LED_Set(u8Led, 0x08);
}

/*******************************************************************************
 *  function :    EXTI9_5_IRQHandler
 ******************************************************************************/
/** \brief        External interrupt on line 5 to 9
 *                <p> 
 *                Button T0 on line 7 (GPIO C7)
 *
 *  \type         global
 *
 *  \return       void
 *
 ******************************************************************************/
void MyEXTI9_5_IRQHandler(void) {

	static uint8_t u8Led = 0x00;

    /* Button T0 interrupt on Port C Pin 7 */
    if(EXTI_GetITStatus(EXTI_Line7) != RESET) {

        EXTI_ClearITPendingBit(EXTI_Line7);

    	u8Led ^= 0x01;
    	CARME_IO1_LED_Set(u8Led, 0x01);
    }
}


/*******************************************************************************
 *  function :    EXTI15_10_IRQHandler
 ******************************************************************************/
/** \brief        External interrupt on line 10 to 15
 *                <p> 
 *                Button T1 on line 15 (GPIO B15)
 *                Button T2 on line 14 (GPIO B14)
 *
 *  \type         global
 *
 *  \return       void
 *
 ******************************************************************************/
void EXTI15_10_IRQHandler(void) {

	static uint8_t u8Led = 0x00;

	/* Button T1 interrupt on Port B Pin 15 */
	if(EXTI_GetITStatus(EXTI_Line15) != RESET) {

		EXTI_ClearITPendingBit(EXTI_Line15);

    	u8Led ^= 0x02;
    	CARME_IO1_LED_Set(u8Led, 0x02);
	}
	/* Button T2 interrupt on Port B Pin 14 */
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) {

		EXTI_ClearITPendingBit(EXTI_Line14);

    	u8Led ^= 0x04;
    	CARME_IO1_LED_Set(u8Led, 0x04);
	}
}
