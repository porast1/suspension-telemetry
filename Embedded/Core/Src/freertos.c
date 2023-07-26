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
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "fatfs.h"


#include "travelSensor.h"
#include "fatfs_sd.h"
#include "File_Handling_RTOS.h"
#include "button.h"
#include "menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define MAX_ACTIVE_BUTTON 4U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

button_t buttonMenu = 5;
/* USER CODE END Variables */
osThreadId buttonTaskHandle;
uint32_t buttonTaskBuffer[ 128 ];
osStaticThreadDef_t buttonTaskControlBlock;
osThreadId sensorReadHandle;
uint32_t sensorReadBuffer[ 2048 ];
osStaticThreadDef_t sensorReadControlBlock;
osThreadId SdCardHandle;
uint32_t SdCardBuffer[ 256 ];
osStaticThreadDef_t SdCardControlBlock;
osThreadId menuProcessDataHandle;
uint32_t menuProcessDataBuffer[ 2048 ];
osStaticThreadDef_t menuProcessDataControlBlock;
osSemaphoreId travelSensorSemHandle;
osStaticSemaphoreDef_t travelSensorSemControlBlock;
osSemaphoreId SendDataHandle;
osStaticSemaphoreDef_t SendDataControlBlock;
osSemaphoreId buttonSemHandle;
osStaticSemaphoreDef_t buttonSemControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void buttonTaskInit(void const * argument);
void initSensorRead(void const * argument);
void SdCardInit(void const * argument);
void menuProcessDataInit(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
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

  /* definition and creation of buttonSem */
  osSemaphoreStaticDef(buttonSem, &buttonSemControlBlock);
  buttonSemHandle = osSemaphoreCreate(osSemaphore(buttonSem), 1);

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
  /* definition and creation of buttonTask */
  osThreadStaticDef(buttonTask, buttonTaskInit, osPriorityNormal, 0, 128, buttonTaskBuffer, &buttonTaskControlBlock);
  buttonTaskHandle = osThreadCreate(osThread(buttonTask), NULL);

  /* definition and creation of sensorRead */
  osThreadStaticDef(sensorRead, initSensorRead, osPriorityNormal, 0, 2048, sensorReadBuffer, &sensorReadControlBlock);
  sensorReadHandle = osThreadCreate(osThread(sensorRead), NULL);

  /* definition and creation of SdCard */
  osThreadStaticDef(SdCard, SdCardInit, osPriorityHigh, 0, 256, SdCardBuffer, &SdCardControlBlock);
  SdCardHandle = osThreadCreate(osThread(SdCard), NULL);

  /* definition and creation of menuProcessData */
  osThreadStaticDef(menuProcessData, menuProcessDataInit, osPriorityNormal, 0, 2048, menuProcessDataBuffer, &menuProcessDataControlBlock);
  menuProcessDataHandle = osThreadCreate(osThread(menuProcessData), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_buttonTaskInit */
/**
  * @brief  Function implementing the buttonTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_buttonTaskInit */
void buttonTaskInit(void const * argument)
{
  /* USER CODE BEGIN buttonTaskInit */
  /* Infinite loop */
  for(;;)
  {
	buttonMenu = readButton();
	if ( MAX_ACTIVE_BUTTON >= buttonMenu ){
	osSemaphoreRelease(buttonSemHandle);
	}
    osDelay(30);
  }
  /* USER CODE END buttonTaskInit */
}

/* USER CODE BEGIN Header_initSensorRead */
/**
* @brief Function implementing the sensorRead thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_initSensorRead */
void initSensorRead(void const * argument)
{
  /* USER CODE BEGIN initSensorRead */

  /* Infinite loop */
  for(;;)
  {


    osDelay(30);
  }
  /* USER CODE END initSensorRead */
}

/* USER CODE BEGIN Header_SdCardInit */
/**
* @brief Function implementing the SdCard thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SdCardInit */
void SdCardInit(void const * argument)
{
  /* USER CODE BEGIN SdCardInit */
puts("jestes w menu start, kliknij:\n1.Pomiar Sagu\n2.Rozpocznij pomiary pracy zawieszenia\n");
/* Infinite loop */
  for(;;)
  {

	  if (MAX_ACTIVE_BUTTON >= buttonMenu){
		  menuSelector(buttonMenu);
	  }
	  osSemaphoreWait(buttonSemHandle, osWaitForever);
	  osDelay(20);
  }
  /* USER CODE END SdCardInit */
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
  /* Infinite loop */
  for(;;)
  {
	menuCalculateBlock();
    osDelay(10);
  }
  /* USER CODE END menuProcessDataInit */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
