#include "base.h"
#define EXTERN extern
#include "Var.h"

#define PWM_0_VALU		0
#define PWM_50_VALU	    2048

#define PreHeat() 	{PreHeatFlag = true;Time1SCnt = 0;}			//预热

/****************************************************************/

void TaskFunc(void)
{
	static u8 StaFlick;
	switch(WorkMode)
	{
		case READY_MODE:							// Long_Press  开机
			if((Key_Forward_Long_val_Flag == 1)) 
			 {
			 	Key_Forward_Long_val_Flag = 0;
				KeyBuzzer(1);
				Open5V();
				Open5V200msCnt = 0;
				WorkMode = RUN_MODE;		//进入恒温模式
				PreHeat();					//预热
			 	TempDir = false;			//温度初始上升过程	
			 	_t0on = true;				// turn off timer 0 to change timer config
			 	IntCnt = 0;					//防止因为开关机引起的中断计数，导致呼吸灯亮
			 }
			 else 	
			 {	
			 	_t0on = false;						// turn off timer 0 to change timer config		
				Heat_Wire_OFF;
		 	 	COM1_OFF;		 	
			 	COM2_OFF;
			 	COM3_OFF;
			 	MOTOR_OFF;
				LEDSta[LED_RED_INDEX] = LED_OFF;
				LEDSta[LED_BLUE_INDEX] = LED_OFF;
				LEDSta[LED_WHITE_INDEX] = LED_OFF;
				LEDSta[LED_GREEN_INDEX] = LED_OFF;
				LED_WHITE_2_OFF;
				if((SleepCnt >= 5) && (!ChargIngFlag))	//无DC插入，5秒自动睡眠
				{
					LED_Breathe_OFF;
					SleepCnt = 0;
					Sleep();					
				 	IntCnt = 0;	
					BatSta = BATTRY_NORMAL;				//电池检测重新开始
					BATLow3p5Flag = false;
					BATUp4p1Flag = false;
					ChargeFullFlag = false;
				}								
			 }
			 break;
		case RUN_MODE:			 
			if((Key_Forward_Long_val_Flag))				// 判断关机
			{
				Key_Forward_Long_val_Flag = 0;
				KeyBuzzer(3);
				WorkMode = READY_MODE;
				Close5V();
			}
			if(Double_Click_Value_Flag)  		//是否切换模式
			{
				Double_Click_Value_Flag = 0;
				KeyBuzzer(1);
				NormalWinterFlag = !NormalWinterFlag;
				PreHeat();						//再次预热
			}
			if(PreHeatFlag)						//预热轮闪LED灯
			{
				MOTOR_OFF;
				HeatCtrDuty(PWM_0_VALU);		//预热无反应
		
				if(Time1SCnt >= PRE_HEAT)		//预热检索时间
				{
					Open5V();
					Open5V200msCnt = 0;		//打开5V
					PreHeatFlag = false;
				}			
				if(Task500msFlag)
				{
					Task500msFlag = false;
					switch(StaFlick)
					{
						case 0:
							LEDSta[LED_RED_INDEX] = LED_ON;
							LEDSta[LED_GREEN_INDEX] = LED_OFF;
							LEDSta[LED_BLUE_INDEX] = LED_OFF;
							break;
						case 1:
							LEDSta[LED_RED_INDEX] = LED_OFF;
							LEDSta[LED_GREEN_INDEX] = LED_ON;
							LEDSta[LED_BLUE_INDEX] = LED_OFF;
							break;
						case 2:
							LEDSta[LED_RED_INDEX] = LED_OFF;
							LEDSta[LED_GREEN_INDEX] = LED_OFF;
							LEDSta[LED_BLUE_INDEX] = LED_ON;
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
					case DOWN_30:
						if(Sys1SCnt & 0x01)						//蓝灯闪烁报警
						{
							LEDSta[LED_BLUE_INDEX] = LED_OFF;							
						}
						else
						{					
							LEDSta[LED_BLUE_INDEX] = LED_ON;
						}
						LEDSta[LED_GREEN_INDEX] = LED_OFF;
						LEDSta[LED_RED_INDEX] = LED_OFF;					
						HeatCtrDuty(PWM_0_VALU);
						MOTOR_OFF;
						break;
					case IN_30_45:
						LEDSta[LED_RED_INDEX] = LED_OFF;
						LEDSta[LED_GREEN_INDEX] = LED_ON;
						LEDSta[LED_BLUE_INDEX] = LED_OFF;
						MOTOR_OFF;
						HeatCtrDuty(ntc_val);		// 按现温度加热
						break;
					case STAY_45:					//区间温度时停风扇持续一段时间后再确认加热还是风扇降温
						LEDSta[LED_BLUE_INDEX] = LED_OFF;
						LEDSta[LED_GREEN_INDEX] = LED_OFF;
						LEDSta[LED_RED_INDEX] = LED_ON;
						HeatCtrDuty(PWM_0_VALU);
						MOTOR_OFF;
						break;
					case UP_45:						//高温
						SumOfEdge = 0;
						LEDSta[LED_BLUE_INDEX] = LED_OFF;
						LEDSta[LED_GREEN_INDEX] = LED_OFF;
						LEDSta[LED_RED_INDEX] = LED_ON;
						HeatCtrDuty(PWM_0_VALU);
						if(NormalWinterFlag)
						{							
							MOTOR_OFF;
						}
						else
						{							
							MOTOR_ON;
						}
						break;
					default:
						SumOfEdge = 0;
						LEDSta[LED_BLUE_INDEX] = LED_OFF;
						LEDSta[LED_GREEN_INDEX] = LED_OFF;
						LEDSta[LED_RED_INDEX] = LED_OFF;
						HeatCtrDuty(PWM_0_VALU);
						MOTOR_OFF;
						break;
				}
			}
			if(NormalWinterFlag)
			{
				LEDSta[LED_WHITE_INDEX] = LED_OFF;
				LED_WHITE_2_ON;					
			}
			else
			{			
				LED_WHITE_2_OFF;
				LEDSta[LED_WHITE_INDEX] = LED_ON;
			}
			
			break;		
		default:
			break;
	}
}
