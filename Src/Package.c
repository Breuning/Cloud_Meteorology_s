/*
 * Package.c
 *
 *  Created on: 2020年7月16日
 *      Author: Breuning
 */

#include "Package.h"
#include "string.h"
#include "cJSON.h"
#include "MD5.h"
#include "usart.h"
#include "rtc.h"
#include "NBiotTask.h"
#include "RS485Task.h"
#include <stdlib.h>
#include "stdio.h"


uint8_t CloudPackage[200] = { 0 };

static void AddMessageToJsonObj(cJSON *root);
static void _addIntToJson(cJSON *root, const char *name, uint32_t value, bool complement);
static void _addTimeToJson(cJSON *root, const char *name);
static void _addStringToJson(cJSON *root, const char *name, const char *string);
static void _addFloatToJson(cJSON *js_list, const char *name, float value);


//static void _addTimeToJson(cJSON *root, const char *name);
//static void _addMd5ToJson(cJSON *root,const char *name);


void PackageComposition(void)
{
	/*json缓冲区(发送至NB)*/
	uint8_t jsonOutPutBuf[200] = { 0 };


	/*创建JSON对象*/
	cJSON *root;
	root = cJSON_CreateObject();
	if (root == NULL)
	{
		cJSON_Delete(root);
		return;
	}

	AddMessageToJsonObj(root);

	if (cJSON_PrintPreallocated(root, (char*) jsonOutPutBuf, sizeof(jsonOutPutBuf), FALSE))
	{

		strcpy((char *)CloudPackage, (const char *)jsonOutPutBuf);
//		HAL_UART_Transmit(&huart2, (uint8_t *)jsonOutPutBuf, sizeof(jsonOutPutBuf), 1000);  	//将数组内的数据通过USART2发送至NB
//		LED1_RJ45_Blink();																		//发一包数据闪一次灯
	}

	HAL_Delay(100);

	cJSON_Delete(root);					                  //指针用完了以后，需要手工调用 cJSON_Delete(root)进行内存回收，释放内存空间
	root = NULL;

	memset(jsonOutPutBuf, 0, sizeof(jsonOutPutBuf));      //建议使用后清零

}

/*把设备上传需要的数据添加到JSON中 */
static void AddMessageToJsonObj(cJSON *root)
{
	_addIntToJson(root, "messageType", 0, false);
	_addStringToJson(root, "devNum", (const char *)NBiotIMEI);

	_addTimeToJson(root, "collectDt");

//	_addIntToJson(root, "upInterval", 600, false);       //上传间隔不传
//	_addMd5ToJson(root, "apptoken");                     //MD5不传

	_addFloatToJson(root, "Temp", RS485Data.Temperature);
	_addFloatToJson(root, "Humi", RS485Data.Humidity);
	_addIntToJson(root, "Illu", RS485Data.Illumination, false);
	_addIntToJson(root, "WDire", RS485Data.Winddirection, false);
	_addFloatToJson(root, "WSped", RS485Data.Windspeed);
	_addFloatToJson(root, "Rain", RS485Data.Rainfall);
}


static void _addIntToJson(cJSON *root, const char *name, uint32_t value, bool complement)
{
	uint8_t num_buf[8]={0};
	if(complement)
	{
		sprintf((char *)num_buf, "%02ld", value);
	}
	else
	{
		sprintf((char *)num_buf, "%0ld", value);
	}

	cJSON_AddStringToObject(root, name, (const char*) num_buf);
}

static void _addStringToJson(cJSON *root, const char *name, const char *string)
{
	cJSON_AddStringToObject(root, name, string);
}

static void _addFloatToJson(cJSON *root, const char *name, float value)
{
	uint8_t value_buf[8]={0};

	sprintf((char*)value_buf, "%3.1f", value);
	cJSON_AddStringToObject(root, name, (const char*)value_buf);
}


