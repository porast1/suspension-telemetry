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
#include "stdint.h"
#include "stdio.h"
#include "adc.h"
#include "tim.h"
#include "cmsis_os.h"
/******************************************************************************
* Module Preprocessor Constants
*******************************************************************************/


#define ADC_RESOLUTION 4096U
#define FRONT_SENSOR_TRAVEL 300U
#define REAR_SENSOR_TRAVEL 125U

#define BUFFER_SIZE 256U
#define DATA_BUFF_READY  1U
#define DATA_BUFF_EMPTY	 0U
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

static volatile uint8_t dataReadyFlag;

volatile int time_start;
volatile int time_end;
/******************************************************************************
* Function Prototypes
*******************************************************************************/
void processData(void);
void startAdcDma (ADC_HandleTypeDef *hadc);
static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite, int16_t sensorTravel);
static void printAllBuffers(int16_t *frontBuff, int16_t *rearBuff, int size);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
/******************************************************************************
* Function Definitions
*******************************************************************************/
void processData(void){
	if(DATA_BUFF_READY == dataReadyFlag){
		for (int n = 0; n < (BUFFER_SIZE/2) -1 ; n += 2){
			outRearBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n], REAR_SENSOR_TRAVEL);
			outFrontBufPtr[n/2] = convertAdcToTravel(&inBufPtr[n + 1], FRONT_SENSOR_TRAVEL);
		}
		//printAllBuffers(adcFrontDataRead, adcRearDataRead, BUFFER_SIZE/2);
		dataReadyFlag = DATA_BUFF_EMPTY;
	}
}
void startAdcDma (ADC_HandleTypeDef *hadc){
	HAL_ADC_Start_DMA(hadc, (uint32_t*)adcDataWrite, BUFFER_SIZE);
	HAL_TIM_Base_Start(&htim2);
}
static int16_t convertAdcToTravel(volatile int16_t *adcDataWrite, int16_t sensorTravel){

	return (*adcDataWrite*sensorTravel/ADC_RESOLUTION);
}
static void printAllBuffers(int16_t *frontBuff, int16_t *rearBuff, int size){
	puts("front sensor:");
	for (int i = 0; i < size; i++){
		printf(" %d",frontBuff[i]);
	}
	puts("\nrear sensor:");
	for (int i = 0; i < size; i++){
			printf(" %d",rearBuff[i]);
		}
		puts("\n");
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
	inBufPtr = &adcDataWrite[0];
	outRearBufPtr = &adcRearDataRead[0];
	outFrontBufPtr = &adcFrontDataRead[0];
	dataReadyFlag = 1;

}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	inBufPtr = &adcDataWrite[BUFFER_SIZE/2];
	outRearBufPtr = &adcRearDataRead[BUFFER_SIZE/4];
	outFrontBufPtr = &adcFrontDataRead[BUFFER_SIZE/4];
	dataReadyFlag = 1;
}


/*************** END OF FUNCTIONS ***************************************************************************/
