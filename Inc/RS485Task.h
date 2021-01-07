/*
 * RS485Task.h
 *
 *  Created on: 2020��7��14��
 *      Author: Breuning
 */

#ifndef RS485TASK_H_
#define RS485TASK_H_

typedef struct
{


	float Temperature;
	float Humidity;
	uint32_t Illumination;
	uint32_t Winddirection;
	float Windspeed;
	float Rainfall;

}RS485Data_Struct;

typedef enum
{
	RECEIVE = 0,
    TRANSMIT = 1

}RS485_STATE;

//������ʵ��ʮ�������븡������ת
typedef union test_float
{
   float all;
   struct
   {
     uint8_t Byte[4];
   }stByte;

}FLOAT;

//������ʵ��ʮ��������uint32��ת
typedef union test_uint32
{
   uint32_t all;
   struct
   {
     uint8_t Byte[4];
   }stByte;

}UINT32;



extern RS485Data_Struct RS485Data;
extern BOOL RS485DataReadTimerFlag;

extern void RS485_Task();
extern void RS485Uart_RX_TX_Switch(RS485_STATE RS485state);
uint16_t CRC16(uint8_t *puchMsg, uint8_t usDataLen);



#endif /* RS485TASK_H_ */
