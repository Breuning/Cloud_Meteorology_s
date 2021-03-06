/*
 * RS485DataTask.c
 *
 *  Created on: 2020年7月14日
 *      Author: Breuning
 */

#include "main.h"
#include "string.h"
#include "usart.h"
#include "RS485Task.h"


uint8_t Command_Temhum       [8] = {0x0B, 0x03, 0x01, 0xF4, 0x00, 0x02, 0x84, 0xAF};   //地址11，寄存器地址0x01F4
uint8_t Command_Lllumination [8] = {0x0B, 0x03, 0x01, 0xFA, 0x00, 0x02, 0xE5, 0x6C};   //地址11，寄存器地址0x01FA
uint8_t Command_Winddirection[8] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x38};   //地址02，寄存器地址0x0000
uint8_t Command_Windspeed    [8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};   //地址01，寄存器地址0x0000

uint8_t Command_Rainfall     [8] = {0x03, 0x03, 0x00, 0x00, 0x00, 0x01, 0x85, 0xE8};   //地址03，寄存器地址0x0000

RS485Data_Struct RS485Data;
BOOL RS485DataReadTimerFlag = FALSE;

static void GetRS485Data(uint8_t *command, RS485Data_Struct *rs485data);


void RS485_Task()
{

	if(RS485DataReadTimerFlag == TRUE)              //通过TIM5设置为每3min读取一次485数据
	{
		RS485DataReadTimerFlag = FALSE;

		GetRS485Data(Command_Temhum, &RS485Data);
		GetRS485Data(Command_Lllumination, &RS485Data);
		GetRS485Data(Command_Winddirection, &RS485Data);
		GetRS485Data(Command_Windspeed, &RS485Data);
		GetRS485Data(Command_Rainfall, &RS485Data);
	}

}

static void GetRS485Data(uint8_t *command, RS485Data_Struct *rs485data)
{
	uint16_t RS485Data_CrcVal;
    uint16_t RS485Data_CheckCrc;

    RS485Uart_RX_TX_Switch(TRANSMIT);
    HAL_Delay(5);
	HAL_UART_Transmit(&huart4, command, 8, 0xff);
	RS485Uart_RX_TX_Switch(RECEIVE);
	HAL_Delay(100);

	if(RS485Uart_RX.receive_flag == 1)
	{
		if(command == Command_Windspeed || command == Command_Rainfall)
		{
			if(RS485Uart_RX.rx_len != 7)
				return;

			RS485Data_CrcVal   = (RS485Uart_RX.RX_Buf[5]<<8) | RS485Uart_RX.RX_Buf[6];
			RS485Data_CheckCrc = CRC16(RS485Uart_RX.RX_Buf, 5);
		}
		else
		{
			if(RS485Uart_RX.rx_len != 9)
				return;

			RS485Data_CrcVal   = (RS485Uart_RX.RX_Buf[7]<<8) | RS485Uart_RX.RX_Buf[8];
			RS485Data_CheckCrc = CRC16(RS485Uart_RX.RX_Buf, 7);
		}

		if(RS485Data_CrcVal != RS485Data_CheckCrc)
			return;




		if(command == Command_Temhum)
		{
			rs485data->Temperature = (float)(((RS485Uart_RX.RX_Buf[5] << 8) | RS485Uart_RX.RX_Buf[6]) / 10.0);
			rs485data->Humidity    = (float)(((RS485Uart_RX.RX_Buf[3] << 8) | RS485Uart_RX.RX_Buf[4]) / 10.0);
		}
		else if(command == Command_Lllumination)
		{
			UINT32 illu;
			illu.stByte.Byte[0] = RS485Uart_RX.RX_Buf[6];
			illu.stByte.Byte[1] = RS485Uart_RX.RX_Buf[5];
			illu.stByte.Byte[2] = RS485Uart_RX.RX_Buf[4];
			illu.stByte.Byte[3] = RS485Uart_RX.RX_Buf[3];
			rs485data->Illumination = illu.all;
		}
		else if(command == Command_Winddirection)
		{
			rs485data->Winddirection = (float)(((RS485Uart_RX.RX_Buf[5] << 8) | RS485Uart_RX.RX_Buf[6]));
		}
		else if(command == Command_Windspeed)
		{
			rs485data->Windspeed     = (float)(((RS485Uart_RX.RX_Buf[3] << 8) | RS485Uart_RX.RX_Buf[4]) / 10.0);
		}
		else if(command == Command_Rainfall)
		{
			rs485data->Rainfall      = (float)(((RS485Uart_RX.RX_Buf[3] << 8) | RS485Uart_RX.RX_Buf[4]) / 10.0);
		}
		else
			return;


		RS485Uart_RX.receive_flag = 0;
		memset(RS485Uart_RX.RX_Buf, 0 , sizeof(RS485Uart_RX.RX_Buf));

//		HAL_UART_Receive_DMA(&huart4, RS485Uart_RX.RX_Buf, RECEIVELEN);
	}
}


