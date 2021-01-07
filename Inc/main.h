/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

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
void Hardware_Init(void); 
void McuReset(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED2_NETWORK_Pin GPIO_PIN_5
#define LED2_NETWORK_GPIO_Port GPIOA
#define LED1_RUNNING_Pin GPIO_PIN_6
#define LED1_RUNNING_GPIO_Port GPIOA
#define NB_TX_Pin GPIO_PIN_10
#define NB_TX_GPIO_Port GPIOB
#define NB_RX_Pin GPIO_PIN_11
#define NB_RX_GPIO_Port GPIOB
#define NB_PWRKEY_Pin GPIO_PIN_6
#define NB_PWRKEY_GPIO_Port GPIOC
#define RS485_SEL_Pin GPIO_PIN_8
#define RS485_SEL_GPIO_Port GPIOC
#define RS485_TX_Pin GPIO_PIN_10
#define RS485_TX_GPIO_Port GPIOC
#define RS485_RX_Pin GPIO_PIN_11
#define RS485_RX_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */
#define LORANODE_GPIO_Port GPIOC

typedef enum
{
	FALSE,
	TRUE,
}BOOL;

typedef enum{
	false = 0,
	true = 1,
}bool;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
