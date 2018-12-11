//products mode:   MW
//functional description: 
//MCU mode:        HT66F018
//MCU frequency:   8MHz
//designer:        Lan
//data:    2018/7/27 10:58
//record:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "base.h"
#define EXTERN
#define VAR_DEFINE
#include "Var.h"

extern unsigned int ntc_val;
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
	_idlen 	= 0;				//   IDLEN 位为低时，系统进入休眠模式
	_wdtc 	= 0b10101000;		// 除能看门狗
	_lvden 	= 0x00;		  		// 除能LVD    //***  低电压检测控制位
	GCC_HALT();	
}

void Sleep(void)
{
//	u8 i = 0;	
//	for(i=0;i<8;i++)		//清零防止启动时点亮几个灯
//		LEDSta[i] = 0;
//	IOReset();
	_adoff = true;
	Dealy();
	do
	{
		_emi = 0;		
		SleepMode();		//进入休眠
		_emi = 1;
		break;		
	}while(1);

	_adoff = false;
}

/********************************************************/
void main(void)
{	
    InitSys();					//系统初始化
    Ram_Init();
    Timer_Init();				//定时器初始化
    while(1)
    {
		GCC_CLRWDT();			//清看门狗
		GetPTCValu();	     	//检测NTC
		Get_Battery_Valu();		//检测电池电量      
		GetKey();				//读取键值
		Run_Operation();		//
		Battery_Status_Operation();
		TaskBuzzer();     
		if(DCInFlag)			//正在充电
		{			
			Breathe_Flag = 1;
		}
		else 
		{
			Breathe_Flag = 0;
		}
    }
}