static void _addTimeToJson(cJSON *root, const char *name)
{
	uint8_t time_str_buf[26] = { '\0' };

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

//没有屏幕更新程序无法修改时间不使用RTC,改用通过读取NB模组获取基站实时时间

//	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
//	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
//	RTC_Read_Config_Par(&date);

	NBiot_ATSend(ATCmds[AT_CTZR]);
	NBiot_CleanRXBuf();

	NBiot_ATSend(ATCmds[AT_CLOCK]);
	if(NBUart_RX.receive_flag)
	{
		uint8_t year_str   [2] = {NBUart_RX.RX_Buf[19], NBUart_RX.RX_Buf[20]};
		uint8_t month_str  [2] = {NBUart_RX.RX_Buf[22], NBUart_RX.RX_Buf[23]};
		uint8_t date_str   [2] = {NBUart_RX.RX_Buf[25], NBUart_RX.RX_Buf[26]};
		uint8_t hours_str  [2] = {NBUart_RX.RX_Buf[28], NBUart_RX.RX_Buf[29]};
		uint8_t minutes_str[2] = {NBUart_RX.RX_Buf[31], NBUart_RX.RX_Buf[32]};
		uint8_t seconds_str[2] = {NBUart_RX.RX_Buf[34], NBUart_RX.RX_Buf[35]};
//		int8_t zone_str    [3] = {NBUart_RX.RX_Buf[36], NBUart_RX.RX_Buf[37], NBUart_RX.RX_Buf[38]};

		date.Year    = atoi((const char *)year_str);
		date.Month   = atoi((const char *)month_str);
		date.Date    = atoi((const char *)date_str);
//		time.Hours   = atoi((const char *)hours_str) + atoi((const char *)zone_str)/4;
		time.Hours   = atoi((const char *)hours_str) + 8;
		time.Minutes = atoi((const char *)minutes_str);
		time.Seconds = atoi((const char *)seconds_str);


///***********************************************************
		if(time.Hours > 23 && time.Hours < 32)                  // 24,25,26,27,28,29,30,31
		{
			time.Hours = time.Hours - 24;

			if(date.Month == 1 || date.Month == 3 || date.Month == 5 || date.Month == 7 || date.Month == 8 || date.Month == 10 || date.Month == 12)
			{
				if(date.Date == 31)
				{
					date.Month = date.Month + 1;
					date.Date = 1;
				}
				else
				{
					date.Date = date.Date + 1;
				}
			}
			else if(date.Month == 4 || date.Month == 6 || date.Month == 9 || date.Month == 10)
			{
				if(date.Date == 30)
				{
					date.Month = date.Month + 1;
					date.Date = 1;
				}
				else
				{
					date.Date = date.Date + 1;
				}
			}
			else if(date.Month == 2)
			{
				if(date.Date == 28)
				{
					date.Month = date.Month + 1;
					date.Date = 1;
				}
				else
				{
					date.Date = date.Date + 1;
				}
			}
		}
//*************************************************************/

		NBUart_RX.receive_flag = 0;
		memset(NBUart_RX.RX_Buf, 0 , sizeof(NBUart_RX.RX_Buf));
	}


	sprintf((char *) time_str_buf, "%d-%02d-%02d %02d:%02d:%02d", 2000 + date.Year, date.Month, date.Date, time.Hours, time.Minutes, time.Seconds);
	cJSON_AddStringToObject(root, name, (const char*) time_str_buf);
}

/*
static void _addMd5ToJson(cJSON *root,const char *name)
{
	uint8_t date_str_buf[42] = { '\0' };
	uint8_t md5_str[17];

	const char MD5_KEY[] = "ABCk4JBQWEL.SNWn2&(M)@kEY";

	MD5_CTX hmd5;
	RTC_DateTypeDef date;

	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	sprintf((char *) date_str_buf, "%d%02d%02d", 2000 + date.Year, date.Month, date.Date);
	strcat((char*) date_str_buf, (char*) MD5_KEY);

	MD5Init(&hmd5);
	MD5Update(&hmd5, (uint8_t*) date_str_buf, strlen((const char*) date_str_buf));
	MD5Final(&hmd5, md5_str);

	memset(date_str_buf,0,sizeof(date_str_buf));
	for (uint32_t i = 0; i < 16; i++)              //MD5值转成字符
	{
		uint8_t temp = md5_str[i];
		sprintf((char*) (date_str_buf + 2 * i), "%02x", temp);
	}

	cJSON_AddStringToObject(root, name, (const char*) date_str_buf);
}
*/

