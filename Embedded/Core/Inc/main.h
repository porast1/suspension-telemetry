/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BRAKE_SENSOR_RIGHT_Pin GPIO_PIN_0
#define BRAKE_SENSOR_RIGHT_GPIO_Port GPIOC
#define BRAKE_SENSOR_LEFT_Pin GPIO_PIN_1
#define BRAKE_SENSOR_LEFT_GPIO_Port GPIOC
#define TRAVEL_SENSOR_FRONT_Pin GPIO_PIN_2
#define TRAVEL_SENSOR_FRONT_GPIO_Port GPIOC
#define TRAVEL_SENSOR_REAR_Pin GPIO_PIN_3
#define TRAVEL_SENSOR_REAR_GPIO_Port GPIOC
#define PRESSURE_SENSOR_FRONT_Pin GPIO_PIN_1
#define PRESSURE_SENSOR_FRONT_GPIO_Port GPIOA
#define PRESSURE_SENSOR_REAR_Pin GPIO_PIN_2
#define PRESSURE_SENSOR_REAR_GPIO_Port GPIOA
#define BATTERY_LEVEL_Pin GPIO_PIN_4
#define BATTERY_LEVEL_GPIO_Port GPIOA
#define ACCELEROMETER_INT0_Pin GPIO_PIN_7
#define ACCELEROMETER_INT0_GPIO_Port GPIOC
#define SD_detection_Pin GPIO_PIN_8
#define SD_detection_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define CHARGE_DETECTION_Pin GPIO_PIN_15
#define CHARGE_DETECTION_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define RIGHT_BUTTON_Pin GPIO_PIN_6
#define RIGHT_BUTTON_GPIO_Port GPIOB
#define LEFT_BUTTON_Pin GPIO_PIN_7
#define LEFT_BUTTON_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
