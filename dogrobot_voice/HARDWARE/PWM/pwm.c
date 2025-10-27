#include "pwm.h"

/**
* @name   My_TIM1_Init
* @brief  ��ʼ��TIM1ΪPWM���ģʽ
* @param  arr: �Զ���װ�ؼĴ�����ֵ������PWMƵ��
* @param  psc: Ԥ��Ƶ����ֵ��������������ʱ��Ƶ��
* @retval ��
* @detail �˺�����PA8��PA11����ΪTIM1��PWM��������ڲ���PWM�źš�
*/
void My_TIM1_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	TIM_OCInitTypeDef TIM_OCInitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	TIM_TimeBaseInitStructure.TIM_Period = arr;

	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=10;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=10;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OC4Init(TIM1,&TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
/**
* @name   My_TIM3_Init
* @brief  ��ʼ��TIM3ΪPWM���ģʽ
* @param  arr: �Զ���װ�ؼĴ�����ֵ������PWMƵ��
* @param  psc: Ԥ��Ƶ����ֵ��������������ʱ��Ƶ��
* @retval ��
* @detail �˺�����PA6��PA7����ΪTIM3ͨ��1��2��PWM��������ڲ���PWM�źš�
*/
void My_TIM3_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;

	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;

	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM3,&TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);        
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_Cmd(TIM3,ENABLE);       
	
	//	TIM_SetCompare2(TIM2,1500);
	//	TIM_SetCompare3(TIM2,2000);

}
/**
* @name   My_TIM5_Init
* @brief  ��ʼ��TIM5ΪPWM���ģʽ
* @param  arr: �Զ���װ�ؼĴ�����ֵ������PWMƵ��
* @param  psc: Ԥ��Ƶ����ֵ��������������ʱ��Ƶ��
* @retval ��
* @detail �˺�����PA2��PA3����ΪTIM5��PWM��������ڲ���PWM�źš�
*/
void My_TIM5_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
	TIM_OC3Init(TIM5,&TIM_OCInitStruct);
	TIM_OC4Init(TIM5,&TIM_OCInitStruct);
	
	TIM_OC3PreloadConfig(TIM5,TIM_OCPreload_Enable);       
	TIM_OC4PreloadConfig(TIM5,TIM_OCPreload_Enable);
	TIM_Cmd(TIM5,ENABLE);        
	TIM_CtrlPWMOutputs(TIM2, ENABLE);
	
	//	TIM_SetCompare2(TIM2,1500);
	//	TIM_SetCompare3(TIM2,2000);

}




