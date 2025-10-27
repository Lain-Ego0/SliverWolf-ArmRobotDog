#ifndef __US_H
#define __US_H	 
#include "sys.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200

void My_USART1_Init(void);
void My_USART2_Init(void);
void My_USART3_Init(void);
void TIM2_Init(void);

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern u8  USART_RX_BUF3[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA3;         		//����״̬���	

void USART1_SENDDATA(u8 *USART1_SendDATA,u16 SIZE);
void Voice_Status_Send_Bluetooth(u8 status);

extern int16_t YAW,PITCH,ROLL;
extern uint8_t flag;
extern float YAW_Angle,PITCH_Angle,ROLL_Angle;

extern u8 *data;
		 				    
#endif
