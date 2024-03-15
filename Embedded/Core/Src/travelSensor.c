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
volatile float adcDataRead[TRAVEL_SENSOR_BUFFER_SIZE];

static volatile int16_t *inBufPtr;
static volatile float *outBufPtr;

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
static float convertAdcToTravel(volatile int16_t adcDataWrite,
		int16_t sensorTravel);
#ifdef PRESSURE_SENSOR
static int16_t convertAdcToPressure(volatile int16_t adcDataWrite,
		int16_t sensorMaxPressure);
#endif

#ifdef BRAKE_SENSOR
static int16_t convertAdcToBrakeForce(volatile int16_t *adcDataWrite,
		int16_t sensorBrakeMaxForce);
#endif
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
		for (int i = 0; i <= TRAVEL_SENSOR_BUFFER_SIZE / 2 - NUMBER_OF_SENSORS;
				i += NUMBER_OF_SENSORS)
		{
			calibrationValues.frontTravelSensor += inBufPtr[i
								+ FRONT_TRAVEL_BUFFER_POSITION];
			calibrationValues.rearTravelSensor += inBufPtr[i
					+ REAR_TRAVEL_BUFFER_POSITION];
#ifdef PRESSURE_SENSOR
			calibrationValues.rearPressureSensor += inBufPtr[i
					+ REAR_PRESSURE_BUFFER_POSITION];
			calibrationValues.frontPressureSensor += inBufPtr[i
					+ FRONT_PRESSURE_BUFFER_POSITION];
#endif
#ifdef BRAKE_SENSOR
			calibrationValues.leftBrakeSensor += inBufPtr[i
					+ LEFT_BRAKE_POSITION];
			calibrationValues.rightBrakeSensor += inBufPtr[i
					+ RIGHT_BRAKE_POSITION];
#endif
		}
		calibrationValues.rearTravelSensor = calibrationValues.rearTravelSensor
				/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.frontTravelSensor =
				calibrationValues.frontTravelSensor
						/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
#ifdef PRESSURE_SENSOR
		calibrationValues.rearPressureSensor =
				calibrationValues.rearPressureSensor
						/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.frontPressureSensor =
				calibrationValues.frontPressureSensor
						/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
#endif
#ifdef BRAKE_SENSOR
		calibrationValues.leftBrakeSensor = calibrationValues.leftBrakeSensor
				/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
		calibrationValues.rightBrakeSensor = calibrationValues.rightBrakeSensor
				/ (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
#endif
		status = writeCalibrationData(&calibrationValues);


	return (status);
}

void processData(char *sensorsData)
{
	normalizeTravelData(inBufPtr);
	sendDataSD(sensorsData, outBufPtr);
}

