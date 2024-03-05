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
#define NUMBER_OF_SENSORS 					6U
#define TRAVEL_SENSOR_BUFFER_SIZE 			(NUMBER_OF_SENSORS)*(32U)

#define FRONT_TRAVEL_BUFFER_POSITION 		0U
#define REAR_TRAVEL_BUFFER_POSITION 		1U
#define FRONT_PRESSURE_BUFFER_POSITION 		2U
#define REAR_PRESSURE_BUFFER_POSITION 		3U
#define LEFT_BRAKE_POSITION					4U
#define RIGHT_BRAKE_POSITION				5U
/******************************************************************************
 * Configuration Constants
 *******************************************************************************/

/******************************************************************************
 * Macros
 *******************************************************************************/

/******************************************************************************
 * Typedefs
 *******************************************************************************/
typedef struct calibration
{
	int16_t frontTravelSensor;
	int16_t rearTravelSensor;
	int16_t frontPressureSensor;
	int16_t rearPressureSensor;
	int16_t leftBrakeSensor;
	int16_t rightBrakeSensor;
	int16_t frontTravel;
	int16_t rearStroke;
} calibration_t;
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

void processDataSag(int16_t *sagFrontRear, int16_t *pressureFrontRear);
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
