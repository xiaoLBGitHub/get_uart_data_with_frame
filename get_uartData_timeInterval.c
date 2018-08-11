#include "get_uartData_timeInterval.h"

/*
  ʹ�÷�����1���½� UartDateStruct �ṹ�壬��һ��uint8_t �������飬����UartData_timeInterval_Init ��ʼ��
            2��ȷ����ʱ�ȴ�ʱ�� timeInterval
            3�������� USART_IRQHandler �����Ӧ�Ĵ���RXNE�ж���
            4��CatchUartDataInSystickHandle ���˺�������δ��ж��е�1ms�ж���
            5���� CatchUartDataInSystickHandle ����ֵ��Ϊ0ʱ����Ϊ�յ�1������֡�����Դӻ�������ȡlen���ֽ�

*/
#define COUNT_DOWN_DEFAULT	0xFFFF
/**
  * @brief	�ڵδ��ж��м�⴮���Ƿ��������ݵ���
  * @note 	
  * @param	uartData	�������ݽṹ
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
  * @brief	�ڵδ��ж��У���֡�������ʱ����Ϊ0ʱ���ж��Ƿ��յ����ݡ�
  * @note 	
  * @param	uartData���������ݽṹ
  * @retva	���յ�������һ֡ʱ������һ֡���ݵĳ��ȣ���û�����ݻ�����Ϊһ֡û�н���ʱ������0
  */  

uint16_t CatchUartDataInSystickHandle(UartDateStruct* uartData)
{
	uint16_t len=0;
	if(uartData->timer == 0)					//�ж��Ƿ񵽹涨��ʱ����
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
	else	//û���յ����ݵ�ʱ��������
	{
		return 0;
	}
}
/**
  * @brief	���ڴ����ж��У������յ������ݷŵ����ݽṹ��
  * @note 	
  * @param	uartData	�������ݽṹ
						data			���ڽ��ռĴ����е�����
  * @retva
  */     
void USART_IRQHandler(UartDateStruct* uartData,uint8_t data)
{
	if(uartData->length >= uartData->buf_max_length) //���ж��н������ݣ���ֹ�������
	{
		uartData->length = 0;
	}
	uartData->buf[uartData->length] = data;
	uartData->length++;
	uartData->timer = uartData->max_countDown_value;
}

/**
  * @brief	��ʼ���������ݽṹ
  * @note 	
  * @param	uartData			���ݽṹ
						buf						���ݻ�������ָ��
						buf_length		�������ĳ���
						timeInterval	�ж�һ֡������ʱ����
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
