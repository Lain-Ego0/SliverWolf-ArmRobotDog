#include "timer.h"

void PWM_ParameterConfiguration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    // ʹ�ܶ�ʱ����AFIO���ù���ʱ��    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
    // TIM2������ӳ��
    GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);

    // ���ö�ʱ����������
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; // ����Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_Period = 19999; // �Զ���װ��ֵ�����Ԥ��Ƶʵ��PWMƵ��
    TIM_TimeBaseStructure.TIM_Prescaler = 71;  // Ԥ��Ƶ������õ�PWMƵ��
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // ���ظ�����
    // ��ʼ��TIM2, TIM3, TIM4��ʱ���������
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    // ��������Ƚϣ�OC��ģʽ������PWM���
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; // PWMģʽ2
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; // ����ȽϿ���״̬Ϊ�͵�ƽ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; // �������Ϊ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ʹ������Ƚ�״̬	
    TIM_OCInitStructure.TIM_Pulse = 0; // ����ռ�ձȳ�ʼֵΪ0
	// ��ʼ��TIM1����ͨ����OCģʽ
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    // ��ʼ��TIM2����ͨ����OCģʽ
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    // ��ʼ��TIM3����ͨ����OCģʽ
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    // ��ʼ��TIM4����ͨ����OCģʽ
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_OC2Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    // ʹ��Ԥװ�ؼĴ������������ռ�ձȶ��������ӳ�
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
    
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
    // ʹ���Զ���װ��Ԥװ��
	TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//ע��TIM1�����ģʽ
	
    // ������ʱ��
	TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}




