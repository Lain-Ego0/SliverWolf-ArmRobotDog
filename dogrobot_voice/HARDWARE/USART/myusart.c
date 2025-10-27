#include "myusart.h"
#include "led.h"

////////////////////////////////////////BLUETOOTH/////////////////////////////////////////////////
u16 test = 0;
u16 status = 55;
u8 USART_RX_BUF[USART_REC_LEN];
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���

u8  USART3_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART3_RX_STA;         		//����״̬���	

u8 *data;

unsigned char LED_flag_twice = 0;
unsigned char LED_flag_third = 0;


void My_USART1_Init(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(Res=='[')
			{
				USART_RX_STA|=0x4000;
			}
			else if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res==']')
				{
					USART_RX_STA|=0x8000;	//��������� 
				}
				else //��û�յ�]
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
				}
			}
		}   		 
	} 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}

void USART1_SENDDATA(u8 *USART1_SendDATA,u16 SIZE)
{
	u16 i = 0;
	for(i = 0;i<SIZE; i++)
	{
		USART_SendData(USART1,USART1_SendDATA[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//�������ˣ���־λ��1����ʱ�жϡ�������1��==0 �Ϳ��ԾͿ���
	}
}

void Voice_Status_Send_Bluetooth(u8 status)
{
	if(status == 49)  //ǰ��
		data = "[1]";
	else if(status == 50)  //����
		data = "[2]";
	else if(status == 51)  //�����ƶ�
		data = "[7]";
	else if(status == 52)  //�����ƶ�
		data = "[8]";
	else if(status == 53)  //��ת
		data = "[3]";
	else if(status == 54)  //��ת
		data = "[4]";
	else if(status == 55)  //վ��
		data = "[X]";
	else if(status == 98)  //����
		data = "[M]";
	else if(status == 99)  //ԭ��̤��
		data = "[N]";
	else if(status == 100)  //����ҡ��
		data = "[B]";
	else if(status == 101)  //ǰ��ҡ��
		data = "[V]";
	else if(status == 102)  //ɢ��
		data = "[A]";
	
	USART1_SENDDATA(data,3);
}


void My_USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1                 //ʹ�ܴ���1 
}	



void USART2_IRQHandler(void)
{
	u8 ress; 
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
		ress = USART_ReceiveData(USART2); 
    USART_SendData(USART2,ress);
		if(ress =='0')
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		}
		else
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		}
	}
}

u8 USART_RX_BUF3[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART_RX_STA3=0;       //����״̬���	 


////////////////////////////////////////VOICE/////////////////////////////////////////////////

void My_USART3_Init(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);
}


void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		test++;	
		Res = USART_ReceiveData(USART3);	//��ȡ���յ�������
		if((test%3) == 2)
		{
			status = Res;		
			Voice_Status_Send_Bluetooth(status);
		}
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(Res=='[')
			{
				USART3_RX_STA|=0x4000;
			}
			else if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res==']')
				{
					USART3_RX_STA|=0x8000;	//��������� 
				}
				else //��û�յ�
				{
					USART3_RX_BUF[0]=Res;
					//USART3_RX_STA++;
				}
			}
		}   		 
	} 
}

/**
  * @brief  ��ʱ��2�ĳ�ʼ��
  */

void TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the TIM2 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2499;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	/* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־ 
			LED_flag_twice++;
			LED_flag_third++;
			if(LED_flag_twice == 2)LED_flag_twice = 0;
			if(LED_flag_third == 3)LED_flag_third = 0;
			if(status == 48) //�ؼ���
			{
				LED_Direction_Control(0,0,0,0,0);
				LED_Move_Control(0,0,0);
				if(LED_flag_twice)
					LED_Voice_Control(1);
				else
					LED_Voice_Control(0);
			}
			else if(status == 49)  //ǰ��
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,1,0,0,0);
				LED_Move_Control(0,0,0);
			}
			else if(status == 50)  //����
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,0,0,1,0);
				LED_Move_Control(0,0,0);
			}
			else if(status == 51)  //�����ƶ�
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,0,0,0,1);
				LED_Move_Control(0,0,0);
			}
			else if(status == 52)  //�����ƶ�
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,0,1,0,0);
				LED_Move_Control(0,0,0);
			}
			else if(status == 53)  //��ת
			{
				LED_Voice_Control(0);
				if(LED_flag_third == 0)
				{
					LED_Direction_Control(0,1,0,0,0);
					LED_Move_Control(0,0,0);
				}
				else if(LED_flag_third == 1)
				{
					LED_Direction_Control(0,0,0,0,1);
					LED_Move_Control(0,0,0);
				}
				else
				{
					LED_Direction_Control(0,0,0,1,0);
					LED_Move_Control(0,0,0);
				}
			}
			else if(status == 54)  //��ת
			{
				LED_Voice_Control(0);
				if(LED_flag_third == 0)
				{
					LED_Direction_Control(0,1,0,0,0);
					LED_Move_Control(0,0,0);
				}
				else if(LED_flag_third == 1)
				{
					LED_Direction_Control(0,0,1,0,0);
					LED_Move_Control(0,0,0);
				}
				else
				{
					LED_Direction_Control(0,0,0,1,0);
					LED_Move_Control(0,0,0);
				}
			}
			else if(status == 55)  //վ��
			{
				LED_Voice_Control(0);
				LED_Direction_Control(1,0,0,0,0);
				LED_Move_Control(0,0,0);
			}
			else
			{
				LED_Voice_Control(0);
				if(LED_flag_third == 0)
				{
					LED_Direction_Control(0,0,0,0,0);
					LED_Move_Control(1,0,0);
				}
				else if(LED_flag_third == 1)
				{
					LED_Direction_Control(0,0,0,0,0);
					LED_Move_Control(0,1,0);
				}
				else
				{
					LED_Direction_Control(0,0,0,0,0);
					LED_Move_Control(0,0,1);
				}
			}


			
		}
}
