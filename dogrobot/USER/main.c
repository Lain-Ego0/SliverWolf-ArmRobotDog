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
	// 变量定义和初始化
	uint8_t RxDate;
	u8 mode = 0;                // 模式
	u8 pattern = 0;
	u8 turn_flag = 0;
	u8 ZH = 10;                 // 爬行步态左右移动的最大值
	
	u8 Max1=Ts/Bj;// 角步态一个周期20ms定时器溢出的次数
	u8 Max2=Ts2/Bj2;//爬行步态一个周期20ms定时器溢出的次数
	u8 t1=Max1/6,t2=Max1/2+Max1/6;//对角步态的标志
	u8 t3=0,t4=Max2/4,t5=Max2/2,t6=Max2-Max2/4;//爬行步态的标志

    volatile int MCU0,MCU1,MCU2;//记录读取的角度
	float x1=0,y1=HIGH,z1=0;//初始足端坐标
	float x2=10,y2=HIGH,z2=0;//初始足端坐标
	float x3=10,y3=HIGH,z3=0;//初始足端坐标
	float x4=0,y4=HIGH,z4=0;//初始足端坐标	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	//主要初始化
	PWM_Init();//PWM的IO初始化
	PWM_ParameterConfiguration();//PWM定时器初始化
	uart_init2(9600);	 //串口初始化为9600，用于蓝牙接收控制指令
	LED_Init();
	
	WZ(0,HIGH,0,10,HIGH,0,10,HIGH,0,0,HIGH,0);//站立
	TIM_SetCompare2(TIM1,1500);
	delay_ms(100);
	TIM_SetCompare4(TIM1,1500);
	delay_ms(100);
	TIM_SetCompare2(TIM1,2000);
	delay_ms(100);
	TIM_SetCompare4(TIM1,2000);
	delay_ms(100);
	//流水灯表示初始化进程完成
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
					default: break; // 无效输入处理
				}
			}

		}
if (turn_flag == 0) {
    pattern = mode;
} else if (turn_flag == 1) {
    pattern = 0; // 用于后续开发OpenMV识别自动模式
}

switch (pattern) {
    case 0: // 站立
        WZ(x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
        t1 = Max1 / 6;
        t2 = Max1 / 2 + Max1 / 6;
        t3 = 0;
        t4 = Max2 / 4;
        t5 = Max2 / 2;
        t6 = Max2 - Max2 / 4;
        delay_ms(10);
        break;
    case 1: // 前进
        WZ(LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 2: // 后退
        WZ(LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 3: // 左转
        WZ(LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 4: // 右转
        WZ(LUJIN_X(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X(t1), HIGH + LUJIN_Y(t1), 0, LUJIN_X2(t2), HIGH + LUJIN_Y(t2), 0, LUJIN_X2(t1), HIGH + LUJIN_Y(t1), 0);
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 5: // 左平移
        WZ(0, HIGH + LUJIN_Y(t2), LUJIN_Z_left(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_left(t1), 0, HIGH + LUJIN_Y(t2), LUJIN_Z_left(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_left(t1));
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 6: // 右平移
        WZ(0, HIGH + LUJIN_Y(t2), LUJIN_Z_right(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_right(t1), 0, HIGH + LUJIN_Y(t2), LUJIN_Z_right(t2), 0, HIGH + LUJIN_Y(t1), LUJIN_Z_right(t1));
        t1++;
        t2++;
        if (t1 >= Max1) t1 = 0;
        if (t2 >= Max1) t2 = 0;
        delay_ms(10);
        break;
    case 7: // 低头
        HIGH += 4;
        y1 = y2 = y3 = y4 = HIGH;
        mode = 0;
        break;
    case 8: // 抬头
        HIGH -= 4;
        y1 = y2 = y3 = y4 = HIGH;
        mode = 0;
        break;
    case 9: // 爬行
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
    case 10: // 一键取物
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
    case 11: // 流水灯
        running_water_lamp(255);
        running_water_lamp(255);
        running_water_lamp(255);
        break;
    case 12: // 握手
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
    case 13: // 复位
        mode = 0;
        HIGH = 70;
        y1 = y2 = y3 = y4 = HIGH;
        WZ(0, HIGH, 0, 10, HIGH, 0, 10, HIGH, 0, 0, HIGH, 0); // 站立
        break;
    default:
        // 处理意外的 pattern 值
        break;
}

	}
}


