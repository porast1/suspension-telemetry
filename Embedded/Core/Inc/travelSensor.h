/****************************************************************************
 * Title                 :   Suspension Telemetry
 * Filename              :   travelSensor.h
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
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
#include "ff.h"
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
void startAdcDma(void);
void stopAdcDma(void);
/**
 * @brief sending a finished data
 *
 * @param sensorFront - file for front sensor
 * @param sensorRear - file for front sensor
 */
void processData(char *sensorFront, char *sensorRear, char *sensorPressureFront,
		char *sensorPressureRear);

void processDataSag(int16_t *sagRearFront, int16_t *pressureRearFront);
void travelPressureSensorCalibration(void);
/**
 * @brief Half buffer data ready interrupt
 *
 * @param hadc
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc);
/**
 * @brief complete buffer data ready interrupt
 *
 * @param hadc
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
#endif /* INC_TRAVELSENSOR_H_ */

/*** End of File **************************************************************/
