/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l1xx_hal.h"

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
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define CANINT_Pin GPIO_PIN_4
#define CANINT_GPIO_Port GPIOC
#define CAN_RX0BF_Pin GPIO_PIN_5
#define CAN_RX0BF_GPIO_Port GPIOC
#define CAN_RX0BF_EXTI_IRQn EXTI9_5_IRQn
#define CAN_RX1BF_Pin GPIO_PIN_0
#define CAN_RX1BF_GPIO_Port GPIOB
#define GND_Pin GPIO_PIN_12
#define GND_GPIO_Port GPIOB
#define VDD_Pin GPIO_PIN_13
#define VDD_GPIO_Port GPIOB
#define BLUE_SWITCH_Pin GPIO_PIN_8
#define BLUE_SWITCH_GPIO_Port GPIOC
#define GREEN_SWITCH_Pin GPIO_PIN_9
#define GREEN_SWITCH_GPIO_Port GPIOC
#define LED_BLUE_Pin GPIO_PIN_10
#define LED_BLUE_GPIO_Port GPIOC
#define LED_GREEN_Pin GPIO_PIN_11
#define LED_GREEN_GPIO_Port GPIOC
#define LED_RED_Pin GPIO_PIN_12
#define LED_RED_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
static const unsigned int TASK_QUEUE_TIMEOUT = 0; // 0ms
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
