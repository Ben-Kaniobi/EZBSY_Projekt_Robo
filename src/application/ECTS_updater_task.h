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
typedef struct {
	uint16_t x;
	uint8_t y;
	enum {Laufband1, Laufband2, Laufband3,
		Robo1, Ablage11, Ablage12, Ablage13,
		Robo2, Ablage21, Ablage22, Ablage23} z;
	enum {Schwarz, Grau} Farbe;
} ECTS;

/* exported define ------------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define ECTS_UPDATER_TASK_NAME       "Fliessband-ECTS"
#define ECTS_UPDATER_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define ECTS_UPDATER_TASK_PRIORITY   (configMAX_PRIORITIES - 4UL) /* priority 4*/

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/

/* exported function prototypes -----------------------------------------------*/
extern void init_ECTS_updater_tasks(void);

#endif /* ECTS_UPDATER_H_ */

/**
 * @}
 */
