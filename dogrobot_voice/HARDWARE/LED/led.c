#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_5|GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);

}

void LED_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);

}

void LED_Voice_Control(unsigned char LED)
{
	if(LED == 0)
	{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
	}
	else
	{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_SetBits(GPIOA,GPIO_Pin_15);
	}
}

void LED_Direction_Control(unsigned char LED1,unsigned char LED2,unsigned char LED3,unsigned char LED4,unsigned char LED5)
{
	if(LED1 == 0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
	
	if(LED2 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_12);

	if(LED3 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_13);

	if(LED4 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	if(LED5 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
}

void LED_Move_Control(unsigned char LED1,unsigned char LED2,unsigned char LED3)
{
	if(LED1 == 0)
	{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	}
	else
	{
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
	}

	if(LED2 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_5);

	if(LED3 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
}

