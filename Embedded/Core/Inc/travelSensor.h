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
#define TRAVEL_SENSOR_BUFFER_SIZE 512U
#define FRONT_TRAVEL_BUFFER_POSITION 3U
#define REAR_TRAVEL_BUFFER_POSITION 2U
#define FRONT_PRESSURE_BUFFER_POSITION 1U
#define REAR_PRESSURE_BUFFER_POSITION 0
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
void processData(char *sensorsData);

void processDataSag(int32_t *sagFrontRear, int32_t *pressureFrontRear);
int travelPressureSensorCalibration(void);
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
