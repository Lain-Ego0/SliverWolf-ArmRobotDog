#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define key_P7_read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define key_P8_read  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)
#define key_P9_read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define key_P10_read  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)

#define key_P7 	7
#define key_P8	8
#define key_P9 	9
#define key_P10	10

void Key_Init(void);
unsigned char Key_Scan(unsigned char);		


#endif
