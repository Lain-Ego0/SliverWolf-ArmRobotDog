#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "dog.h"
#include "stmflash.h"
#include "adc.h"
#include "led.h"
#include "timer.h"
#include "stm32f10x.h"

void Roll(float roll,float *X1,float *Y1,float *Z1,float *X2,float *Y2,float *Z2,float *X3,float *Y3,float *Z3,float *X4,float *Y4,float *Z4);
void Pitch(float pitch,float *X1,float *Y1,float *Z1,float *X2,float *Y2,float *Z2,float *X3,float *Y3,float *Z3,float *X4,float *Y4,float *Z4);
void Yaw(float yaw,float *X1,float *Y1,float *Z1,float *X2,float *Y2,float *Z2,float *X3,float *Y3,float *Z3,float *X4,float *Y4,float *Z4);

int main(void)
{ 
	// ��������ͳ�ʼ��
	uint8_t RxDate;
	u8 mode = 0;                // ģʽ
	u8 pattern = 0;
	u8 turn_flag = 0;
	u8 ZH = 10;                 // ���в�̬�����ƶ������ֵ
	
	u8 Max1=Ts/Bj;// �ǲ�̬һ������20ms��ʱ������Ĵ���
	u8 Max2=Ts2/Bj2;//���в�̬һ������20ms��ʱ������Ĵ���
	u8 t1=Max1/6,t2=Max1/2+Max1/6;//�Խǲ�̬�ı�־
	u8 t3=0,t4=Max2/4,t5=Max2/2,t6=Max2-Max2/4;//���в�̬�ı�־

    volatile int MCU0,MCU1,MCU2;//��¼��ȡ�ĽǶ�
	float x1=0,y1=HIGH,z1=0;//��ʼ�������
	float x2=10,y2=HIGH,z2=0;//��ʼ�������
	float x3=10,y3=HIGH,z3=0;//��ʼ�������
	float x4=0,y4=HIGH,z4=0;//��ʼ�������	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	//��Ҫ��ʼ��
	PWM_Init();//PWM��IO��ʼ��
	PWM_ParameterConfiguration();//PWM��ʱ����ʼ��
	uart_init2(9600);	 //���ڳ�ʼ��Ϊ9600�������������տ���ָ��
	LED_Init();
	
	WZ(0,HIGH,0,10,HIGH,0,10,HIGH,0,0,HIGH,0);//վ��
	TIM_SetCompare2(TIM1,1500);
	delay_ms(100);
	TIM_SetCompare4(TIM1,1500);
	delay_ms(100);
	TIM_SetCompare2(TIM1,2000);
	delay_ms(100);
	TIM_SetCompare4(TIM1,2000);
	delay_ms(100);
	//��ˮ�Ʊ�ʾ��ʼ���������
	running_water_lamp(255);	
	running_water_lamp(255);
	running_water_lamp(255);
				
	while(1)
	{
		if(Serial_GetRxFlag() == 1)
		{
			RxDate = Serial_GetRxData();
			Serial_SendByte(RxDate);

			if (RxDate == '0') {
				mode = 0; x1 = 0; y1 = HIGH; z1 = 0;
				x2 = 10; y2 = HIGH; z2 = 0;
				x3 = 10; y3 = HIGH; z3 = 0;
				x4 = 0; y4 = HIGH; z4 = 0;
			} else if (RxDate >= '1' && RxDate <= '9') {
				mode = RxDate - '0';
			} else {
				switch (RxDate) {
					case 'a': mode = 10; break;
					case 'b': mode = 11; break;
					case 'c': mode = 12; break;
					case 'd': mode = 13; break;
					default: break; // ��Ч���봦��
				}
			}

		}
if (turn_flag == 0) {
    pattern = mode;
} else if (turn_flag == 1) {
    pattern = 0; // ���ں�������OpenMVʶ���Զ�ģʽ
}

switch (pattern) {
    case 0: // վ��
        WZ(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
        t1 = Max1 / 6;
        t2 = Max1 / 2 + Max1 / 6;
        t3 = 0;
        t4 = Max2 / 4;
        t5 = Max2 / 2;
        t6 = Max2 - Max2 / 4;
        delay_ms(10);
        break;
    case 1: // ǰ��
        WZ(LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 2: // ����
        WZ(LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 3: // ��ת
        WZ(LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 4: // ��ת
        WZ(LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 5: // ��ƽ��
        WZ(0, HIGH + LUJIN_Y(t2), LUJIN_Z_left(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_left(t1), 0, HIGH + LUJIN_Y(t2), LUJIN_Z_left(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_left(t1));
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 6: // ��ƽ��
        WZ(0, HIGH + LUJIN_Y(t2), LUJIN_Z_right(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_right(t1), 0, HIGH + LUJIN_Y(t2), LUJIN_Z_right(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_right(t1));
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 7: // ��ͷ
        HIGH += 4;
        y1 = y2 = y3 = y4 = HIGH;
        mode = 0;
        break;
    case 8: // ̧ͷ
        HIGH -= 4;
        y1 = y2 = y3 = y4 = HIGH;
        mode = 0;
        break;
    case 9: // ����
        WZ(LUJIN_X3(t4), HIGH + LUJIN_Y3(t4), ZH * sin(2 * Pi * t5 / Max2), LUJIN_X3(t5), HIGH + LUJIN_Y3(t5), ZH * sin(2 * Pi * t5 / Max2), LUJIN_X3(t3), HIGH + LUJIN_Y3(t3), ZH * sin(2 * Pi * t5 / Max2), LUJIN_X3(t6), HIGH + LUJIN_Y3(t6), ZH * sin(2 * Pi * t5 / Max2));
        t3++;
        t4++;
        t5++;
        t6++;
        if (t3 >= Max2) t3 = 0;
        if (t4 >= Max2) t4 = 0;
        if (t5 >= Max2) t5 = 0;
        if (t6 >= Max2) t6 = 0;
        delay_ms(10);
        break;
    case 10: // һ��ȡ��
        TIM_SetCompare2(TIM1, 1500);
        delay_ms(100);
        TIM_SetCompare3(TIM1, 2500);
        delay_ms(100);
        TIM_SetCompare4(TIM1, 1500);
        delay_ms(100);
        TIM_SetCompare2(TIM1, 2000);
        delay_ms(100);
        TIM_SetCompare3(TIM1, 500);
        delay_ms(100);
        TIM_SetCompare4(TIM1, 2000);
        delay_ms(100);
        break;
    case 11: // ��ˮ��
        running_water_lamp(255);
        running_water_lamp(255);
        running_water_lamp(255);
        break;
    case 12: // ����
	{   unsigned int j;
		WZ(15, 90, 0, 15, 30, 0, 15, 30, 0, 15, 90, 30);
		delay_ms(300);
		for (j = 0; j < 5; j++) {
			WZ(-55, 5, 0, 15, 30, 0, 15, 30, 0, 15, 90, 30);
			delay_ms(300);
			WZ(-55, 70, 0, 15, 30, 0, 15, 30, 0, 15, 90, 30);
			delay_ms(300);
		}
        mode = 0;}
        break;
    case 13: // ��λ
        mode = 0;
        HIGH = 70;
        y1 = y2 = y3 = y4 = HIGH;
        WZ(0, HIGH, 0, 10, HIGH, 0, 10, HIGH, 0, 0, HIGH, 0); // վ��
        break;
    default:
        // ��������� pattern ֵ
        break;
}

	}
}


