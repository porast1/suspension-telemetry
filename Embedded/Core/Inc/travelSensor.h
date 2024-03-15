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
#define TRAVEL_SENSOR_BUFFER_SIZE 			(NUMBER_OF_SENSORS)*(64U)

#define FRONT_TRAVEL_BUFFER_POSITION 		0U
#define REAR_TRAVEL_BUFFER_POSITION 		1U
#define FRONT_PRESSURE_BUFFER_POSITION 		2U
#define REAR_PRESSURE_BUFFER_POSITION 		3U
#define LEFT_BRAKE_POSITION					4U
#define RIGHT_BRAKE_POSITION				5U

/**
 * @brief additional sensors
 *
 */

#define NO_PRESSURE_SENSOR 2 //how many sensors are missing
#define NO_BRAKE_SENSOR    2


/**
 * Debug define
 */

#define NO_CHECK_SAMPLE_TIME  /** To check time in SwV delete NO_ */
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
	int32_t frontTravelSensor;
	int32_t rearTravelSensor;
	int32_t frontPressureSensor;
	int32_t rearPressureSensor;
	int32_t leftBrakeSensor;
	int32_t rightBrakeSensor;
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

void processDataSag(char* frontTravelBuf,char* rearTravelBuf, char* frontPressureBuf, char* rearPressureBuf);
void floatToStringTravel(char* buffer, float value, int decimalPlaces);
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
