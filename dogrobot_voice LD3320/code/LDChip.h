/*************************************************
**  工程名称：UNV-LD3320+STC11语音识别模块驱动程序
**	CPU: STC11
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**	配套产品信息：UNV-LD3320+STC11语音识别开发板
**  说明：口令模式： 即每次识别时都需要说“小鹰”这个口令 ，才能够进行下一级的识别
/****************************************************/


#ifndef LD_CHIP_H
#define LD_CHIP_H

#define uint8 unsigned char
#define uint16 unsigned int
#define uint32 unsigned long

//	以下五个状态定义用来记录程序是在运行ASR识别过程中的哪个状态
#define LD_ASR_NONE				0x00	 /*	表示没有在作ASR识别*/
#define LD_ASR_RUNING			0x01	/*	表示LD3320正在作ASR识别中*/
#define LD_ASR_FOUNDOK			0x10	/*表示一次识别流程结束后，有一个识别结果*/
#define LD_ASR_FOUNDZERO 		0x11	/*表示一次识别流程结束后，没有识别结果*/
#define LD_ASR_ERROR	 		0x31	/*表示一次识别流程中LD3320芯片内部出现不正确的状态*/


#define CLK_IN   		22.1184	/* 用户注意修改输入的晶振时钟大小 */
#define LD_PLL_11			(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

//函数声明
void LD_Reset();
void LD_Init_Common();
void LD_Init_ASR();
uint8 RunASR(void);
void LD_AsrStart();
uint8 LD_AsrRun();
uint8 LD_AsrAddFixed();
uint8 LD_GetResult();


//识别码客户修改处 
#define CODE_CMD  0x00   //该命令码0x00用户不可进行修改。
#define CODE_forward 0x01
#define CODE_back 0x02
#define CODE_left 0x03
#define CODE_right 0x04
#define CODE_left_turn 0x05
#define CODE_right_turn 0x06
#define CODE_stand 0x07
#define CODE_sit 0x08
#define CODE_rise 0x09
#define CODE_down 0x0a
#define CODE_hand 0x0b
#define CODE_stand_still 0x0c
#define CODE_left_right_swing 0x0d
#define CODE_forward_back_swing 0x0e
#define CODE_walk 0x0f

//数值越大越灵敏识别距离越远，但误识别率就越大， 根据自己的实际情况调节。
#define MIC_VOL 0x55	 //咪头增益（灵敏度调节） 范围：00-7f 
#endif
