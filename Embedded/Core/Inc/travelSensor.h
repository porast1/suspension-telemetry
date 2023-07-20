/****************************************************************************
* Title                 :   Automotive academy Nov 22   
* Filename              :   travelSensor.h
* Author                :   patry@globallogic.com
* Compiler              :   GCC
* Target                :   Car simulator
* Notes                 :   None
*****************************************************************************/

/** \file travelSensor.h
 *  \brief This module contains ...
 * 
 *  This is the header file for the definition for tasks that ...
 */
#ifndef INC_TRAVELSENSOR_H_
#define INC_TRAVELSENSOR_H_

/******************************************************************************
* Includes
*******************************************************************************/
#include "main.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
/**
 * This constant is
 */


/******************************************************************************
* Configuration Constants
*******************************************************************************/

/******************************************************************************
* Macros
*******************************************************************************/


/******************************************************************************
* Typedefs
*******************************************************************************/


/******************************************************************************
* Variables
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief init DMA and ADC
 *
 */
void startAdcDma (void);
/**
 * @brief sending a finished data
 *
 * @param sensorFront - file for front sensor
 * @param sensorRear - file for front sensor
 */
void processData(char *sensorFront, char *sensorRear);
/**
 * @brief Half buffer data ready interrupt
 *
 * @param hadc
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
/**
 * @brief complete buffer data ready interrupt
 *
 * @param hadc
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
#endif /* INC_TRAVELSENSOR_H_ */

/*** End of File **************************************************************/
