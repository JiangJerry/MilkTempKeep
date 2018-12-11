#include "base.h"
#define EXTERN extern
#include "Var.h"

/****************************************************************/

void Run_Operation(void)
{
	static u8 StaFlick;
	switch(WorkMode)
	{
		case READY_MODE:							// Long_Press  开机
		/**************************************/
			if((Key_Forward_Long_val_Flag == 1)) 
			 {
			 	Key_Forward_Long_val_Flag = 0;
				KeyBuzzer(1);
				Power_ON;
				Open5V();
				Open5V200msCnt = 0;
				NormalWinterFlag = false;			
				WorkMode = RUN_MODE;		//进入恒温模式	
				PreHeatFlag = true;			//预热一分钟
				Time1SCnt = 0;				//清除计时
			 	_t0on = true;						// turn off timer 0 to change timer config		
			 }
			 else 	
			 {	
			 	_t0on = false;						// turn off timer 0 to change timer config		
			 	_pa0 = false;
			 	LED_3_BLU_OFF;		 	
			 	LED_3_GREEN_OFF;
			 	LED_3_RED_OFF;			 	
				LED_WHITE_1_OFF;
				LED_WHITE_2_OFF;
				if(Close_5V_Flag)					//关机
				{
					Out_5V_IO = 0;					
					if(Open5V200msCnt >= 50)
					{
						Close_5V_Flag = false;
						Open5V200msCnt = 0;
						Out_5V_IO = 1;
					}
				}
				if((SleepCnt >= 5) && (!DCInFlag))	//无DC插入，5秒自动睡眠
				{
					SleepCnt = 0;
					Sleep();
				}								
			 }
			 break;
		case RUN_MODE:			 
			if((Key_Forward_Long_val_Flag))		// 判断关机
			{
				Key_Forward_Long_val_Flag = 0;
				Power_OFF;
				KeyBuzzer(3);
				WorkMode = 0;
				Close5V();
				SleepPrepareFlag = true;		//准备睡眼
			}
			if(Double_Click_Value_Flag)  		//是否切换模式
			{
				Double_Click_Value_Flag = 0;
				KeyBuzzer(2);
				NormalWinterFlag = !NormalWinterFlag;
			}
			if(PreHeatFlag)						//预热轮闪LED灯
			{
				if(Task500msFlag)
				{
					Task500msFlag = false;
					switch(StaFlick)
					{
						case 0:
							LED_3_GREEN_ON;
							LED_3_BLU_OFF;
							LED_3_RED_OFF;
							break;
						case 1:
							LED_3_GREEN_OFF;
							LED_3_BLU_ON;
							LED_3_RED_OFF;						
							break;
						case 2:
							LED_3_GREEN_OFF;
							LED_3_BLU_OFF;
							LED_3_RED_ON;
							break;
						default:
							break;
					}
					if(StaFlick++ >= 2)
						StaFlick = 0;
				}
			}
			else
			{				
				switch(TempSta)
				{
					case DOWN_22:
						LED_3_BLU_ON;
						LED_3_GREEN_OFF;
						LED_3_RED_OFF;
						Heat_Wire_Duty(2048);   	// 全功加热
						MOTOR_OFF;					
						break;
					case IN_22_37:
						LED_3_GREEN_ON;
						LED_3_BLU_OFF;
						LED_3_RED_OFF;
						MOTOR_OFF;
						Heat_Wire_Duty(ntc_val);	// 按现温度加热
						break;
					case IN_37_45:					//区间温度时停风扇持续一段时间后再确认加热还是风扇降温
						LED_3_GREEN_OFF;
						LED_3_BLU_ON;
						LED_3_RED_ON;
						Heat_Wire_Duty(0);
						MOTOR_OFF;
						break;
					case UP_45:						//高温
						SumOfEdge = 0;
						LED_3_GREEN_OFF;
						LED_3_BLU_OFF;
						LED_3_RED_ON;
						Heat_Wire_Duty(0);
						MOTOR_ON;
						break;
					default:
						break;
				}
			}
		/*************************************************************/
			if(NormalWinterFlag)
			{
				LED_WHITE_1_OFF;
				LED_WHITE_2_ON;					
			}
			else
			{			
				LED_WHITE_1_ON;
				LED_WHITE_2_OFF;
			}
			break;	
	
	/***************************************************************************/			
		default:
			break;
	}
}
