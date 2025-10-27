#ifndef __US_H
#define __US_H	 
#include "sys.h"



#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define USART3_REC_LEN  			200  	//�����������ֽ��� 200


extern u16 test;
extern u16 status;

extern unsigned char LED_flag_twice;
extern unsigned char LED_flag_third;

void My_USART1_Init(void);
void My_USART2_Init(void);
void My_USART3_Init(void);
void USART1_SENDDATA(u8 *USART1_SendDATA,u16 SIZE);

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern u8  USART3_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬���	

extern u8 *data;
		 				    
#endif
