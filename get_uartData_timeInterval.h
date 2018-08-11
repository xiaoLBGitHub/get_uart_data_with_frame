#ifndef	 _GET_UARTDATA_TIMEINTERVAL_H
#define	 _GET_UARTDATA_TIMEINTERVAL_H

#include "stm32f10x.h"


typedef struct
{
	uint8_t* buf;					//指向数据缓冲区

	uint16_t length; 					//反映接收数据长度
	uint16_t state; 						//指示一帧结束 ，0表示没结束，其它值表示数据长度
	uint16_t timer ;						//用于滴答中断中的倒计时
	
	uint16_t max_countDown_value;		//根据滴答中断的间隔，判断一帧结束的时间间隔
	uint16_t buf_max_length;	//存储区的最大长度
}UartDateStruct;

extern void UartData_timeInterval_Init(UartDateStruct* uartData,uint8_t* buf,uint16_t buf_length,uint16_t timeInterval);
extern void USART_IRQHandler(UartDateStruct* uartData,uint8_t data);
extern uint16_t CatchUartDataInSystickHandle(UartDateStruct* uartData);
#endif
