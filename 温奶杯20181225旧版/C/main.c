//products mode:   X1
//functional description: 
//MCU mode:        HT66F018
//MCU frequency:   8MHz
//designer:        JerryJiang
//data:    2018/12/20
//record:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "base.h"
#define EXTERN
#define VAR_DEFINE
#include "Var.h"

extern  int ntc_val;
void Dealy(void)
{//延时程序
	unsigned int i,j;
	for(i=0;i<50;i++)
	{
		for(j=0;j<50;j++)
		{
			 GCC_CLRWDT();
			_nop();
		}
	}	
}
//~~~~~~~~~~~~~~~~ 休眠模式0 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SleepMode(void)
{
	_idlen 	= 0;				// IDLEN 位为低时，系统进入休眠模式
	_wdtc 	= 0b10101000;		// 除能看门狗
	_lvden 	= 0x00;		  		// 除能LVD 
	GCC_HALT();	
}

void Sleep(void)
{
 	COM1_OFF;		 	
 	COM2_OFF;
 	COM3_OFF;			 	
	_adoff = true;
	Dealy();
	do
	{
		_emi = 0;		
		SleepMode();			//进入休眠
		_emi = 1;
		break;		
	}while(1);
	_adoff = false;
}

void main(void)
{	
    InitSys();					//系统初始化
    VarInit();					//变量初始化
    TimerInit();				//定时器初始化
    while(1)
    {
		GCC_CLRWDT();			//清看门狗
		if(TaskADRdyFlag)		//4ms一次
		{
			TaskADRdyFlag = false;
			GetPTCValu();	     	//检测目标温度值
			GetAroudTempValu();		//检测室温值
		}
		TaskKey();				//读取键值
		TaskFunc();				//功能
		TaskBatOp();			//电压检测
		TaskBuzzer();     		//蜂鸣器和5V电源输出控制功能
    }
}
