/**
 * \file    FliessbandECTS.h
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
#ifndef FLIESSBANDECTS_H_
#define FLIESSBANDECTS_H_

/* exported typedef -----------------------------------------------------------*/
typedef struct {
	unsigned int x;
	unsigned int y;
	unsigned int z; //Evtl. enum
	enum {Schwarz, Grau} Farbe;
} ECTS;

/* exported define ------------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define FLIESSBANDECTS_TASK_NAME       "Fliessband-ECTS"
#define FLIESSBANDECTS_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define FLIESSBANDECTS_TASK_PRIORITY   (configMAX_PRIORITIES - 4UL) /* priority 4*/

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/

/* exported function prototypes -----------------------------------------------*/
extern void initFliessbandECTSTasks(void);

#endif /* LAUFBANDECTS_H_ */

/**
 * @}
 */
