#include "base.h"
#define EXTERN extern
#include "Var.h"


void TaskBatOp(void)
{
	if(TaskBatRdyFlag)
	{
/*		MOTOR_ON;*/
		TaskBatRdyFlag = false;
		GetBatValu();								//获取电池电压值		
/*		MOTOR_OFF;*/
		EnADCFlag = false;							//打开发热丝输出
		if((ChargIngFlag)&&(!ChargeFullFlag))		//正在充电
		{			
			BatSta = BATTRY_CHARGE;
			Breathe_Flag = 1;
		}
		else 
		{
			Breathe_Flag = 0;
			if(BATLow3p5Flag)						//是否底电压
			{
				BatSta = BATTRY_LOW;						
			}
			else
			{
				if(ChargeFullFlag)					//是否充饱
				{
					BatSta = BATTRY_FULL;
				}
				else
				{
					BatSta = BATTRY_NORMAL;			
				}
			}
		}		
		switch(BatSta)
		{
			case BATTRY_WAIT:
				break;
			case BATTRY_NORMAL:
				LED_Breathe_OFF;		//正常状态关灯
				break;
			case BATTRY_CHARGE:
				break;			
			case BATTRY_FULL:
				LED_Breathe_ON;      	//充饱点灯
				break;		
			case BATTRY_LOW:		
				if(LowBatShutDnSCnt <= BAT_LOW_TIP)    	//提醒时间过后
				{
					if(Sys1SCnt & 0x01)		//闪烁
					{
						LED_Breathe_ON;
					}
					else
					{
						LED_Breathe_OFF;
					}			
					if(Task5SFlag)			//5秒	
					{		
						Task5SFlag = false;			
						KeyBuzzer(4);		// 鸣三声
					}
				}
				else
				{
					WorkMode = READY_MODE;
					Close5V();
					BatSta = BATTRY_WAIT;				
				}
				break;
			default:
				LED_Breathe_OFF;
				break;
		}		
	}

}