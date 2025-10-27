#include "stm32f10x.h"
#include "pwm.h"
#include "myusart.h"
#include "delay.h"
#include "math.h"
#include "key.h"
#include "led.h"
#include "stdio.h"

int main(void)
{	
//	unsigned char flag;
	unsigned char key_flag = 0;
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	My_USART1_Init();
	My_USART3_Init();	
//	TIM2_Init();
	Key_Init();
	LED_Init();
	
	while(1)
	{
		
		key_flag = Key_Scan(0);
	    if(key_flag)
		{						   
			switch(key_flag)
			{				 
				case key_P7:
					data = "[0]";
					USART1_SENDDATA(data,3);
					break; 
				case key_P8: 
					data = "[M]";
					USART1_SENDDATA(data,3);
					break;
				case key_P9: 
					data = "[5]";
					USART1_SENDDATA(data,3);
					break;
				case key_P10: 
					data = "[6]";
					USART1_SENDDATA(data,3);
					break;
			}
		}
		else 
			delay_ms(5); 

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

		if(USART_RX_STA&0x8000)//如果接收到控制指令
		{
			if(USART_RX_BUF[0]=='1')
			{
				LED_Control(1,0,0,0,1);	
			}
			else if(USART_RX_BUF[0]=='2')
			{
				LED_Control(0,1,0,0,1);			
			}
			else if(USART_RX_BUF[0]=='3')
			{
				LED_Control(0,0,1,0,1);				
			}
			else if(USART_RX_BUF[0]=='4')
			{
				LED_Control(0,0,0,1,1);				
			}
			USART_RX_STA=0;
		}
	}
}

