#include "get_uartData_timeInterval.h"

/*
  使用方法：1、新建 UartDateStruct 结构体，与一个uint8_t 缓存数组，调用UartData_timeInterval_Init 初始化
            2、确定超时等待时间 timeInterval
            3、将函数 USART_IRQHandler 放入对应的串口RXNE中断中
            4、CatchUartDataInSystickHandle 将此函数放入滴答中断中的1ms中断中
            5、当 CatchUartDataInSystickHandle 返回值不为0时，认为收到1个完整帧，可以从缓冲区读取len个字节

*/
#define COUNT_DOWN_DEFAULT	0xFFFF
/**
  * @brief	在滴答中断中检测串口是否有新数据到来
  * @note 	
  * @param	uartData	串口数据结构
  * @retva	
  */  
static uint16_t CatchUartData(UartDateStruct* uartData)  
{
	if(uartData->length != 0)
	{
		uartData->state = uartData->length;
		uartData->length = 0;
	}
	return uartData->state;
}
/**
  * @brief	在滴答中断中，对帧间隔倒计时。当为0时，判断是否收到数据。
  * @note 	
  * @param	uartData缓冲区数据结构
  * @retva	当收到完整的一帧时，返回一帧数据的长度，当没有数据或者认为一帧没有结束时，返回0
  */  

uint16_t CatchUartDataInSystickHandle(UartDateStruct* uartData)
{
	uint16_t len=0;
	if(uartData->timer == 0)					//判断是否到规定的时间间隔
	{
		uartData->timer = COUNT_DOWN_DEFAULT;
		len = CatchUartData(uartData);
		uartData->state = 0;
		return len;
	}
	else if(uartData->timer != 0 && uartData->timer != COUNT_DOWN_DEFAULT)
	{
		(uartData->timer)--;
		return 0;
	}
	else	//没有收到数据的时候不做操作
	{
		return 0;
	}
}
/**
  * @brief	放在串口中断中，将接收到的数据放到数据结构中
  * @note 	
  * @param	uartData	串口数据结构
						data			串口接收寄存器中的数据
  * @retva
  */     
void USART_IRQHandler(UartDateStruct* uartData,uint8_t data)
{
	if(uartData->length >= uartData->buf_max_length) //在中断中接收数据，防止数据溢出
	{
		uartData->length = 0;
	}
	uartData->buf[uartData->length] = data;
	uartData->length++;
	uartData->timer = uartData->max_countDown_value;
}

/**
  * @brief	初始化串口数据结构
  * @note 	
  * @param	uartData			数据结构
						buf						数据缓冲区的指针
						buf_length		缓冲区的长度
						timeInterval	判断一帧结束的时间间隔
  * @retva
  */  
void UartData_timeInterval_Init(UartDateStruct* uartData,uint8_t* buf,uint16_t buf_length,uint16_t timeInterval)
{
	uartData->buf = buf;
	uartData->buf_max_length = buf_length;
	uartData->max_countDown_value = timeInterval;
	uartData->length = 0;
	uartData->state = 0;
	uartData->timer = 0;
}
