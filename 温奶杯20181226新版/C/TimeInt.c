#include "base.h"
#define EXTERN extern
#include "Var.h"

#define INIT_NO_DETECT_TIME	3
volatile static unsigned char Time1SClearCnt __attribute__ ((at(0x1d7)));

unsigned char Time500mSCnt = 0,Time4mSCnt = 0;

/********************************************************/
volatile unsigned int PWMCnt = 0;
volatile unsigned int HuxiCnt = 0;
volatile unsigned int PWMValu = 10;

#define HUXI_VAL 		1600
#define PWM_CYCLE		12

/***************** 呼吸灯 **********************************************/
void LED_Breathe(void)
{
	if((Breathe_Flag == 1) && (BatSta != BATTRY_FULL))
	{
	    PWMCnt++;
	    HuxiCnt++;
	    if(PWMCnt == PWMValu)      //判断是否到了点亮LED的时候
	    {
	        LED_Breathe_ON;       	//点亮LED
	    }
	    if(PWMCnt == PWM_CYCLE)   	//当前周期结束
	    {
	        LED_Breathe_OFF;      	//熄灭LED
	        PWMCnt = 0;              //重新计时
	    }
	    if((HuxiCnt == HUXI_VAL) && (direc_flag == 0))
	    {                               //占空比增加10%
	        HuxiCnt = 0;
	        PWMValu++;
	        if(PWMValu == PWM_CYCLE)  	//占空比更改方向
	            direc_flag = 1; 
	    }
 	    if((HuxiCnt == HUXI_VAL) && (direc_flag == 1))
	    {                               //占空比减少10%
	        HuxiCnt = 0;
	        PWMValu--;
	        if(PWMValu == 1)          //占空比更改方向
	            direc_flag = 0; 
	    } 	
	}	
}

/*****************************************************************/
extern  int ntc_val;		
//值越小，底电平时间越小，高电平时间越长，MOS导通越长
void HeatCtrDuty(u16 step_15)
{
	if(!EnADCFlag)	
	{
		_tm0al = ((step_15&0xff));
		_tm0ah = ((step_15>>8));
	}
	else
	{
		_tm0al = 0;		//关闭发热丝以备测试
		_tm0ah = 0;		
	}	
}
/*********************************************************/
void TimerInit(void)
{               
//time0 PWM
	_wdtc = 0xA8;  	
	_pac0 = 0;//PA0 output--PWM输出口	
	_t0cp = 1;//TP0(PA0)引脚控制位使能

	//系统时钟
//	_smod = 0x01;//fsys = fH   bit0 = 1
	
	//-------STM init-------
	//计数器和比较器
	_tm0al = 0x00;//占空比设定
	_tm0ah = 0x00;//占空比设定
	_tm0dl = 0x00;
	_tm0dh = 0x00; 
	_tm0rp = 0x08;//周期设定, 8位，与_tm0dh比较
	_tm0c1 = 0xA4;//设定PWM模式，CCRP - 周期；CCRA - 占空比
	_tm0c0 = 0x28;//启动timer0, 计数时钟选择fsys
/*********************  TM_2 INIT ******************************************************/
	_t2cp = 0;
	_tm2al = 0xc8;     //  03e8  500us  07d0 = 1ms   c8 = 100us
	_tm2ah = 0x00;
	_tm2rp = 0xff;

	_tm2c1 = 0b11000001;     // 匹配器 A 
	_tm2c0 = 0b00001000;		// Tm2 开关控制位  bit3 = 1  Open
	
	_t2af = 1;		// 中断请求标志位
	_t2ae = 1;		// a 匹配中断控制位
	_mf2e = 0;		//多功能中断2控制位使能	
	
	_mf1f = 1;
	_mf1e = 1;
		
/**************	外部中断 ************************************************************/ 
	_int0s0 = 1;_int0s1 = 1;			// 10 下降沿触发   11 双沿触发
    _int0e = true;  
	_emi = 1;      
	Time1SClearCnt = 0;      
}

/*********************************************************************************************************
** 函数名称: ISR_tmr2
** 功能描述: 定时器2中断服务函数，控制水泵出水量
** 输　入: 	 
** 输　出 :  无
** 全局变量: 无							  
** 调用模块: 无 .100us中断
********************************************************************************************************/
u16 Time100uSCnt = 0;
u16 ChargPlusDly = 0;
/**********************************************************

 * 名称: void Led2Disp_Scan(void)
 * 功能: led2和按键扫描函数
 * 形参: 无
 * 返回: 无
 * 说明: 无
 **********************************************************/
