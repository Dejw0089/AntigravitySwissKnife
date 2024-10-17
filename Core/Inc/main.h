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
#define B1_Pin GPIO_PIN_0
#define B1_GPIO_Port GPIOA
#define OK_Pin GPIO_PIN_5
#define OK_GPIO_Port GPIOA
#define Blink_Pin GPIO_PIN_12
#define Blink_GPIO_Port GPIOB
#define close_Pin GPIO_PIN_13
#define close_GPIO_Port GPIOB
#define open_Pin GPIO_PIN_14
#define open_GPIO_Port GPIOB
#define Sport_Pin GPIO_PIN_15
#define Sport_GPIO_Port GPIOB
#define GthLamp2_Pin GPIO_PIN_3
#define GthLamp2_GPIO_Port GPIOB
#define ttRelay_Pin GPIO_PIN_4
#define ttRelay_GPIO_Port GPIOB
#define GthRelay_Pin GPIO_PIN_5
#define GthRelay_GPIO_Port GPIOB
#define GthLicznik_Pin GPIO_PIN_8
#define GthLicznik_GPIO_Port GPIOB
#define GthLamp1_Pin GPIO_PIN_9
#define GthLamp1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
