/**
 * \file    ECTS_updater_task.h
 * \author  kasen1
 * \date    2014-03-12
 *
 * \version 1.0
 *  import from template (2014-03-12)
 *
 * \brief   tasks to handle the conveyor and ECTS updater
 *
 * @{
 */

/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef ECTS_UPDATER_H_
#define ECTS_UPDATER_H_

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
/* Conveyors */
#define CONVEYOR_L (0)
#define CONVEYOR_C (1)
#define CONVEYOR_R (2)

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/
ects ECTS_1;
ects ECTS_2;
ects ECTS_3;
conveyorState conveyor_L_state;
conveyorState conveyor_C_state;
conveyorState conveyor_R_state;

/* exported function prototypes -----------------------------------------------*/
extern void init_ECTS_updater_tasks(void);
void CAN_conveyor_status_response(uint8_t conveyor, uint8_t DB[]);

#endif /* ECTS_UPDATER_H_ */

/**
 * @}
 */
