#include "pc_communication.h"


#define BUF_LEN           100
#define MAX_TIMEOUT_TIME  10  //10ms

UartDateStruct Pc_com;
uint8_t Pc_com_buf[BUF_LEN];
uint8_t Pc_com_mirror_buf[BUF_LEN];    

uint8_t Recv_flag = 0;
void init_pc_communication()
{
  UartData_timeInterval_Init(&Pc_com,Pc_com_buf,BUF_LEN,MAX_TIMEOUT_TIME);
}

void USART1_IRQHandler(void)
{
  uint8_t data;
  
  if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
    data = USART_ReceiveData(USART1);
    USART_IRQHandler(&Pc_com,data);
  }
}

void SysTick_Handler(void)
{
  uint16_t recv_len;
  recv_len = CatchUartDataInSystickHandle(&Pc_com);
  if(recv_len != 0){
    memcpy(Pc_com_mirror_buf,Pc_com_buf,recv_len);
    Recv_flag = recv_len;
  }
}

void usart_send_data(uint8_t *send_data,uint8_t len)
{
  uint8_t i;
  for(i=0;i<len;i++){
    USART_SendData(USART1,send_data[i])
  }
}

int main()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 
  USART_Cmd(USART1, ENABLE); 

  SysTick_Config(SystemCoreClock / Systicks);
  
  while(1){
    if(Recv_flag!= 0){
      usart_send_data(Pc_com_mirror_buf,Recv_flag);
      Recv_flag = 0;
    }
  }
}



