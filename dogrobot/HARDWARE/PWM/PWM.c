//此部分为GPIO初始化
#include "pwm.h"

void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
								   GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10|
								   GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1,ENABLE);//注意复用！！！！！
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|
								   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}









