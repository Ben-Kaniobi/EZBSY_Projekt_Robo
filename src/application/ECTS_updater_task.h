/******************************************************************************/
/*! \file ECTS_updater.h
******************************************************************************
* \brief Task to update the position of the ECTS structs
*
* Procedures : 	init_ECTS_updater_task()
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
#ifndef __APP_ECTS_UPDATER_TASK_H_
#define __APP_ECTS_UPDATER_TASK_H_

/* Includes -------------------------------------------------------------------*/
#include <stdint.h>                      /* Integer typedefs                   */

/* exported typedef -----------------------------------------------------------*/
/* ECTS */
typedef struct {
	uint16_t x;
	uint8_t y;
	enum {conveyor_L = 1, conveyor_C, conveyor_R, robo_L, robo_R} z;
} ects;
/* Conveyor */
typedef enum {STOPPED = 1, RUNNING = 2} conveyorState;

/* exported define ------------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define ECTS_UPDATER_TASK_NAME       "ECTS-updater task"
#define ECTS_UPDATER_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define ECTS_UPDATER_TASK_PRIORITY   (configMAX_PRIORITIES - 4UL) /* priority 4*/

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/
extern ects ECTS_1;
extern ects ECTS_2;
extern ects ECTS_3;
extern conveyorState conveyor_L_state;
extern conveyorState conveyor_C_state;
extern conveyorState conveyor_R_state;

/* exported function prototypes -----------------------------------------------*/
extern void init_ECTS_updater_task(void);

#endif /* __APP_ECTS_UPDATER_TASK_H_ */

/**
 * @}
 */
