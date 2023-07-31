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
#define BUFFER_SIZE 512U
#define NUMBER_OF_SENSORS 4U
#define HALF_CONV_BUFF_SIZE 8U
#define CONVERT_TO_PERCENT  100U
#define CONVERT_MPa_to_PSI  145U
#define PRESSURE_SENSOR_MAX_VALUE 6U
#define FRONT_SENSOR_TRAVEL 300U
#define REAR_SENSOR_TRAVEL 125U

#define FRONT_SUSPENSION_TRAVEL 300U
#define REAR_SUSPENSION_TRAVEL 125U
/**
 * @brief Timers for measurement and trigger ADC
 *
 */
#define TRAVEL_SENSOR_ADC_CHANNEL &hadc2
#define TRAVEL_SENSOR_ADC_TRIGER_TIMER &htim2
#define TRAVEL_SENSOR_TIMER_SAMPLE_CHECK &htim6
/**
 * @brief System definition
 * @note if no system change to NO_FREE_RTOS
 *
 */
#define FREE_RTOS

/**
 * Debug define
 */

#define NO_CHECK_SAMPLE_TIME  /** To check time in SwV delete NO_ */
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
typedef struct calibration
{
	int16_t rearTravelSensor;
	int16_t frontTravelSensor;
	int16_t rearPressureSensor;
	int16_t frontPressureSensor;
} calibration_t;

static calibration_t calibrationValues =
{ 0 };
int16_t adcDataWrite[BUFFER_SIZE];
int16_t adcRearDataRead[BUFFER_SIZE / NUMBER_OF_SENSORS];
int16_t adcFrontDataRead[BUFFER_SIZE / NUMBER_OF_SENSORS];
int16_t adcRearPressureDataRead[BUFFER_SIZE / NUMBER_OF_SENSORS];
int16_t adcFrontPressureDataRead[BUFFER_SIZE / NUMBER_OF_SENSORS];

static volatile int16_t *inBufPtr;
static volatile int16_t *outRearBufPtr = &adcRearDataRead[0];
static volatile int16_t *outFrontBufPtr = &adcFrontDataRead[0];
static volatile int16_t *outRearPressureBufPtr = &adcRearPressureDataRead[0];
static volatile int16_t *outFrontPressureBufPtr = &adcFrontPressureDataRead[0];
#ifdef FREE_RTOS
extern osSemaphoreId travelSensorSemHandle;
#endif

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

/**
 * @brief sendDataSD - sending half ADC buffer to SD Card
 *
 * @param file - choose file where to store
 * @param sensor - choose sensor
 */
static void sendDataSD(char *file, volatile int16_t *sensor);
/******************************************************************************
 * Function Definitions
 *******************************************************************************/