unsigned char scan_num = 0;//扫描状态变量
void LEDDispKeyScan(void)
{
	scan_num++;	
	switch(scan_num)	
	{
		case 1://状态1扫描LED灯
			if(LEDSta[0] == 1)	//红
			{
				COM2_ON;
				COM3_ON;
				COM1_OFF;
			}
			else				//一定要有关的操作，不然会出现闪烁
			{
				COM2_OFF;
				COM3_OFF;
				COM1_OFF;				
			}
			break;
		case 2:
			if(LEDSta[1] == 1)	//蓝
			{				
				COM1_ON;
				COM2_OFF;
				COM3_OFF;
			}
			else
			{
				COM2_OFF;
				COM3_OFF;
				COM1_OFF;				
			}
			break;
		case 3:	
			if(LEDSta[2] == 1)	//白
			{
				COM1_ON;
				COM2_ON;
				COM3_OFF;
			}
			else
			{
				COM2_OFF;
				COM3_OFF;
				COM1_OFF;				
			}
			break;
      	case 4:
			if(LEDSta[3] == 1)	//绿
			{
				COM3_ON;
				COM1_OFF;
				COM2_OFF;
			}
			else
			{
				COM2_OFF;
				COM3_OFF;
				COM1_OFF;				
			}
 			scan_num = 0;
	      	break;
      default:
	      break;
   }
}

#define CNT_4MS		40 	//4ms
#define CNT_1S		250	//1s
void __attribute((interrupt(0x10))) ISR_tmr2 (void)
{
//	static u8 i =0;
//	u8 j = 0;
	_t2af = 0;
	Time100uSCnt++;	
	LED_Breathe();						//呼吸灯
	if(Time100uSCnt >= CNT_4MS)			//4ms
	{
		Time100uSCnt = 0;
		LEDDispKeyScan();				//4ms扫描灯
		Time4mSCnt++;
		if((Time4mSCnt&0x0f) == 0x0e)	//提前4ms让电压恢复
		{
			EnADCFlag = true;			
		}
		if((Time4mSCnt&0x0f) == 0x0f)	//67ms周期
		{
			TaskBatRdyFlag = true;
		}
		if((Time4mSCnt&0x03) == 0x03)	//12ms周期
		{
			TaskADRdyFlag = true;
		}
		TaskBuzzerRdy  = true;
		ChargPlusDly++;
		if((Time4mSCnt & 0x02) == 0x02)	//按键8MS周期扫描
			TaskKeyRdy = true;
		Open5V200msCnt++;
		Time500mSCnt++;		
		if(Time4mSCnt >= CNT_1S)		//1s
		{
			Time4mSCnt = 0;
			Time1SCnt++;
			Sys1SCnt++;
			Time1SClearCnt++;
			SumOfEdge++;
			Stay45Cnt++;				//保持时间
			if(WorkMode == READY_MODE)	//待机模式下
			{
				SleepCnt++;				
			}
			if((Sys1SCnt % 5) == 0)
			{
				Task5SFlag = true;
			}
			if(BATLow3p5Flag)			//底电压状态下关机延时
			{
				LowBatShutDnSCnt++;
			}
		}
		if(Time500mSCnt >= 125)			//500ms
		{			
			Task500msFlag = true;
			Time500mSCnt = 0;
		}
		if(Time1SClearCnt >= 5)			//5S清除延时
		{
			Time1SClearCnt = 0;
			IntCnt = 0;
			ChargIngFlag = false;
		}
		if(Sys1SCnt >= 60)
		{
			Sys1SCnt = 0;
			if(BATUp4p1Flag)					//4.1V以上计时生效
			{
				BATChargeFullCnt++;
				if(BATChargeFullCnt >= CHARG_FULL_TIME)
				{
					ChargeFullFlag = true;		//充饱
				}
			}
		}			
	} 	
}
/*********************************************************************************************************/

void __attribute((interrupt(0x0c))) ISR_tmr0 (void)
{
/*	_t0af=0;*/									// clr TM0 Comparator A match interrupt req flg.
	_t0pf = 0;//进中断后，请清除中断标志
	_nop();	
}

/*********************************************************************************************************
** 函数名称: ISR_int0
** 功能描述: 外部中断0，检测到最长信号脉宽信号
** 输　入: 	 
** 输　出 :  无
** 全局变量: 无							  
** 调用模块: 无
********************************************************************************************************/
void __attribute((interrupt(0x04))) ISR_int0 (void)
{
    _int0f=0;
	if((ChargPlusDly > 170 ) && (ChargPlusDly < 350))	//检测到底电平,暂时不清楚中心值在哪里 1100ms好像
	{		
		if(IntCnt++ >= 2)								//2次检测有效，否则容易出现开关机时的底电平误检测
		{
			ChargIngFlag = true;						//正在充电中
		}
		Time1SClearCnt = 0;								//清除无DC插入计时
	}
	SleepCnt = 0;										//防止拨掉DC后残留脉冲导致一会醒一会睡的情况，清除睡眠计时
	ChargPlusDly = 0;
}