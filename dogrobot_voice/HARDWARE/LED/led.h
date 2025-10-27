#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


void LED_Init(void);
void LED_IO_Init(void);
void LED_Voice_Control(unsigned char LED);
void LED_Direction_Control(unsigned char LED1,unsigned char LED2,unsigned char LED3,unsigned char LED4,unsigned char LED5);
void LED_Move_Control(unsigned char LED1,unsigned char LED2,unsigned char LED3);
		 				    
#endif
