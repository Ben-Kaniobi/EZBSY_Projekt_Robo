#ifndef BTN_H_
#define BTN_H_
/******************************************************************************/
/** \file       btn.h
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
 ******************************************************************************/
/*
 *  function    btn_initInterrupt
 *              EXTI0_IRQHandler
 *              EXTI9_5_IRQHandler
 *              EXTI15_10_IRQHandler
 *
 ******************************************************************************/

//----- Header-Files -----------------------------------------------------------

//----- Macros -----------------------------------------------------------------

//----- Data types -------------------------------------------------------------

//----- Function prototypes ----------------------------------------------------
extern void btn_initInterrupt(void);
extern void EXTI0_IRQHandler(void);
extern void EXTI9_5_IRQHandler(void);
extern void EXTI15_10_IRQHandler(void);

//----- Data -------------------------------------------------------------------

#endif /* BTN_H_ */
