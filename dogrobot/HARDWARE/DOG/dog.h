#ifndef __DOG_H
#define __DOG_H
#include "sys.h"
#include <math.h>
#include "timer.h"
#include "pwm.h"

#define Pi 3.1415926

extern u8 HIGH; //վ���ƶ�ʱ����ĸ߶�
extern u8 HIGHing; //վ���ƶ�ʱ����ĸ߶�

extern int flag1;//��ʱ������20ms���˵ı�־
extern int flag_500ms;
extern int flag_PA8;
extern int flag_PA11;

extern u8 XH[12];
extern u8 datatemp[12];
extern u8 location[12];
extern float Bj;//���Ƽ��ʱ��
extern float Ts;//����
extern float Bj2;//���Ƽ��ʱ��
extern float Ts2;//����

int LUJIN_X(u8 t);
int LUJIN_Y(u8 t);
int LUJIN_X2(u8 t);
int LUJIN_X3(u8 t);
int LUJIN_Y3(u8 t);
int LUJIN_Z_left(u8 t);
int LUJIN_Z_right(u8 t);
void WZ(int x1,int y1,int z1,int x2,int y2,int z2,int x3,int y3,int z3,int x4,int y4,int z4);
 
#endif
