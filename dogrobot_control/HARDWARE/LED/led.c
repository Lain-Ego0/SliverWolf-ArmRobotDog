#include "led.h"

/**
* @name   LED_Init
* @brief  Initializes the LEDs connected to GPIOB and GPIOA pins
* @param  None
* @retval None
* @detail This function initializes GPIO pins PB12, PB13, PB14, PB15, and PA12 
*         for LED control. It configures these pins as output push-pull with 
*         a speed of 50MHz. For GPIOB pins (PB12 to PB15), the LEDs are initially
*         turned off. For GPIOA pin (PA12), the LED is initially turned on.
*/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}

void LED_Control(unsigned char LED1,unsigned char LED2,unsigned char LED3,unsigned char LED4,unsigned char LED5)
{
	if(LED1 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_12);

	if(LED2 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_13);

	if(LED3 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	if(LED4 == 0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
	
	if(LED5 == 0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
}
