/*******************************************************************************
 * Title                 :   Suspension Telemetry
 * Filename              :   travelSensor.c
 * Author                :   patryk.grzywnowicz@gmail.com
 * Compiler              :   GCC
 * Target                :   data_acquisition_module
 *******************************************************************************/

/** \file travelSensor.c
 * \brief This module contains the ...
 */

/******************************************************************************
 * Includes
 *******************************************************************************/
#include "travelSensor.h"
#include "cmsis_os.h"
#include "stdint.h"
#include "string.h"
#include "adc.h"
#include "tim.h"
#include "File_Handling_RTOS.h"
/******************************************************************************
 * Module Preprocessor Constants
 *******************************************************************************/
/**
 * @brief travelSensor configuration def
 *
 */
#define SD_INIT_TIME 10

#define ADC_RESOLUTION 4096U

#define HALF_BUFF_SINGLE_SENSOR (2 * NUMBER_OF_SENSORS)

#define CONVERT_TO_PERCENT  100U
#define CONVERT_MPa_to_PSI  145U
#define PRESSURE_SENSOR_MAX_VALUE 6U
#define FRONT_SENSOR_TRAVEL 300U
#define REAR_SENSOR_TRAVEL 125U

#define FRONT_SUSPENSION_TRAVEL 300U
#define REAR_SUSPENSION_TRAVEL 125U
#define BRAKE_SENSOR_MAX_FORCE 100U
/**
 * @brief Timers for measurement and trigger ADC
 *
 */
#define TRAVEL_SENSOR_ADC_CHANNEL &hadc2
#define TRAVEL_SENSOR_ADC_TRIGER_TIMER &htim2
#define TRAVEL_SENSOR_TIMER_SAMPLE_CHECK &htim6

/**
 * Debug define
 */

#define CHECK_SAMPLE_TIME  /** To check time in SwV delete NO_ */
/******************************************************************************
 * Module Preprocessor Macros
 *******************************************************************************/

/******************************************************************************
 * Module Typedefs
 *******************************************************************************/

/******************************************************************************
 * Module Variable Definitions
 *******************************************************************************/
/**
 *@brief - store ADC data buffers
 */

calibration_t calibrationValues =
{ 0 };
volatile int16_t adcDataWrite[TRAVEL_SENSOR_BUFFER_SIZE];
volatile int16_t adcDataRead[TRAVEL_SENSOR_BUFFER_SIZE];

static volatile int16_t *inBufPtr;
static volatile int16_t *outBufPtr;

extern osSemaphoreId travelSensorSemHandle;

#ifdef CHECK_SAMPLE_TIME
volatile int time_start;
volatile int time_end;
volatile int previousTime = 0;
volatile float sample_time = 0;
#endif

/******************************************************************************
 * Function Prototypes
 *******************************************************************************/
/**
 * @brief convertAdcToTravel - convert a half ADC buffer to fork/shock travel
 *
 * @param adcDataWrite  - half ADC buffer
 * @param sensorTravel  - define constants(sensor travel)
 * @return actualTravel
 */
static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite,
		int16_t sensorTravel);

static int16_t convertAdcToPressure(volatile int16_t *adcDataWrite,
		int16_t sensorMaxPressure);

static int16_t convertAdcToBrakeForce(volatile int16_t *adcDataWrite,
		int16_t sensorBrakeMaxForce);
static void normalizeTravelData(volatile int16_t *inBufPtr);

/**
 * @brief sendDataSD - sending half ADC buffer to SD Card
 *
 * @param file - choose file where to store
 * @param sensor - choose sensor
 */
