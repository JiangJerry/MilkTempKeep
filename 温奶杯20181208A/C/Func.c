#include "base.h"
#define EXTERN extern
#include "Var.h"

/****************************************************************/

void Run_Operation(void)
{
	static u8 StaFlick;
	switch(WorkMode)
	{
		case READY_MODE:							// Long_Press  ����
		/**************************************/
			if((Key_Forward_Long_val_Flag == 1)) 
			 {
			 	Key_Forward_Long_val_Flag = 0;
				KeyBuzzer(1);
				Power_ON;
				Open5V();
				Open5V200msCnt = 0;
				NormalWinterFlag = false;			
				WorkMode = RUN_MODE;		//�������ģʽ	
				PreHeatFlag = true;			//Ԥ��һ����
				Time1SCnt = 0;				//�����ʱ
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
				if(Close_5V_Flag)					//�ػ�
				{
					Out_5V_IO = 0;					
					if(Open5V200msCnt >= 50)
					{
						Close_5V_Flag = false;
						Open5V200msCnt = 0;
						Out_5V_IO = 1;
					}
				}
				if((SleepCnt >= 5) && (!DCInFlag))	//��DC���룬5���Զ�˯��
				{
					SleepCnt = 0;
					Sleep();
				}								
			 }
			 break;
		case RUN_MODE:			 
			if((Key_Forward_Long_val_Flag))		// �жϹػ�
			{
				Key_Forward_Long_val_Flag = 0;
				Power_OFF;
				KeyBuzzer(3);
				WorkMode = 0;
				Close5V();
				SleepPrepareFlag = true;		//׼��˯��
			}
			if(Double_Click_Value_Flag)  		//�Ƿ��л�ģʽ
			{
				Double_Click_Value_Flag = 0;
				KeyBuzzer(2);
				NormalWinterFlag = !NormalWinterFlag;
			}
			if(PreHeatFlag)						//Ԥ������LED��
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
						Heat_Wire_Duty(2048);   	// ȫ������
						MOTOR_OFF;					
						break;
					case IN_22_37:
						LED_3_GREEN_ON;
						LED_3_BLU_OFF;
						LED_3_RED_OFF;
						MOTOR_OFF;
						Heat_Wire_Duty(ntc_val);	// �����¶ȼ���
						break;
					case IN_37_45:					//�����¶�ʱͣ���ȳ���һ��ʱ�����ȷ�ϼ��Ȼ��Ƿ��Ƚ���
						LED_3_GREEN_OFF;
						LED_3_BLU_ON;
						LED_3_RED_ON;
						Heat_Wire_Duty(0);
						MOTOR_OFF;
						break;
					case UP_45:						//����
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
