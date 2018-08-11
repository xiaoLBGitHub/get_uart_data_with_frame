#ifndef	 _GET_UARTDATA_TIMEINTERVAL_H
#define	 _GET_UARTDATA_TIMEINTERVAL_H

#include "stm32f10x.h"


typedef struct
{
	uint8_t* buf;					//ָ�����ݻ�����

	uint16_t length; 					//��ӳ�������ݳ���
	uint16_t state; 						//ָʾһ֡���� ��0��ʾû����������ֵ��ʾ���ݳ���
	uint16_t timer ;						//���ڵδ��ж��еĵ���ʱ
	
	uint16_t max_countDown_value;		//���ݵδ��жϵļ�����ж�һ֡������ʱ����
	uint16_t buf_max_length;	//�洢������󳤶�
}UartDateStruct;

extern void UartData_timeInterval_Init(UartDateStruct* uartData,uint8_t* buf,uint16_t buf_length,uint16_t timeInterval);
extern void USART_IRQHandler(UartDateStruct* uartData,uint8_t data);
extern uint16_t CatchUartDataInSystickHandle(UartDateStruct* uartData);
#endif