/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void startAdcDma(void)
{
	osDelayUntil((uint32_t*) osKernelSysTick(), SD_INIT_TIME);
	HAL_ADC_Start_DMA(TRAVEL_SENSOR_ADC_CHANNEL, (uint32_t*) adcDataWrite,
	TRAVEL_SENSOR_BUFFER_SIZE);
#ifdef CHECK_SAMPLE_TIME
	HAL_TIM_Base_Start(TRAVEL_SENSOR_TIMER_SAMPLE_CHECK);
#endif
	HAL_TIM_Base_Start(TRAVEL_SENSOR_ADC_TRIGER_TIMER);
}
void stopAdcDma(void)
{
	HAL_ADC_Stop_DMA(TRAVEL_SENSOR_ADC_CHANNEL);
#ifdef CHECK_SAMPLE_TIME
	HAL_TIM_Base_Stop(TRAVEL_SENSOR_TIMER_SAMPLE_CHECK);
#endif
	HAL_TIM_Base_Stop(TRAVEL_SENSOR_ADC_TRIGER_TIMER);
}
int travelPressureSensorCalibration(void)
{
	int status;

		memset(&calibrationValues, 0, sizeof(calibration_t));
		normalizeTravelData(inBufPtr);
		for (int i = 0; i <= TRAVEL_SENSOR_BUFFER_SIZE / 2 - NUMBER_OF_SENSORS;
				i += NUMBER_OF_SENSORS)
		{
			calibrationValues.rearTravelSensor += outBufPtr[i
					+ REAR_TRAVEL_BUFFER_POSITION];
			calibrationValues.frontTravelSensor += outBufPtr[i
					+ FRONT_TRAVEL_BUFFER_POSITION];
			calibrationValues.rearPressureSensor += outBufPtr[i
					+ REAR_PRESSURE_BUFFER_POSITION];
			calibrationValues.frontPressureSensor += outBufPtr[i
					+ FRONT_PRESSURE_BUFFER_POSITION];
			calibrationValues.leftBrakeSensor += outBufPtr[i
					+ LEFT_BRAKE_POSITION];
			calibrationValues.rightBrakeSensor += outBufPtr[i
					+ RIGHT_BRAKE_POSITION];

		}
		calibrationValues.rearTravelSensor = calibrationValues.rearTravelSensor
				/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.frontTravelSensor =
				calibrationValues.frontTravelSensor
						/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.rearPressureSensor =
				calibrationValues.rearPressureSensor
						/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.frontPressureSensor =
				calibrationValues.frontPressureSensor
						/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.leftBrakeSensor = calibrationValues.leftBrakeSensor
				/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.rightBrakeSensor = calibrationValues.rightBrakeSensor
				/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);

		status = writeCalibrationData(&calibrationValues);


	return (status);
}

void processData(char *sensorsData)
{
	normalizeTravelData(inBufPtr);
	sendDataSD(sensorsData, outBufPtr);
}

void processDataSag(int16_t *sagFrontRear, int16_t *pressureFrontRear)
{
	int numberOfElements = (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
	int i;
	normalizeTravelData(inBufPtr);
	sagFrontRear[0] = 0;
	sagFrontRear[1] = 0;
	pressureFrontRear[0] = 0;
	pressureFrontRear[1] = 0;
	for (i = 0; i <= TRAVEL_SENSOR_BUFFER_SIZE / 2 - NUMBER_OF_SENSORS; i +=
	NUMBER_OF_SENSORS)
	{
		sagFrontRear[0] += outBufPtr[i + FRONT_TRAVEL_BUFFER_POSITION];
		sagFrontRear[1] += outBufPtr[i + REAR_TRAVEL_BUFFER_POSITION];
		pressureFrontRear[0] += outBufPtr[i + FRONT_PRESSURE_BUFFER_POSITION];
		pressureFrontRear[1] += outBufPtr[i + REAR_PRESSURE_BUFFER_POSITION];

	}
	sagFrontRear[0] = sagFrontRear[0] / numberOfElements;
	sagFrontRear[1] = sagFrontRear[1] / numberOfElements;
	pressureFrontRear[0] = pressureFrontRear[0] / numberOfElements;
	pressureFrontRear[1] = pressureFrontRear[1] / numberOfElements;

	sagFrontRear[0] = ((int16_t)CONVERT_TO_PERCENT * sagFrontRear[0]) / (calibrationValues.frontTravel);
	sagFrontRear[1] = ((int16_t)CONVERT_TO_PERCENT * sagFrontRear[1]) / (calibrationValues.rearStroke);
	UNUSED(0);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	inBufPtr = &adcDataWrite[0];
	outBufPtr = &adcDataRead[0];
	osSemaphoreRelease(travelSensorSemHandle);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
#ifdef CHECK_SAMPLE_TIME
	previousTime = time_end;
	time_end = __HAL_TIM_GET_COUNTER(&htim6);
	sample_time = NUMBER_OF_SENSORS
			* ((time_end - previousTime)
					/ (2 * ((float) (TRAVEL_SENSOR_BUFFER_SIZE))));
	printf("semFullTime: %f\n", sample_time);
#endif

	inBufPtr = &adcDataWrite[TRAVEL_SENSOR_BUFFER_SIZE / 2];
	outBufPtr = &adcDataRead[TRAVEL_SENSOR_BUFFER_SIZE / 2];
	osSemaphoreRelease(travelSensorSemHandle);

}

static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite,
		int16_t sensorTravel)
{

	return (*adcDataWrite * sensorTravel /  (float)ADC_RESOLUTION);
}

