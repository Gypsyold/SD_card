/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
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
#define KeyUp_Pin GPIO_PIN_0
#define KeyUp_GPIO_Port GPIOC
#define KeyDown_Pin GPIO_PIN_1
#define KeyDown_GPIO_Port GPIOC
#define KeyLeft_Pin GPIO_PIN_2
#define KeyLeft_GPIO_Port GPIOC
#define KeyRight_Pin GPIO_PIN_3
#define KeyRight_GPIO_Port GPIOC
#define SD_Detect_Pin GPIO_PIN_3
#define SD_Detect_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
extern SD_HandleTypeDef hsd;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern DMA_HandleTypeDef hdma_sdio_rx;

extern UART_HandleTypeDef huart1;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
