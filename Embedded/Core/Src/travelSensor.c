/*******************************************************************************
* Title                 :   Automotive academy Nov 22
* Filename              :   travelSensor.c
* Author                :   patryk.grzywnowicz@gmail.com
* Compiler              :   GCC
* Target                :   Car simulator
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
#include "stdio.h"
#include "string.h"
#include "adc.h"
#include "tim.h"
#include "File_Handling_RTOS.h"
#include "fatfs.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fatfs_sd.h"

/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/


#define ADC_RESOLUTION 4096U
#define FRONT_SENSOR_TRAVEL 300U
#define REAR_SENSOR_TRAVEL 125U


#define DATA_BUFF_READY  1U
#define DATA_BUFF_EMPTY	 0U

/**
 * Debug define
 */

#define CHECK_SAMPLE_TIME 0
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
int16_t adcDataWrite[BUFFER_SIZE];
int16_t adcRearDataRead[BUFFER_SIZE/2];
int16_t adcFrontDataRead[BUFFER_SIZE/2];
static volatile int16_t *inBufPtr;
static volatile int16_t *outRearBufPtr = &adcRearDataRead[0];
static volatile int16_t *outFrontBufPtr = &adcFrontDataRead[0];

volatile int time_start;
volatile int time_end;
volatile int previousTime = 0;
volatile float sample_time = 0;

extern osSemaphoreId travelSensorSemHandle;


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void startAdcDma (ADC_HandleTypeDef *hadc);
void processData(void);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite, int16_t sensorTravel);
static uint8_t  sendDataSD(volatile int16_t * frontSensor);
/******************************************************************************
* Function Definitions
*******************************************************************************/
void startAdcDma (ADC_HandleTypeDef *hadc){
	HAL_ADC_Start_DMA(hadc, (uint32_t*)adcDataWrite, BUFFER_SIZE);
	HAL_TIM_Base_Start(&htim6);
	HAL_TIM_Base_Start(&htim2);
}

void processData(void){
	osSemaphoreWait(travelSensorSemHandle, osWaitForever);
	sendDataSD(outFrontBufPtr);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	inBufPtr = &adcDataWrite[0];
	outRearBufPtr = &adcRearDataRead[0];
	outFrontBufPtr = &adcFrontDataRead[0];
		for (int n = 0; n < (BUFFER_SIZE/2) -1 ; n+=2){
		outRearBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n], REAR_SENSOR_TRAVEL);
		outFrontBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n + 1], FRONT_SENSOR_TRAVEL);
	}
	osSemaphoreRelease(travelSensorSemHandle);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
#if CHECK_SAMPLE_TIME == 1
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
	osSemaphoreRelease(travelSensorSemHandle);
}

static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite, int16_t sensorTravel){

	return (*adcDataWrite*sensorTravel/ADC_RESOLUTION);
}

static uint8_t sendDataSD(volatile int16_t * frontSensor){
	char buffer[BUFFER_SIZE+1];
	memset(buffer,0,BUFFER_SIZE+1);
	int i;
	for (i = 0; i < (BUFFER_SIZE)/4; i++){
		sprintf(buffer + strlen(buffer), "%d ", frontSensor[i]);
	}
	size_t size = strlen(buffer)+1;
	char newBuff[size];
	memset(newBuff,0,size);
	strncpy(newBuff, buffer,sizeof(newBuff));
	newBuff[size] = '\0';
	Mount_SD("/");
	Update_File("ADC_DATA.TXT", newBuff);
	Unmount_SD("/");
	return (1);
}

/*************** END OF FUNCTIONS ***************************************************************************/
