#include "myusart.h"
#include "led.h"

////////////////////////////////////////BLUETOOTH/////////////////////////////////////////////////
u16 test = 0;
u16 status = 55;
u8 USART_RX_BUF[USART_REC_LEN];
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记

u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART3_RX_STA;         		//接收状态标记	

u8 *data;

unsigned char LED_flag_twice = 0;
unsigned char LED_flag_third = 0;


void My_USART1_Init(void)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART1_RX	  GPIOA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(Res=='[')
			{
				USART_RX_STA|=0x4000;
			}
			else if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res==']')
				{
					USART_RX_STA|=0x8000;	//接收完成了 
				}
				else //还没收到]
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
				}
			}
		}   		 
	} 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}

void USART1_SENDDATA(u8 *USART1_SendDATA,u16 SIZE)
{
	u16 i = 0;
	for(i = 0;i<SIZE; i++)
	{
		USART_SendData(USART1,USART1_SendDATA[i]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);//发送完了，标志位置1，此时判断“不等于1”==0 就可以就可以
	}
}

void Voice_Status_Send_Bluetooth(u8 status)
{
	if(status == 49)  //前进
		data = "[1]";
	else if(status == 50)  //后退
		data = "[2]";
	else if(status == 51)  //左向移动
		data = "[7]";
	else if(status == 52)  //右向移动
		data = "[8]";
	else if(status == 53)  //左转
		data = "[3]";
	else if(status == 54)  //右转
		data = "[4]";
	else if(status == 55)  //站立
		data = "[X]";
	else if(status == 98)  //握手
		data = "[M]";
	else if(status == 99)  //原地踏步
		data = "[N]";
	else if(status == 100)  //左右摇摆
		data = "[B]";
	else if(status == 101)  //前后摇摆
		data = "[V]";
	else if(status == 102)  //散步
		data = "[A]";
	
	USART1_SENDDATA(data,3);
}


void My_USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART2, ENABLE);                    //使能串口1                 //使能串口1 
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

u8 USART_RX_BUF3[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA3=0;       //接收状态标记	 


////////////////////////////////////////VOICE/////////////////////////////////////////////////

void My_USART3_Init(void)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_Cmd(USART3, ENABLE);
}


void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		test++;	
		Res = USART_ReceiveData(USART3);	//读取接收到的数据
		if((test%3) == 2)
		{
			status = Res;		
			Voice_Status_Send_Bluetooth(status);
		}
		if((USART3_RX_STA&0x8000)==0)//接收未完成
		{
			if(Res=='[')
			{
				USART3_RX_STA|=0x4000;
			}
			else if(USART3_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res==']')
				{
					USART3_RX_STA|=0x8000;	//接收完成了 
				}
				else //还没收到
				{
					USART3_RX_BUF[0]=Res;
					//USART3_RX_STA++;
				}
			}
		}   		 
	} 
}

/**
  * @brief  定时器2的初始化
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


void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
			LED_flag_twice++;
			LED_flag_third++;
			if(LED_flag_twice == 2)LED_flag_twice = 0;
			if(LED_flag_third == 3)LED_flag_third = 0;
			if(status == 48) //关键词
			{
				LED_Direction_Control(0,0,0,0,0);
				LED_Move_Control(0,0,0);
				if(LED_flag_twice)
					LED_Voice_Control(1);
				else
					LED_Voice_Control(0);
			}
			else if(status == 49)  //前进
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,1,0,0,0);
				LED_Move_Control(0,0,0);
			}
			else if(status == 50)  //后退
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,0,0,1,0);
				LED_Move_Control(0,0,0);
			}
			else if(status == 51)  //左向移动
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,0,0,0,1);
				LED_Move_Control(0,0,0);
			}
			else if(status == 52)  //右向移动
			{
				LED_Voice_Control(0);
				LED_Direction_Control(0,0,1,0,0);
				LED_Move_Control(0,0,0);
			}
			else if(status == 53)  //左转
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
			else if(status == 54)  //右转
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
			else if(status == 55)  //站立
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