void processDataSag(char* frontTravelBuf,char* rearTravelBuf, char* frontPressureBuf, char* rearPressureBuf)
{
	float frontTravel = 0;
	float rearTravel = 0;
	#ifdef PRESSURE_SENSOR
	int32_t frontPressure = 0;
	int32_t rearPressure = 0;
	#endif
	
	int numberOfElements = (TRAVEL_SENSOR_BUFFER_SIZE / HALF_BUFF_SINGLE_SENSOR);
	int i;
	for (i = 0; i <= TRAVEL_SENSOR_BUFFER_SIZE / 2 - NUMBER_OF_SENSORS; i +=
	NUMBER_OF_SENSORS)
	{
		frontTravel += inBufPtr[i + FRONT_TRAVEL_BUFFER_POSITION];
		rearTravel += inBufPtr[i + REAR_TRAVEL_BUFFER_POSITION];
		#ifdef PRESSURE_SENSOR
		frontPressure += inBufPtr[i + FRONT_PRESSURE_BUFFER_POSITION];
		rearPressure += inBufPtr[i + REAR_PRESSURE_BUFFER_POSITION];
		#endif

	}
	#ifdef PRESSURE_SENSOR
	frontPressure = (frontPressure / numberOfElements) - calibrationValues.frontPressureSensor;
	rearPressure = (rearPressure / numberOfElements) - calibrationValues.rearPressureSensor;
	sprintf(frontPressureBuf, "%ld", frontPressure);
	sprintf(rearPressureBuf, "%ld", rearPressure);
	#endif
	frontTravel = (frontTravel / numberOfElements) - calibrationValues.frontTravelSensor;
	rearTravel = (rearTravel / numberOfElements) - calibrationValues.rearTravelSensor;
	(frontTravel < 0) ? (frontTravel = 0) : UNUSED(0);
	(rearTravel < 0) ? (rearTravel = 0) : UNUSED(0);
	frontTravel = (CONVERT_TO_PERCENT * frontTravel) / (ADC_RESOLUTION - calibrationValues.frontTravelSensor);
	rearTravel = (CONVERT_TO_PERCENT * rearTravel) / (ADC_RESOLUTION - calibrationValues.rearTravelSensor);
	floatToStringTravel(frontTravelBuf, frontTravel, 1);
	floatToStringTravel(rearTravelBuf, rearTravel, 1);
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
void floatToStringTravel(char* buffer, float value, int decimalPlaces) {
	(value < 0) ? (value = 0) : UNUSED(0);
    int integerPart = (int)value;
    float fractionalPart = value - integerPart;
    int multiplier = 1;
    int i;

    for(i = 0; i < decimalPlaces; ++i)
        multiplier *= 10;

    fractionalPart *= multiplier;
    int fractional = (int)fractionalPart;

    sprintf(buffer, "%d.%d", integerPart, fractional);
}
static float convertAdcToTravel(volatile int16_t adcDataWrite,
		int16_t sensorTravel)
{

	return (adcDataWrite * sensorTravel /  (float)(ADC_RESOLUTION));
}
#ifdef PRESSURE_SENSOR
static int16_t convertAdcToPressure(volatile int16_t adcDataWrite,
		int16_t sensorMaxPressure)
{

	return ((2.5 * CONVERT_MPa_to_PSI)
			* (adcDataWrite * sensorMaxPressure / (float)(ADC_RESOLUTION)));
}
#endif
#ifdef BRAKE_SENSOR
static int16_t convertAdcToBrakeForce(volatile int16_t *adcDataWrite,
		int16_t sensorBrakeMaxForce)
{
	return (*adcDataWrite * sensorBrakeMaxForce /  (float)(ADC_RESOLUTION));
}
#endif
static void normalizeTravelData(volatile int16_t *inBufPtr)
{
	for (int n = 0; n <= (TRAVEL_SENSOR_BUFFER_SIZE / 2) - NUMBER_OF_SENSORS;
			n += NUMBER_OF_SENSORS)
	{
		outBufPtr[n + FRONT_TRAVEL_BUFFER_POSITION] =
				convertAdcToTravel((inBufPtr[n + FRONT_TRAVEL_BUFFER_POSITION] - calibrationValues.frontTravelSensor), FRONT_SENSOR_TRAVEL);
		outBufPtr[n + REAR_TRAVEL_BUFFER_POSITION] =
				convertAdcToTravel((inBufPtr[n + REAR_TRAVEL_BUFFER_POSITION] - calibrationValues.rearTravelSensor), REAR_SENSOR_TRAVEL);
#ifdef PRESSURE_SENSOR
		outBufPtr[n + FRONT_PRESSURE_BUFFER_POSITION] =
				convertAdcToPressure(inBufPtr[n + FRONT_PRESSURE_BUFFER_POSITION] - calibrationValues.frontPressureSensor, PRESSURE_SENSOR_MAX_VALUE);
		outBufPtr[n + REAR_PRESSURE_BUFFER_POSITION] =
				convertAdcToPressure(inBufPtr[n + REAR_PRESSURE_BUFFER_POSITION] - calibrationValues.rearPressureSensor, PRESSURE_SENSOR_MAX_VALUE);
#endif
#ifdef BRAKE_SENSOR
		outBufPtr[n + LEFT_BRAKE_POSITION] =
				inBufPtr[n + LEFT_BRAKE_POSITION] - calibrationValues.leftBrakeSensor;
		outBufPtr[n + RIGHT_BRAKE_POSITION] =
				inBufPtr[n + RIGHT_BRAKE_POSITION] - calibrationValues.rightBrakeSensor;
#endif

	}
}

/*************** END OF FUNCTIONS ***************************************************************************/
