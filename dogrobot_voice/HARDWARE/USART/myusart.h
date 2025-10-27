#ifndef __US_H
#define __US_H	 
#include "sys.h"



#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define USART3_REC_LEN  			200  	//定义最大接收字节数 200


extern u16 test;
extern u16 status;

extern unsigned char LED_flag_twice;
extern unsigned char LED_flag_third;

void My_USART1_Init(void);
void My_USART2_Init(void);
void My_USART3_Init(void);
void USART1_SENDDATA(u8 *USART1_SendDATA,u16 SIZE);

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

extern u8  USART3_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记	

extern u8 *data;
		 				    
#endif
