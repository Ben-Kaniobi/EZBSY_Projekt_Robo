#ifndef PWM_H_
#define PWM_H_
/******************************************************************************/
/** \file       pwm.h
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
 ******************************************************************************/
/*
 *  function    pwm_init
 *              pwm_setPWM3
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------
#include "carme.h"

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern void        pwm_init(void);
extern ERROR_CODES pwm_setPWM3(uint16_t value);

//----- Data -------------------------------------------------------------------


#endif /* PWM_H_ */
