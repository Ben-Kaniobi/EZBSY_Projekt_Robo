/******************************************************************************/
/** \file       pwm.c
 *******************************************************************************
 *
 *  \brief      Convenience functions to control/initialize pwm3 on carme
 *              io2.
 *              <p>
 *              This module can be used to control the speed of the embedded
 *              real-time module motor.
 *
 *  \author     wht4
 *
 *  \date       13.03.2014
 *
 ******************************************************************************/
/*
 *  functions  global:
 *              pwm_init
 *              pwm_setPWM3
 *  functions  local:
 *              .
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include "carme.h"
#include "carme_io1.h"                  /* CARMEIO1 Board Support Package     */
#include "carme_io2.h"                  /* CARMEIO1 Board Support Package     */

//----- Macros -----------------------------------------------------------------
#define PWM_HZ         ( 650000 )       /* PWM counter speed */
#define PWM_PERIOD     ( 100 )          /* PWM period length */

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------

//----- Data -------------------------------------------------------------------

//----- Implementation ---------------------------------------------------------

/*******************************************************************************
 *  function :    pwm_init
 ******************************************************************************/
void pwm_init(void) {

	uint16_t                u16Prescaler;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

	/* Calculate clock prescaler */
    u16Prescaler = (uint16_t) (((SystemCoreClock) / 2) / PWM_HZ) - 1;

    /* Initialize pwm clock structure */
	TIM_TimeBaseStruct.TIM_Prescaler = u16Prescaler;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Period = PWM_PERIOD - 1;
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	CARME_IO2_PWM_Settings(&TIM_TimeBaseStruct);

	/* Set start output value of pwm to zero */
	CARME_IO2_PWM_Set(CARME_IO2_PWM0, 0);
	CARME_IO2_PWM_Set(CARME_IO2_PWM1, 0);
	CARME_IO2_PWM_Set(CARME_IO2_PWM2, 0);
	CARME_IO2_PWM_Set(CARME_IO2_PWM3, 0);

	/* Set direction of pwm3 to normal */
	CARME_IO2_PWM_Phase(CARME_IO2_PWM_NORMAL_DIRECTION);

	/* Redefine PWM0 to PWM3 as normal gpio's. Just pwm3 is defined as pwm */
	CARME_IO2_GPIO_Out_Settings(CARME_IO2_GPIO_OUT_PIN0, CARME_IO2_GPIO_OUT_MODE_GPIO);
	CARME_IO2_GPIO_Out_Settings(CARME_IO2_GPIO_OUT_PIN1, CARME_IO2_GPIO_OUT_MODE_GPIO);
	CARME_IO2_GPIO_Out_Settings(CARME_IO2_GPIO_OUT_PIN2, CARME_IO2_GPIO_OUT_MODE_GPIO);
}

/*******************************************************************************
 *  function :    pwm_setPWM3
 ******************************************************************************/
ERROR_CODES pwm_setPWM3(uint16_t value) {

	/* Limit value to max period */
	if(value > (PWM_PERIOD - 1)) {
		value = PWM_PERIOD - 1;
	}
	return(CARME_IO2_PWM_Set(CARME_IO2_PWM3, value));
}




