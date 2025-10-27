//江科大蓝牙模块
#include "sys.h"
#include "usart.h"	  
#include "stm32f10x.h"                  
#include "stdarg.h"

uint8_t Serial_RxData;
uint8_t Serial_RxFlag;

void uart_init2(u32 bound){
    // GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   // 使能USART2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    // 使能GPIOA时钟
    
    // USART2_TX   GPIOA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA.2
    
    // USART2_RX   GPIOA3初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIOA.3  
    
    // Usart2 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); // 根据指定的参数初始化VIC寄存器
    
    // USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound; // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1; // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No; // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
    USART_Init(USART2, &USART_InitStructure); // 初始化串口2
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启串口接受中断
    USART_Cmd(USART2, ENABLE); // 使能串口2
}

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        Serial_RxData = USART_ReceiveData(USART2);
        Serial_RxFlag = 1;
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

