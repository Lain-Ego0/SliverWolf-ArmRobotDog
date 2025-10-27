#include "dog.h"

int flag1=0;//��ʱ������20ms���˵ı�־
int flag_500ms = 0;
int flag_PA8=0;
int flag_PA11=0;
u8 HIGH=70; //վ��ʱ�ĸ�
u8 HIGHing = 70;
float Ts=0.5;//����
float fai=0.4;//�ڿ���ռ�ձ�
int xs=30;//���xλ��
int xf=-30;//�յ�xλ��

int zs_1=-15;//���xλ��
int zf_1=15;//�յ�xλ��
int zs_2=15;//���xλ��
int zf_2=-15;//�յ�xλ��

int xsh=25;//���xλ��
int xfh=-25;//�յ�xλ��
int zs=0;//���zλ��
int h=20;//̧�ȸ߶�
float Bj = 0.020;//���Ƽ��ʱ��

float Ts2=1.2;//����
float fai2=0.2;//�ڿ���ռ�ձ�
int xs2=40;//���xλ��
int xf2=-20;//�յ�xλ��
int zs2=-10;//���zλ��
int h2=30;//̧�ȸ߶�
float Bj2 = 0.025;//���Ƽ��ʱ��

u8 datatemp[12]={18,45,40,
				 40,15,30,
                 38,50,40,
                 40,35,30};//������е�ƫ�������ǵ��ֶ����Σ�ÿ����װ��ǵ��޸�

u8 location[12]={15,70,0,
				 15,70,0,
				 15,70,0,
				 15,70,0};//

void WZ(int x1,int y1,int z1,int x2,int y2,int z2,int x3,int y3,int z3,int x4,int y4,int z4)//���������������PWM
{
	float bb,x,y,z;
	u8 a1,b1,c1;
	u8 a2,b2,c2;
	u8 a3,b3,c3;
	u8 a4,b4,c4;
	x=x1;
	y=y1;
	z=-z1;
	if(z>0)
		c1=atan(y/z)*180/Pi;
	if(z<0)
		c1=180+atan(y/z)*180/Pi;
	if(z==0)
		c1=90;
	bb=(5000-(sqrt(x*x+y*y+z*z)-8)*(sqrt(x*x+y*y+z*z)-8))/5000;
	b1=180-acos(bb)*180/Pi;
	if(x>0)
		a1=atan((y-8*cos(c1))/x)*180/Pi-b1/2;
	if(x<0)
		a1=180+atan((y-8*cos(c1))/x)*180/Pi-b1/2;
	if(x==0)
		a1=90-b1/2;
	TIM_SetCompare3(TIM2,(45+180-c1+datatemp[0]-30)*11);	//P11 PB10
	TIM_SetCompare2(TIM2,(45+180-a1+datatemp[1]-30)*11);	//P6 PA1
	TIM_SetCompare1(TIM2,(45+b1+datatemp[2]-30)*11);	//P1 PA0
	
	
	x=x2;
	y=y2;
	z=z2;
	if(z>0)
	c2=atan(y/z)*180/Pi;
	if(z<0)
	c2=180+atan(y/z)*180/Pi;
	if(z==0)
	c2=90;
	bb=(5000-(sqrt(x*x+y*y+z*z)-8)*(sqrt(x*x+y*y+z*z)-8))/5000;
	b2=180-acos(bb)*180/Pi;
	if(x>0)
	a2=atan((y-8*cos(c2))/x)*180/Pi-b2/2;
	if(x<0)
	a2=180+atan((y-8*cos(c2))/x)*180/Pi-b2/2;
	if(x==0)
	a2=90-b2/2;
	TIM_SetCompare4(TIM2,(45+c2+datatemp[3]-30)*11);	//P15 PB11
	TIM_SetCompare1(TIM3,(45+180-a2+datatemp[4]-30)*11);	//P18 PA6
	TIM_SetCompare3(TIM3,(45+b2+datatemp[5]-30)*11);	//P4 PB0
	

	
	x=x3;
	y=y3;
	z=-z3;
	if(z>0)
	c3=atan(y/z)*180/Pi;
	if(z<0)
	c3=180+atan(y/z)*180/Pi;
	if(z==0)
	c3=90;
	bb=(5000-(sqrt(x*x+y*y+z*z)-8)*(sqrt(x*x+y*y+z*z)-8))/5000;
	b3=180-acos(bb)*180/Pi;
	if(x>0)
	a3=atan((y-8*cos(c3))/x)*180/Pi-b3/2;
	if(x<0)
	a3=180+atan((y-8*cos(c3))/x)*180/Pi-b3/2;
	if(x==0)
	a3=90-b3/2;
	TIM_SetCompare1(TIM4,(45+180-c3+datatemp[6]-30)*11);	//P12 PB6
	TIM_SetCompare4(TIM3,(45+a3+datatemp[7]-30)*11);	//P7 PB1
	TIM_SetCompare2(TIM3,(45+180-b3+datatemp[8]-30)*11);	//P2 PA7
	
	
	x=x4;
	y=y4;
	z=z4;
	if(z>0)
	c4=atan(y/z)*180/Pi;
	if(z<0)
	c4=180+atan(y/z)*180/Pi;
	if(z==0)
	c4=90;
	bb=(5000-(sqrt(x*x+y*y+z*z)-8)*(sqrt(x*x+y*y+z*z)-8))/5000;
	b4=180-acos(bb)*180/Pi;
	if(x>0)
	a4=atan((y-8*cos(c4))/x)*180/Pi-b4/2;
	if(x<0)
	a4=180+atan((y-8*cos(c4))/x)*180/Pi-b4/2;
	if(x==0)
	a4=90-b4/2;
	TIM_SetCompare2(TIM4,(45+c4+datatemp[9]-30)*11);	//P16 PB7
	TIM_SetCompare3(TIM4,(45+a4+datatemp[10]-30)*11);	//P19 PB8
	TIM_SetCompare4(TIM4,(45+180-b4+datatemp[11]-30)*11);	//P3 PB9
	
}

