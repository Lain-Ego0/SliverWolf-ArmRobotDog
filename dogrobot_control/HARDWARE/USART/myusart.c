#include "myusart.h"

////////////////////////////////////////BLUETOOTH/////////////////////////////////////////////////
u8 USART_RX_BUF[USART_REC_LEN];
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	
u8 *data;

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
	else if(status == 102)  //ɢ��
		data = "[A]";
	USART1_SENDDATA(data,3);
}

/////////////////////////////////////////////����2����////////////////////////////////////////////////
void My_USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
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
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 
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


////////////////////////////////////////attitude sensor initialization/////////////////////////////////////////////////
void My_USART3_Init(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
	
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.9
	//USART3_RX	  PB11
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
	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);
}

//�����ǿ���
int16_t YAW=0,PITCH=0,ROLL=0;
float YAW_Angle=0,PITCH_Angle=0,ROLL_Angle=0;
uint8_t flag=0;
void USART3_IRQHandler(void)                	//����1�жϷ������
{
	static uint8_t k=0,rebuf[8]={0};
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		rebuf[k++] =USART_ReceiveData(USART3);	//��ȡ���յ�������
		if(!(rebuf[0]==0xaa))//���֡ͷ�����建��
		{
			k=0;
			rebuf[0]=0;
		}
		if(k==8)//���ݽ������
		{
			if(rebuf[7]==0x55)//�ж�֡β����ȷ�������̬��
			{
				YAW=(rebuf[1]<<8|rebuf[2]);//YAW��PITCH,ROLLΪ��ʵֵ��100��
				PITCH=(rebuf[3]<<8|rebuf[4]);
				ROLL=(rebuf[5]<<8|rebuf[6]);
				
				YAW_Angle = (float)YAW/100.0;
				PITCH_Angle = (float)PITCH/100.0;
				ROLL_Angle = (float)ROLL/100.0;
//				flag=1;
			}
			k=0;//�建��
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
	TIM_TimeBaseStructure.TIM_Period = 999;
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
			if(PITCH_Angle<=-40)
				Voice_Status_Send_Bluetooth(49);
			else if(PITCH_Angle>=40)
				Voice_Status_Send_Bluetooth(50);
			else if(ROLL_Angle<=-40)
				Voice_Status_Send_Bluetooth(54);
			else if(ROLL_Angle>=40)
				Voice_Status_Send_Bluetooth(53);
			else
				Voice_Status_Send_Bluetooth(55);
				
		}
}
