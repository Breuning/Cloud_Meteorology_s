/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */


UART_RECEIVETYPE NBUart_RX;
UART_RECEIVETYPE RS485Uart_RX;


/* USER CODE END 0 */

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_uart4_rx;
DMA_HandleTypeDef hdma_uart4_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

/* UART4 init function */
void MX_UART4_Init(void)
{

  huart4.Instance = UART4;
  huart4.Init.BaudRate = 4800;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspInit 0 */

  /* USER CODE END UART4_MspInit 0 */
    /* UART4 clock enable */
    __HAL_RCC_UART4_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    GPIO_InitStruct.Pin = RS485_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(RS485_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = RS485_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(RS485_RX_GPIO_Port, &GPIO_InitStruct);

    /* UART4 DMA Init */
    /* UART4_RX Init */
    hdma_uart4_rx.Instance = DMA2_Channel3;
    hdma_uart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_uart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_rx.Init.Mode = DMA_CIRCULAR;
    hdma_uart4_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_uart4_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_uart4_rx);

    /* UART4_TX Init */
    hdma_uart4_tx.Instance = DMA2_Channel5;
    hdma_uart4_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_uart4_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_uart4_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_uart4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_uart4_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_uart4_tx.Init.Mode = DMA_NORMAL;
    hdma_uart4_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_uart4_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_uart4_tx);

    /* UART4 interrupt Init */
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspInit 1 */

  /* USER CODE END UART4_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = NB_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(NB_TX_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = NB_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(NB_RX_GPIO_Port, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

    /* USART3_TX Init */
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART4)
  {
  /* USER CODE BEGIN UART4_MspDeInit 0 */

  /* USER CODE END UART4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART4_CLK_DISABLE();
  
    /**UART4 GPIO Configuration    
    PC10     ------> UART4_TX
    PC11     ------> UART4_RX 
    */
    HAL_GPIO_DeInit(GPIOC, RS485_TX_Pin|RS485_RX_Pin);

    /* UART4 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* UART4 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART4_IRQn);
  /* USER CODE BEGIN UART4_MspDeInit 1 */

  /* USER CODE END UART4_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOB, NB_TX_Pin|NB_RX_Pin);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

void UART_IT_IDLE_InitAll(void)
{

		UART_IT_IDLE_Init(&huart3, NBUart_RX.RX_Buf, RECEIVELEN);      //UART3--��NBiot����                                                         115200
		UART_IT_IDLE_Init(&huart4, RS485Uart_RX.RX_Buf, RECEIVELEN);   //UART4--��RS485����(485���߽�����ؿ������豸����)  9600
}

//���ڽ��տ����жϳ�ʼ��
void UART_IT_IDLE_Init(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
{
		__HAL_UART_CLEAR_FLAG(huart,UART_FLAG_IDLE);               	  //��������жϱ�־
		HAL_Delay(2);
		__HAL_UART_CLEAR_FLAG(huart,UART_FLAG_TC);                 	  //������ͱ�־

		HAL_UART_Receive_DMA(huart, pData, Size);  					  //����DMA����
		__HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);                 	  //ʹ�ܿ����ж�

		__HAL_UART_CLEAR_FLAG(huart,UART_FLAG_IDLE);             	  //��������жϱ�־


//�����ϴ��빦�ܣ������ڽ��յ�������ͨ��DMA���ݸ�pData��Ȼ��������ڿ����жϣ����ж�������һ������
}

//���ڽ��տ����жϻص�����
void UARTReceive_IDLECallback(UART_HandleTypeDef *huart, UART_RECEIVETYPE *Uart_RX, uint16_t Size)
{

		if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))       //�ж��Ƿ���������ж�
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);                          //��������жϱ�־
			HAL_UART_DMAStop(huart);                                   //ͣ��DMA

			//CNDTRΪDMAͨ���������ݵļ�������ע����һ���ݼ���������������Ҫ��DMA�Ļ��������ܳ��ȼ�ȥ�ü�������ֵ����DMAͨ���������ݵĳ��ȣ�
			Uart_RX->rx_len = Size - huart->hdmarx->Instance->CNDTR;   //��λDMAָ��

			Uart_RX->receive_flag = 1;                                 //��λ���ڽ��ձ�־λ

			HAL_UART_Receive_DMA(huart, Uart_RX->RX_Buf, Size);
		}
}




/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
