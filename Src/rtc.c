/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
    


  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date 
  */
  sTime.Hours = 0x13;
  sTime.Minutes = 0x40;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_TUESDAY;
  DateToUpdate.Month = RTC_MONTH_JUNE;
  DateToUpdate.Date = 0x1;
  DateToUpdate.Year = 0x20;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

void My_RTC_Init(void)
{
	#define RTC_SET_CONFIG_FLAG	0x4021    //RTC��һ��������ɱ�־

	RTC_DateTypeDef DateToUpdate;

	/*�ж��Ƿ��һ������*/
	if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR1) != RTC_SET_CONFIG_FLAG)
	{
	 	/*ȡ����������д����*/
		HAL_PWR_EnableBkUpAccess();
		/*�����ѳ�ʼ����־*/
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, RTC_SET_CONFIG_FLAG);


		/*����ʱ��Ϊ��2020-6-1-��һ��17��21��0��*/
		RTC_Set_Date(&DateToUpdate, 20, RTC_MONTH_JUNE, 0x02, RTC_WEEKDAY_TUESDAY);
		RTC_Set_Time(0x14, 0x08, 0x00);
	}
	else
	{
		RTC_Read_Config_Par(&DateToUpdate);
	}

}

/**
  * @fn RTC_Set_Time
  * @brief ����RTCʱ��
  * @details
  * @param[in] ʱ �� ��
  */
void RTC_Set_Time(uint8_t Hours, uint8_t Minutes, uint8_t Seconds)
{
	RTC_TimeTypeDef ConfigTime;
	ConfigTime.Hours = Hours;
	ConfigTime.Minutes = Minutes;
	ConfigTime.Seconds = Seconds;
	if(HAL_RTC_SetTime(&hrtc, &ConfigTime, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @fn RTC_Set_Date
  * @brief ����RTC����
  * @details
  * @param[in] �� �� �� ��
  */
void RTC_Set_Date(RTC_DateTypeDef *ConfigDate, uint8_t Year, uint8_t Month, uint8_t Date, uint8_t WeekDay)
{

	ConfigDate->Year = Year;
	ConfigDate->Month = Month;
	ConfigDate->Date = Date;
	ConfigDate->WeekDay = WeekDay;

	if(HAL_RTC_SetDate(&hrtc, ConfigDate, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

	RTC_Save_Config_Par(ConfigDate);
}


/**
  * @fn Save_Config_Par
  * @brief �����������
  * @param[in] configpar
  */
void RTC_Save_Config_Par(RTC_DateTypeDef *pData)
{
	__HAL_RTC_SECOND_ENABLE_IT(&hrtc,RTC_IT_SEC);	 //����RTCʱ�����ж�
	/*ȡ����������д����*/
	HAL_PWR_EnableBkUpAccess();
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR2,pData->Year);
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR3,pData->Month);
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR4,pData->Date);
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR5,pData->WeekDay);
}

/**
  * @fn Read_Config_Par
  * @brief ������ȡ����Ĳ���
  * @param[in] pData
  * @param[out] pData
  */
void RTC_Read_Config_Par(RTC_DateTypeDef *pData)
{
	pData->Year = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR2);
	pData->Month = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR3);
	pData->Date = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR4);
    pData->WeekDay = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR5);

    if (HAL_RTC_SetDate(&hrtc, pData, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

    __HAL_RTC_SECOND_ENABLE_IT(&hrtc,RTC_IT_SEC);	 //����RTCʱ�����ж�
}





/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
