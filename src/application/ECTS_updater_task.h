/******************************************************************************/
/*! \file ECTS_updater.h
******************************************************************************
* \brief Task to update the position of the ECTS structs
*
* Procedures : 	InitECTSUpdaterTask()
* 				find_ECTS()
*
* \author kasen1
*
* \version 0.1.0
*
* \history 28.04.2014 Version 0.1.0
* \history 12.03.2014 Import from template by wht4
*
*
* \ingroup application
*
*/
/* ****************************************************************************/
/* Robo model                                                                 */
/* ****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_ECTS_UPDATER_TASK_H_
#define __APP_ECTS_UPDATER_TASK_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>                      /* Integer typedefs                  */

/* exported typedef ----------------------------------------------------------*/
/*! \enum z_pos
* \brief Enum to define all the possible locations (conveyors, robos)
*/
typedef enum {conveyor_L = 1, conveyor_C, conveyor_R, robo_L, robo_R} z_pos;
/*! \struct ects
* \brief Struct to describe an ECTS with id, and position (x, y and location)
*/
typedef struct {
	uint8_t id;
	uint16_t x;
	uint16_t y;
	z_pos z;
} ects;
/*! \enum conveyorState
* \brief Enum to track the state of a conveyor
*/
typedef enum {STOPPED = 1, RUNNING = 2} conveyorState;

/* exported define -----------------------------------------------------------*/
/* Rangefinder-task RTOS configuration */
#define ECTS_UPDATER_TASK_NAME       "ECTS-updater task"
#define ECTS_UPDATER_STACK_SIZE      configMINIMAL_STACK_SIZE /* size of the receive and transmit task */
#define ECTS_UPDATER_TASK_PRIORITY   (configMAX_PRIORITIES - 4UL) /* priority 4*/

/* exported macro ------------------------------------------------------------*/

/* exported variables --------------------------------------------------------*/
extern ects ECTS_1;                     /*!< First ECTS in the system         */
extern ects ECTS_2;                     /*!< Second ECTS in the system        */
extern ects ECTS_3;                     /*!< Third ECTS in the system         */
extern conveyorState conveyor_L_state;  /*!< State of the left conveyor       */
extern conveyorState conveyor_C_state;  /*!< State of the centre conveyor     */
extern conveyorState conveyor_R_state;  /*!< State of the right conveyor      */

/* exported function prototypes ----------------------------------------------*/
extern void InitECTSUpdaterTask(void);
extern void find_ECTS(ects **ECTS_p, z_pos _z_pos);

/* ****************************************************************************/
/* End Header : ECTS_updater.h                                                */
/* ****************************************************************************/
#endif /* __APP_ECTS_UPDATER_TASK_H_ */

/**
 * @}
 */
