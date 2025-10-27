/***********************************************
**  工程名称：UNV-LD3320+STC11语音识别模块驱动程序
**	CPU: STC11L08XE
**	晶振：22.1184MHZ
**	波特率：9600 bit/S
**  修改日期：2018.5.13
**  说明：口令模式： 即每次识别时都需要说“银狼”这个口令 ，才能够进行下一级的识别
/***********************************************/
#include "config.h"

extern void  delay(unsigned long uldata);

uint8 idata ucRegVal;
extern uint8 idata nAsrStatus;

void ProcessInt0(void);

/************************************************************************
功能描述： 	 复位LD模块
入口参数：	 none
返 回 值： 	 none
其他说明：	 对芯片的第 47 腿（RSTB*）发送低电平，然后需要对片选 CS 做一次
             拉低→拉高的操作，以激活内部 DSP。如果有时芯片的反应不太正常，也可
             用这个方法恢复芯片的初始状态。
**************************************************************************/
void LD_Reset()
{
	RSTB=1;
	delay(5);
	RSTB=0;
	delay(5);
	RSTB=1;

	delay(5);
	CSB=0;
	delay(5);
	CSB=1;
	delay(5);
}
/************************************************************************
功能描述： LD模块命令初始化
入口参数： none
返 回 值： none
其他说明： 该函数为出厂配置，一般不需要修改；
		   语音识别用初始化（包括通用初始化）→写入识别列表→开始识别，
           并准备好中断响应函数，打开中断允许位。
           这里需要说明一下，如果不用中断方式，也可以通过查询方式工作。在“开
           始识别”后，读取寄存器 B2H 的值，如果为 21H 就表示有识别结果产生。
           在此之后读取候选项等操作与中断方式相同。
**************************************************************************/
void LD_Init_Common()
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	delay(10);
	LD_ReadReg(0x06);  
	
	LD_WriteReg(0x89, 0x03);  
	delay(5);
	LD_WriteReg(0xCF, 0x43);   
	delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	
	LD_WriteReg(0x1E,0x00);
	//!!注意，下面三个寄存器，会随晶振频率变化而设置不同
    //!!注意,请根据使用的晶振频率修改参考程序中的 CLK_IN
	LD_WriteReg(0x19, LD_PLL_ASR_19); 
	LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	delay(10);
	
	LD_WriteReg(0xCD, 0x04);
	//	LD_WriteReg(0x17, 0x4c); 
	delay(5);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

/************************************************************************
功能描述： 	 LD模块  语音识别用初始化
入口参数：	 none
返 回 值： 	 none
其他说明：	 该函数为出厂配置，一般不需要修改；
**************************************************************************/
void LD_Init_ASR()
{
	LD_Init_Common();
	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);
	delay( 10 );
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	delay( 1 );
}

/************************************************************************
功能描述： 	中断处理函数
入口参数：	 none
返 回 值： 	 none
其他说明：	当LD模块接收到音频信号时，将进入该函数，
			判断识别是否有结果，如果没有从新配置寄
            存器准备下一次的识别。
**************************************************************************/
void ProcessInt0(void)
{
	uint8 nAsrResCount=0;
	
	EX0=0;
	ucRegVal = LD_ReadReg(0x2B);
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;
	if((ucRegVal & 0x10) &&LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)/*识别成功*/
	{	
		nAsrResCount = LD_ReadReg(0xba);
		if(nAsrResCount>0 && nAsrResCount<=4) 
		{
			nAsrStatus=LD_ASR_FOUNDOK;
		}
		else
		{
			nAsrStatus=LD_ASR_FOUNDZERO;
		}	
	}/*没有识别结果*/
	else
	{	 
		nAsrStatus=LD_ASR_FOUNDZERO;
	}
	LD_WriteReg(0x2b,0);
	LD_WriteReg(0x1C,0);/*写0:ADC不可用*/
	LD_WriteReg(0x29,0);
	LD_WriteReg(0x02,0);
	LD_WriteReg(0x2B,0);
	LD_WriteReg(0xBA,0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);/*清除FIFO_DATA*/
	LD_WriteReg(0x08,0);/*清除FIFO_DATA后 再次写0*/
	EX0=1;
}