void startAdcDma(void)
{
	osDelayUntil((uint32_t*) osKernelSysTick(), SD_INIT_TIME);
	HAL_ADC_Start_DMA(TRAVEL_SENSOR_ADC_CHANNEL, (uint32_t*) adcDataWrite,
	BUFFER_SIZE);
	HAL_TIM_Base_Start(TRAVEL_SENSOR_TIMER_SAMPLE_CHECK);
	HAL_TIM_Base_Start(TRAVEL_SENSOR_ADC_TRIGER_TIMER);
}
void stopAdcDma(void)
{
	HAL_ADC_Stop_DMA(TRAVEL_SENSOR_ADC_CHANNEL);
	HAL_TIM_Base_Stop(TRAVEL_SENSOR_TIMER_SAMPLE_CHECK);
	HAL_TIM_Base_Stop(TRAVEL_SENSOR_ADC_TRIGER_TIMER);
}
void travelPressureSensorCalibration(void)
{
#ifdef FREE_RTOS
	osSemaphoreWait(travelSensorSemHandle, osWaitForever);
#endif

	for (int i = 0; i < BUFFER_SIZE / HALF_CONV_BUFF_SIZE; i++)
	{
		calibrationValues.rearTravelSensor += outRearBufPtr[i];
		calibrationValues.frontTravelSensor += outFrontBufPtr[i];
		calibrationValues.rearPressureSensor += outRearPressureBufPtr[i];
		calibrationValues.frontPressureSensor += outFrontPressureBufPtr[i];

	}
	calibrationValues.rearTravelSensor = calibrationValues.rearTravelSensor
			/ (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
	calibrationValues.frontTravelSensor = calibrationValues.frontTravelSensor
			/ (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
	calibrationValues.rearPressureSensor = calibrationValues.rearPressureSensor
			/ (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
	calibrationValues.frontPressureSensor =
			calibrationValues.frontPressureSensor
					/ (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
}

void processData(char *sensorFront, char *sensorRear, char *sensorPressureFront,
		char *sensorPressureRear)
{
#ifdef FREE_RTOS
	osSemaphoreWait(travelSensorSemHandle, osWaitForever);
#endif
	sendDataSD(sensorRear, outRearBufPtr);
	sendDataSD(sensorFront, outFrontBufPtr);
	sendDataSD(sensorPressureRear, outRearPressureBufPtr);
	sendDataSD(sensorPressureFront, outFrontPressureBufPtr);
}

void processDataSag(int16_t *sagRearFront, int16_t *pressureRearFront)
{
#ifdef FREE_RTOS
	osSemaphoreWait(travelSensorSemHandle, osWaitForever);
#endif
	(uint32_t*) sagRearFront;
	for (int i = 0; i < BUFFER_SIZE / HALF_CONV_BUFF_SIZE; i++)
	{
		sagRearFront[0] += outRearBufPtr[i];
		sagRearFront[1] += outFrontBufPtr[i];
		pressureRearFront[0] += outRearPressureBufPtr[i];
		pressureRearFront[1] += outFrontPressureBufPtr[i];

	}
	sagRearFront[0] = sagRearFront[0] / (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
	sagRearFront[1] = sagRearFront[1] / (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
	pressureRearFront[0] = pressureRearFront[0]
			/ (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);
	pressureRearFront[1] = pressureRearFront[1]
			/ (BUFFER_SIZE / HALF_CONV_BUFF_SIZE);

	sagRearFront[0] = CONVERT_TO_PERCENT
			* (sagRearFront[0] / (float) REAR_SUSPENSION_TRAVEL);
	sagRearFront[1] = CONVERT_TO_PERCENT
			* (sagRearFront[1] / (float) FRONT_SUSPENSION_TRAVEL);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
	inBufPtr = &adcDataWrite[0];
	outRearPressureBufPtr = &adcRearPressureDataRead[0];
	outFrontPressureBufPtr = &adcFrontPressureDataRead[0];
	outRearBufPtr = &adcRearDataRead[0];
	outFrontBufPtr = &adcFrontDataRead[0];

	for (int n = 0; n < (BUFFER_SIZE / 2) - 1; n += NUMBER_OF_SENSORS)
	{
		outRearPressureBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToPressure(
				&inBufPtr[n],
				PRESSURE_SENSOR_MAX_VALUE)
				- calibrationValues.rearPressureSensor;
		outFrontPressureBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToPressure(
				&inBufPtr[n + 1],
				PRESSURE_SENSOR_MAX_VALUE)
				- calibrationValues.frontPressureSensor;
		outRearBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToTravel(
				&inBufPtr[n + 2],
				REAR_SENSOR_TRAVEL) - calibrationValues.rearTravelSensor;
		outFrontBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToTravel(
				&inBufPtr[n + 3],
				FRONT_SENSOR_TRAVEL) - calibrationValues.frontTravelSensor;
	}
#ifdef FREE_RTOS
	osSemaphoreRelease(travelSensorSemHandle);
#endif
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
#ifdef CHECK_SAMPLE_TIME
	previousTime = time_end;
	time_end = __HAL_TIM_GET_COUNTER(&htim6);
	sample_time = ((time_end - previousTime)/2)/(float)(BUFFER_SIZE/2);
	printf("semFullTime: %f\n", sample_time);
#endif

	inBufPtr = &adcDataWrite[BUFFER_SIZE / 2];
	outRearPressureBufPtr = &adcRearPressureDataRead[BUFFER_SIZE
			/ HALF_CONV_BUFF_SIZE];
	outFrontPressureBufPtr = &adcFrontPressureDataRead[BUFFER_SIZE
			/ HALF_CONV_BUFF_SIZE];
	outRearBufPtr = &adcRearDataRead[BUFFER_SIZE / HALF_CONV_BUFF_SIZE];
	outFrontBufPtr = &adcFrontDataRead[BUFFER_SIZE / HALF_CONV_BUFF_SIZE];

	for (int n = 0; n < (BUFFER_SIZE / 2) - 1; n += NUMBER_OF_SENSORS)
	{
		outRearPressureBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToPressure(
				&inBufPtr[n],
				PRESSURE_SENSOR_MAX_VALUE)
				- calibrationValues.rearPressureSensor;
		outFrontPressureBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToPressure(
				&inBufPtr[n + 1],
				PRESSURE_SENSOR_MAX_VALUE)
				- calibrationValues.frontPressureSensor;
		outRearBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToTravel(
				&inBufPtr[n + 2],
				REAR_SENSOR_TRAVEL) - calibrationValues.rearTravelSensor;
		outFrontBufPtr[n / NUMBER_OF_SENSORS] = convertAdcToTravel(
				&inBufPtr[n + 3],
				FRONT_SENSOR_TRAVEL) - calibrationValues.frontTravelSensor;
	}
#ifdef FREE_RTOS
	osSemaphoreRelease(travelSensorSemHandle);
#endif
}

static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite,
		int16_t sensorTravel)
{

	return (*adcDataWrite * sensorTravel / ADC_RESOLUTION);
}

static int16_t convertAdcToPressure(volatile int16_t *adcDataWrite,
		int16_t sensorMaxPressure)
{

	return ((CONVERT_MPa_to_PSI)
			* (*adcDataWrite * sensorMaxPressure / ((float) ADC_RESOLUTION)));
}

static void sendDataSD(char *file, volatile int16_t *sensor)
{
	char buffer[BUFFER_SIZE + 1];
	memset(buffer, 0, BUFFER_SIZE + 1);
	int i;
	for (i = 0; i < (BUFFER_SIZE) / HALF_CONV_BUFF_SIZE; i++)
	{
		sprintf(buffer + strlen(buffer), "%d ", sensor[i]);
	}
	size_t size = strlen(buffer) + 1;
	char newBuff[size];
	memset(newBuff, 0, size);
	strncpy(newBuff, buffer, sizeof(newBuff));
	newBuff[size] = '\0';
	Mount_SD("/");
	Update_File(file, newBuff);
	Unmount_SD("/");
}

/*************** END OF FUNCTIONS ***************************************************************************/
