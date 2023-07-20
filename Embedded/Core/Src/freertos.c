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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FILE_NAME_SIZE 20
#define SD_INIT_TIME 10
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static uint8_t path = 0;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId sensorReadHandle;
uint32_t sensorReadBuffer[ 4096 ];
osStaticThreadDef_t sensorReadControlBlock;
osThreadId SdCardHandle;
uint32_t SdCardBuffer[ 256 ];
osStaticThreadDef_t SdCardControlBlock;
osSemaphoreId travelSensorSemHandle;
osStaticSemaphoreDef_t travelSensorSemControlBlock;
osSemaphoreId SendDataHandle;
osStaticSemaphoreDef_t SendDataControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void initSensorRead(void const * argument);
void SdCardInit(void const * argument);

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of sensorRead */
  osThreadStaticDef(sensorRead, initSensorRead, osPriorityNormal, 0, 4096, sensorReadBuffer, &sensorReadControlBlock);
  sensorReadHandle = osThreadCreate(osThread(sensorRead), NULL);

  /* definition and creation of SdCard */
  osThreadStaticDef(SdCard, SdCardInit, osPriorityHigh, 0, 256, SdCardBuffer, &SdCardControlBlock);
  SdCardHandle = osThreadCreate(osThread(SdCard), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
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
/**
 * Create different file name per measurement
 */
	char dir[FILE_NAME_SIZE];
	char frontSensor[FILE_NAME_SIZE];
	char rearSensor[FILE_NAME_SIZE];
	sprintf(dir,"Data%d",path);
	sprintf(frontSensor,"Data%d/FRONT%d.txt",path,path);
	sprintf(rearSensor,"Data%d/Rear%d.txt",path,path);
	path++;
	/**
	 * Mount SD Card and create dir and files for Travel sensors
	 */
	Mount_SD("/");
	Format_SD();
	Create_Dir(dir);
	Create_File(frontSensor);
	Create_File(rearSensor);
	Unmount_SD("/");
	/**
	 * Wait for system initialize
	 */
	osDelayUntil((uint32_t*)osKernelSysTick(), SD_INIT_TIME);
	startAdcDma();

  /* Infinite loop */
  for(;;)
  {

	processData(frontSensor,rearSensor);
    osDelay(1);
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

/* Infinite loop */
  for(;;)
  {

	  osDelay(200);
  }
  /* USER CODE END SdCardInit */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
