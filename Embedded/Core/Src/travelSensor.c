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
#define BUFFER_SIZE 256U
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
int16_t adcDataWrite[BUFFER_SIZE];
int16_t adcRearDataRead[BUFFER_SIZE/2];
int16_t adcFrontDataRead[BUFFER_SIZE/2];
static volatile int16_t *inBufPtr;
static volatile int16_t *outRearBufPtr = &adcRearDataRead[0];
static volatile int16_t *outFrontBufPtr = &adcFrontDataRead[0];

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
static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite, int16_t sensorTravel);
/**
 * @brief sendDataSD - sending half ADC buffer to SD Card
 *
 * @param file - choose file where to store
 * @param sensor - choose sensor
 */
static void  sendDataSD(char *file,volatile int16_t * sensor);
/******************************************************************************
* Function Definitions
*******************************************************************************/
void startAdcDma (void){
	osDelayUntil((uint32_t*)osKernelSysTick(), SD_INIT_TIME);
	HAL_ADC_Start_DMA(TRAVEL_SENSOR_ADC_CHANNEL, (uint32_t*)adcDataWrite, BUFFER_SIZE);
	HAL_TIM_Base_Start(TRAVEL_SENSOR_TIMER_SAMPLE_CHECK);
	HAL_TIM_Base_Start(TRAVEL_SENSOR_ADC_TRIGER_TIMER);
}
void stopAdcDma (void){
	HAL_ADC_Stop_DMA(TRAVEL_SENSOR_ADC_CHANNEL);
	HAL_TIM_Base_Stop(TRAVEL_SENSOR_TIMER_SAMPLE_CHECK);
	HAL_TIM_Base_Stop(TRAVEL_SENSOR_ADC_TRIGER_TIMER);
}

void processData(char *sensorFront, char *sensorRear){
#ifdef FREE_RTOS
	osSemaphoreWait(travelSensorSemHandle, osWaitForever);
#endif
	sendDataSD(sensorRear,outRearBufPtr);
	sendDataSD(sensorFront,outFrontBufPtr);
}
void processDataSag(uint16_t* sagRearFront){
#ifdef FREE_RTOS
	osSemaphoreWait(travelSensorSemHandle, osWaitForever);
#endif
	(uint32_t*)sagRearFront;
	for(int i = 0; i < BUFFER_SIZE/4; i++){
		sagRearFront[0] += outRearBufPtr[i];
		sagRearFront[1] += outFrontBufPtr[i];
	}
	sagRearFront[0] = sagRearFront[0]/(BUFFER_SIZE/4);
	sagRearFront[1] = sagRearFront[1]/(BUFFER_SIZE/4);

	sagRearFront[0] = 100*(sagRearFront[0]/(float)REAR_SUSPENSION_TRAVEL);
	sagRearFront[1] = 100*(sagRearFront[1]/(float)FRONT_SUSPENSION_TRAVEL);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	inBufPtr = &adcDataWrite[0];
	outRearBufPtr = &adcRearDataRead[0];
	outFrontBufPtr = &adcFrontDataRead[0];
		for (int n = 0; n < (BUFFER_SIZE/2) -1 ; n+=2){
		outRearBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n], REAR_SENSOR_TRAVEL);
		outFrontBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n + 1], FRONT_SENSOR_TRAVEL);
	}
#ifdef FREE_RTOS
	osSemaphoreRelease(travelSensorSemHandle);
#endif
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
#ifdef CHECK_SAMPLE_TIME
	previousTime = time_end;
	time_end = __HAL_TIM_GET_COUNTER(&htim6);
	sample_time = ((time_end - previousTime)/2)/(float)(BUFFER_SIZE/2);
	printf("semFullTime: %f\n", sample_time);
#endif
	inBufPtr = &adcDataWrite[BUFFER_SIZE/2];
	outRearBufPtr = &adcRearDataRead[BUFFER_SIZE/4];
	outFrontBufPtr = &adcFrontDataRead[BUFFER_SIZE/4];
	for (int n = 0; n < (BUFFER_SIZE/2) -1 ; n+=2){
		outRearBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n], REAR_SENSOR_TRAVEL);
		outFrontBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n + 1], FRONT_SENSOR_TRAVEL);
	}
#ifdef FREE_RTOS
	osSemaphoreRelease(travelSensorSemHandle);
#endif
}

static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite, int16_t sensorTravel){

	return (*adcDataWrite*sensorTravel/ADC_RESOLUTION);
}

static void sendDataSD(char *file,volatile int16_t * sensor){
	char buffer[BUFFER_SIZE+1];
	memset(buffer,0,BUFFER_SIZE+1);
	int i;
	for (i = 0; i < (BUFFER_SIZE)/4; i++){
		sprintf(buffer + strlen(buffer), "%d ", sensor[i]);
	}
	size_t size = strlen(buffer)+1;
	char newBuff[size];
	memset(newBuff,0,size);
	strncpy(newBuff, buffer,sizeof(newBuff));
	newBuff[size] = '\0';
	Mount_SD("/");
	Update_File(file, newBuff);
	Unmount_SD("/");
}

/*************** END OF FUNCTIONS ***************************************************************************/