static int16_t convertAdcToPressure(volatile int16_t *adcDataWrite,
		int16_t sensorMaxPressure)
{

	return ((2.5 * CONVERT_MPa_to_PSI)
			* (*adcDataWrite * sensorMaxPressure / (float)(ADC_RESOLUTION)));
}

static int16_t convertAdcToBrakeForce(volatile int16_t *adcDataWrite,
		int16_t sensorBrakeMaxForce)
{
	return (*adcDataWrite * sensorBrakeMaxForce /  (float)ADC_RESOLUTION);
}
static void normalizeTravelData(volatile int16_t *inBufPtr)
{
	for (int n = 0; n <= (TRAVEL_SENSOR_BUFFER_SIZE / 2) - NUMBER_OF_SENSORS;
			n += NUMBER_OF_SENSORS)
	{
		outBufPtr[n + FRONT_TRAVEL_BUFFER_POSITION] = convertAdcToTravel(
				&inBufPtr[n + FRONT_TRAVEL_BUFFER_POSITION],
				FRONT_SENSOR_TRAVEL) - calibrationValues.frontTravelSensor;
		outBufPtr[n + REAR_TRAVEL_BUFFER_POSITION] = convertAdcToTravel(
				&inBufPtr[n + REAR_TRAVEL_BUFFER_POSITION], REAR_SENSOR_TRAVEL)
				- calibrationValues.rearTravelSensor;
		outBufPtr[n + FRONT_PRESSURE_BUFFER_POSITION] = convertAdcToPressure(
				&inBufPtr[n + FRONT_PRESSURE_BUFFER_POSITION],
				PRESSURE_SENSOR_MAX_VALUE)
				- calibrationValues.frontPressureSensor;
		outBufPtr[n + REAR_PRESSURE_BUFFER_POSITION] = convertAdcToPressure(
				&inBufPtr[n + REAR_PRESSURE_BUFFER_POSITION],
				PRESSURE_SENSOR_MAX_VALUE)
				- calibrationValues.rearPressureSensor;
		outBufPtr[n + LEFT_BRAKE_POSITION] = convertAdcToBrakeForce(
				&inBufPtr[n + LEFT_BRAKE_POSITION],
				BRAKE_SENSOR_MAX_FORCE) - calibrationValues.leftBrakeSensor;
		outBufPtr[n + RIGHT_BRAKE_POSITION] = convertAdcToBrakeForce(
				&inBufPtr[n + RIGHT_BRAKE_POSITION],
				BRAKE_SENSOR_MAX_FORCE) - calibrationValues.rightBrakeSensor;

	}
}

/*************** END OF FUNCTIONS ***************************************************************************/