int LUJIN_Y(u8 t)//�Խǲ�̬Y���경̬���ɺ���
{
	int y;
	float num,signa;
	num=t*Bj;
	if(num<=Ts*fai)
	{
		signa=2*Pi*num/(fai*Ts);//����signaֵ
		y=h*(1-cos(signa))/2+zs;//����ʱ�̼���y����ɢ��
	}else y=0;
	return -y;
}

int LUJIN_X(u8 t)//�Խǲ�̬X����ǰ����̬���ɺ���
{
	int x;
	float num,signa;
	num=t*Bj;
	if(num<=Ts*fai)
	{
		signa=2*Pi*num/(fai*Ts);//����signaֵ
		x=(xf-xs)*((signa-sin(signa))/(2*Pi))+xs;//����ʱ�̼���x����ɢ��
	}else
	{
		x = (xs-xf)*(num-Ts*fai)/(Ts-Ts*fai)+xf;
	}
	return x;
}

int LUJIN_X2(u8 t)//�Խǲ�̬X������˲�̬���ɺ���
{
	int x;
	float num,signa;
	num=t*Bj;
	if(num<=Ts*fai)
	{
		signa=2*Pi*num/(fai*Ts);//����signaֵ
		x=(xsh-xfh)*((signa-sin(signa))/(2*Pi))+xfh;//����ʱ�̼���x����ɢ��
	}else
	{
		x = (xfh-xsh)*(num-Ts*fai)/(Ts-Ts*fai)+xsh;
	}
	return x;
}

int LUJIN_Z_left(u8 t)//�Խǲ�̬X������˲�̬���ɺ���
{
	int z;
	float num,signa;
	num=t*Bj;
	if(num<=Ts*fai)
	{
		signa=2*Pi*num/(fai*Ts);//����signaֵ
		z=(zf_1-zs_1)*((signa-sin(signa))/(2*Pi))+zs_1;//����ʱ�̼���x����ɢ��
	}else
	{
		z = (zs_1-zf_1)*(num-Ts*fai)/(Ts-Ts*fai)+zf_1;
	}
	return z;
}

int LUJIN_Z_right(u8 t)//�Խǲ�̬X������˲�̬���ɺ���
{
	int z;
	float num,signa;
	num=t*Bj;
	if(num<=Ts*fai)
	{
		signa=2*Pi*num/(fai*Ts);//����signaֵ
		z=(zf_2-zs_2)*((signa-sin(signa))/(2*Pi))+zs_2;//����ʱ�̼���x����ɢ��
	}else
	{
		z = (zs_2-zf_2)*(num-Ts*fai)/(Ts-Ts*fai)+zf_2;
	}
	return z;
}

int LUJIN_X3(u8 t)//���в�̬X����ǰ����̬���ɺ���
{
	int x;
	float num,signa;
	num=t*Bj2;
	if(num<=Ts2*fai2)
	{
		signa=2*Pi*num/(fai2*Ts2);//����signaֵ
		x=(xf2-xs2)*((signa-sin(signa))/(2*Pi))+xs2;//����ʱ�̼���x����ɢ��
	}else
	{
		x = (xs2-xf2)*(num-Ts2*fai2)/(Ts2-Ts2*fai2)+xf2;
	}
	return x;
}

int LUJIN_Y3(u8 t)//���в�̬Y����ǰ����̬���ɺ���
{
	int y;
	float num,signa;
	num=t*Bj2;
	signa=2*Pi*num/(fai2*Ts2);//����signaֵ
	if(num<=Ts2*fai2)
	{
		y=h2*(1-cos(signa))/2+zs2;//����ʱ�̼���y����ɢ��
	}else 
	{
		y=0;
	}
	return -y;
}