void RS485Uart_RX_TX_Switch(RS485_STATE RS485state)
{
	if(RS485state == TRANSMIT)
		HAL_GPIO_WritePin(RS485_SEL_GPIO_Port, RS485_SEL_Pin, GPIO_PIN_SET);		//拉高RS485_SEL脚，RS485为发送状态
	if(RS485state == RECEIVE)
		HAL_GPIO_WritePin(RS485_SEL_GPIO_Port, RS485_SEL_Pin, GPIO_PIN_RESET);		//拉低RS485_SEL脚，RS485为接收状态
}

const uint8_t auchCRCHi[] =
 {
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
    0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40
};

const uint8_t auchCRCLo[] =
{
    0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,
    0xC6,0x06,0x07,0xC7,0x05,0xC5,0xC4,0x04,
    0xCC,0x0C,0x0D,0xCD,0x0F,0xCF,0xCE,0x0E,
    0x0A,0xCA,0xCB,0x0B,0xC9,0x09,0x08,0xC8,
    0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
    0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,
    0x14,0xD4,0xD5,0x15,0xD7,0x17,0x16,0xD6,
    0xD2,0x12,0x13,0xD3,0x11,0xD1,0xD0,0x10,
    0xF0,0x30,0x31,0xF1,0x33,0xF3,0xF2,0x32,
    0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
    0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,
    0xFA,0x3A,0x3B,0xFB,0x39,0xF9,0xF8,0x38,
    0x28,0xE8,0xE9,0x29,0xEB,0x2B,0x2A,0xEA,
    0xEE,0x2E,0x2F,0xEF,0x2D,0xED,0xEC,0x2C,
    0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
    0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,
    0xA0,0x60,0x61,0xA1,0x63,0xA3,0xA2,0x62,
    0x66,0xA6,0xA7,0x67,0xA5,0x65,0x64,0xA4,
    0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,0x6E,0xAE,
    0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
    0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,
    0xBE,0x7E,0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,
    0xB4,0x74,0x75,0xB5,0x77,0xB7,0xB6,0x76,
    0x72,0xB2,0xB3,0x73,0xB1,0x71,0x70,0xB0,
    0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
    0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,
    0x9C,0x5C,0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,
    0x5A,0x9A,0x9B,0x5B,0x99,0x59,0x58,0x98,
    0x88,0x48,0x49,0x89,0x4B,0x8B,0x8A,0x4A,
    0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
    0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,
    0x82,0x42,0x43,0x83,0x41,0x81,0x80,0x40
};

uint16_t CRC16(uint8_t *puchMsg, uint8_t usDataLen)
{

	uint8_t uchCRCHi = 0xFF;
	uint8_t uchCRCLo = 0xFF;
	uint8_t uIndex;

	while (usDataLen--)
	{
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);
}
