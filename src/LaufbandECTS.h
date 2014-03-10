/**
 * \file    Rangefinder.h
 * \author  kasen1
 * \date    2014-01-14
 *
 * \version 1.0
 *  Softwared tested (14.01.2014)
 * \version 1.0
 *  import from template (14.01.2014)
 *
 * \brief   task for the rangefinder sensors
 *
 * \defgroup rangefinder Rangefinder
 * \brief   Rangefinder task
 * \ingroup hw_task
 *
 * @{
 */
/* Define to prevent recursive inclusion --------------------------------------*/
#ifndef LAUFBANDECTS_H_
#define LAUFBANDECTS_H_

/* exported typedef -----------------------------------------------------------*/
typedef struct {
	unsigned int x;
	unsigned int y;
	unsigned int z; //Evtl. enum
	enum {Schwarz, Grau} Farbe;
} ECTS;

/* exported define ------------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define LAUFBANDECTS_TASK_NAME       "Laufband-ECTS"
#define LAUFBANDECTS_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define LAUFBANDECTS_TASK_PRIORITY   (configMAX_PRIORITIES - 4UL) /* priority 4*/

/* exported macro -------------------------------------------------------------*/

/* exported variables ---------------------------------------------------------*/

/* exported function prototypes -----------------------------------------------*/
extern void initLaufbandECTSTask(void);

#endif /* LAUFBANDECTS_H_ */

/**
 * @}
 */
