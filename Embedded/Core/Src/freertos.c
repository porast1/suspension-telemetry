/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "stdio.h"
#include "stdint.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include "fatfs.h"
#include "i2c.h"
#include "ff.h"

#include "travelSensor.h"
#include "File_Handling_RTOS.h"
#include "button.h"
#include "menu.h"
#include "liquidcrystal_i2c.h"
#include "ADXL345.h"
#include "lcdMenu.h"
#include "usb_device.h"
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_ACTIVE_BUTTON 4U
#define adxl_address 0x53 << 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
 int16_t result[2] =
	{ 0 };
	 int16_t resultPressure[2] =
	{ 0 };

button_t buttonMenu = 5;
/* USER CODE END Variables */
osThreadId SdCardHandle;
osThreadId sensorReadHandle;
uint32_t sensorReadBuffer[ 1024 ];
osStaticThreadDef_t sensorReadControlBlock;
osThreadId menuProcessDataHandle;
uint32_t menuProcessDataBuffer[ 512 ];
osStaticThreadDef_t menuProcessDataControlBlock;
osSemaphoreId travelSensorSemHandle;
osStaticSemaphoreDef_t travelSensorSemControlBlock;
osSemaphoreId SendDataHandle;
osStaticSemaphoreDef_t SendDataControlBlock;
osSemaphoreId writeCalibrationSemHandle;
osStaticSemaphoreDef_t writeCalibrationSemControlBlock;
osSemaphoreId SagMeasurementHandle;
osStaticSemaphoreDef_t SagMeasurementControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void SdCardInit(void const * argument);
void sensorReadInit(void const * argument);
void menuProcessDataInit(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of travelSensorSem */
  osSemaphoreStaticDef(travelSensorSem, &travelSensorSemControlBlock);
  travelSensorSemHandle = osSemaphoreCreate(osSemaphore(travelSensorSem), 1);

  /* definition and creation of SendData */
  osSemaphoreStaticDef(SendData, &SendDataControlBlock);
  SendDataHandle = osSemaphoreCreate(osSemaphore(SendData), 1);

  /* definition and creation of writeCalibrationSem */
  osSemaphoreStaticDef(writeCalibrationSem, &writeCalibrationSemControlBlock);
  writeCalibrationSemHandle = osSemaphoreCreate(osSemaphore(writeCalibrationSem), 1);

  /* definition and creation of SagMeasurement */
  osSemaphoreStaticDef(SagMeasurement, &SagMeasurementControlBlock);
  SagMeasurementHandle = osSemaphoreCreate(osSemaphore(SagMeasurement), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of SdCard */
  osThreadDef(SdCard, SdCardInit, osPriorityNormal, 0, 1024);
  SdCardHandle = osThreadCreate(osThread(SdCard), NULL);

  /* definition and creation of sensorRead */
  osThreadStaticDef(sensorRead, sensorReadInit, osPriorityNormal, 0, 1024, sensorReadBuffer, &sensorReadControlBlock);
  sensorReadHandle = osThreadCreate(osThread(sensorRead), NULL);

  /* definition and creation of menuProcessData */
  osThreadStaticDef(menuProcessData, menuProcessDataInit, osPriorityNormal, 0, 512, menuProcessDataBuffer, &menuProcessDataControlBlock);
  menuProcessDataHandle = osThreadCreate(osThread(menuProcessData), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_SdCardInit */
/**
 * @brief  Function implementing the SdCard thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_SdCardInit */
void SdCardInit(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN SdCardInit */
	Mount_SD("/");
	Unmount_SD("/");
	/* Infinite loop */
	for (;;)
	{

		osDelay(1000);
	}
  /* USER CODE END SdCardInit */
}

/* USER CODE BEGIN Header_sensorReadInit */
/**
 * @brief Function implementing the sensorRead thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_sensorReadInit */
void sensorReadInit(void const * argument)
{
  /* USER CODE BEGIN sensorReadInit */

	/* Infinite loop */
	for (;;)
	{
		if (osOK == osSemaphoreWait(travelSensorSemHandle, osWaitForever))
		{
			if (MENU_MEASURMENT_START == getMenuSelector())
			{
				char *sensorDataPtr = getSensorDataFileName();
				processData(sensorDataPtr);
			}
			else if (MENU_SAG_START == getMenuSelector())
			{
				processDataSag(result, resultPressure);
				lcdMenuSagStart(result, resultPressure);
			}
			else if (MENU_CALIBRATION == getMenuSelector()){
				stopAdcDma();
				travelPressureSensorCalibration();
				lcdMenuFinishedNotification("Cal Finished");
				lcdMenuStart();
				setMenuSelector(MENU_START);
			}
		}

		else
		{

		}
		osDelay(50);
	}
  /* USER CODE END sensorReadInit */
}

/* USER CODE BEGIN Header_menuProcessDataInit */
/**
 * @brief Function implementing the menuProcessData thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_menuProcessDataInit */
void menuProcessDataInit(void const * argument)
{
  /* USER CODE BEGIN menuProcessDataInit */
	button_t buttonLeft;
	button_t buttonRight;
	HAL_I2C_Init(&hi2c1);
	SSD1306_Init();
	lcdMenuStart();
	/* Infinite loop */
	for (;;)
	{
		buttonLeft = readButton(LEFT_BUTTON_GPIO_Port, LEFT_BUTTON_Pin);
		buttonRight = readButton(RIGHT_BUTTON_GPIO_Port, RIGHT_BUTTON_Pin);
		if ((BUTTON_SNA != buttonLeft) || (BUTTON_SNA != buttonRight))
		{
			menuSelector(buttonLeft, buttonRight);
		}

		osDelay(10);
	}
  /* USER CODE END menuProcessDataInit */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
