# get_uart_data_with_frame
====

程序包含 get_uartData_timeInterval.c 与 get_uartData_timeInterval.h 两个文件。
test.c文件为示例文件。

本文件用于单片机通过USART（UART）接收数据。

## 原理

通过串口接收中断接收数据，并通过滴答中断判断有多久没有收到新的数据。若超过一定时间没有收到新的数据则认为一帧数据结束。

## 用法

1. 创建一个数据结构为UartDateStruct的变量，并开辟接收数据的缓冲区；
2. 调用数据结构初始化函数UartData_timeInterval_Init()，将UartDateStruct变量与缓冲区绑定。并确定多久没有收到数据，认为一帧的数据结束。例如：在115200的波特率下，5ms没有收到数据就可以认为一帧数据接收，在9600的波特率下10ms没有收到新的数据就可以认为一帧的数据结束。
3. 将串口中断处理函数USART_IRQHandler放入USART的接收中断中。
4. 将超时判断函数CatchUartDataInSystickHandle放入滴答中断中。
5. 在滴答中断中判断CatchUartDataInSystickHandle()函数的返回值，当返回值不为0时，接收到一帧的数据，返回值为这一帧数据的长度。

## 示例

请参考test.c文件。这个例程是以STM32F103微处理器为基础，使用USART1作为通讯接口。接收到一帧数据后在原样发送回去。
