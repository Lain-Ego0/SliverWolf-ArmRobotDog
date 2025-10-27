#include "key.h"
#include "delay.h"


//key1  P7-B1
//key2	P8-B0
//key3  P9-A7
//key4	P10-A6

void Key_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


unsigned char Key_Scan(unsigned char mode)
{
	static u8 key_up=1;
	if(mode)
		key_up=1;
	if(key_up && (key_P7_read == 0 || key_P8_read == 0 || key_P9_read == 0 || key_P10_read == 0))
	{
		delay_ms(10);
		key_up=0;
		if(key_P7_read == 0)
			return key_P7;
		else if(key_P8_read == 0)
			return key_P8;
		if(key_P9_read == 0)
			return key_P9;
		else if(key_P10_read == 0)
			return key_P10;
	}
	else if(key_P7_read == 1 && key_P8_read ==1 && key_P9_read ==1 && key_P10_read ==1)
		key_up=1;
 	return 0;
}