/************************************************************************
功能描述： 	运行ASR识别流程
入口参数：	none
返 回 值：  asrflag：1->启动成功， 0—>启动失败
其他说明：	识别顺序如下:
						1、RunASR()函数实现了一次完整的ASR语音识别流程
						2、LD_AsrStart() 函数实现了ASR初始化
						3、LD_AsrAddFixed() 函数实现了添加关键词语到LD3320芯片中
						4、LD_AsrRun()	函数启动了一次ASR语音识别流程					
						任何一次ASR识别流程，都需要按照这个顺序，从初始化开始
**************************************************************************/
uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	for (i=0; i<5; i++)			//	防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_AsrStart();
		delay(50);
		if (LD_AsrAddFixed()==0)
		{
			LD_Reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}
		delay(10);
		if (LD_AsrRun() == 0)
		{
			LD_Reset();			//	LD3320芯片内部出现不正常，立即重启LD3320芯片
			delay(50);			//	并从初始化开始重新ASR识别流程
			continue;
		}
		asrflag=1;
		break;					//	ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}

	return asrflag;
}
/************************************************************************
功能描述：  检测LD模块是否空闲
入口参数：	none
返 回 值： 	flag：1-> 空闲
其他说明：	none
**************************************************************************/
uint8 LD_Check_ASRBusyFlag_b2()
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		delay(10);		
	}
	return flag;
}
/************************************************************************
功能描述： 	启动ASR
入口参数：	none
返 回 值： 	none
其他说明：	none
**************************************************************************/
void LD_AsrStart()
{
	LD_Init_ASR();
}
/************************************************************************
功能描述： 	运行ASR
入口参数：	none
返 回 值： 	1：启动成功
其他说明：	none
**************************************************************************/
uint8 LD_AsrRun()
{
	EX0=0;
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay( 1 );
	LD_WriteReg(0x08, 0x00);
	delay( 1 );

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}
//	LD_WriteReg(0xB6, 0xa); //识别时间	 1S
//	LD_WriteReg(0xB5, 0x1E); //背景音段时间 300ms
//	LD_WriteReg(0xB8, 10); //结束时间
//	LD_WriteReg(0x1C, 0x07); //配置双通道音频信号做为输入信号
	LD_WriteReg(0x1C, 0x0b); //配置麦克风做为输入信号
	LD_WriteReg(0xB2, 0xff);
	delay( 1);	
	LD_WriteReg(0x37, 0x06);
	delay( 1 );
    LD_WriteReg(0x37, 0x06);
	delay( 5 );
	LD_WriteReg(0x29, 0x10);
	LD_WriteReg(0xBD, 0x00);
	EX0=1;
	return 1;
}
/************************************************************************
功能描述： 向LD模块添加关键词
入口参数： none
返 回 值： flag：1->添加成功
其他说明： 用户修改.
			1、根据如下格式添加拼音关键词，同时注意修改sRecog 和pCode 数组的长度
			和对应变了k的循环置。拼音串和识别码是一一对应的。
			2、开发者可以学习"语音识别芯片LD3320高阶秘籍.pdf"中
			关于垃圾词语吸收错误的用法，来提供识别效果。
			3、”yin lang “ 为口令，故在每次识别时，必须先发一级口令“银狼”
**************************************************************************/
uint8 LD_AsrAddFixed()
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 16   /*数组二维数值*/
	#define DATE_B 30	/*数组一维数值*/
	uint8 code sRecog[DATE_A][DATE_B] = {
		 "yin lang",\
		 "qian jin",\
		 "hou tui",\
		 "zuo xiang yi dong",\
		 "you xiang yi dong",\
		 "zuo zhuan",\
		 "you zhuan",\
		 "zhan li",\
		 "dun xia",\
		 "yang tou",\
		 "di tou",\	
		 "wo shou",\
		 "yuan di ta bu",\
		 "zuo you yao bai",\
		 "qian hou yao bai",\
		 "san bu"
	};	/*添加关键词，用户修改*/
	uint8 code pCode[DATE_A] = {
		CODE_CMD,\
		CODE_forward,\
		CODE_back,\
		CODE_left,\
		CODE_right,\
		CODE_left_turn,\
		CODE_right_turn,\
		CODE_stand,\
		CODE_sit,\
		CODE_rise,\
		CODE_down,\
		CODE_hand,\
		CODE_stand_still,\
		CODE_left_right_swing,\
		CODE_forward_back_swing,\
		CODE_walk
	};	/*添加识别码，用户修改*/	
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{
			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}
		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0 );
		LD_WriteReg(0x08, 0x04);
		delay(1);
		LD_WriteReg(0x08, 0x00);
		delay(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}
    return flag;
}
/************************************************************************
功能描述： 	获取识别结果
入口参数：	none
返 回 值： 	LD_ReadReg(0xc5 )；  读取内部寄存器返回识别码。
其他说明：	none
**************************************************************************/
uint8 LD_GetResult()
{		
	return LD_ReadReg(0xc5 );
}



