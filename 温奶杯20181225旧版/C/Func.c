#include "base.h"
#define EXTERN extern
#include "Var.h"

#define PWM_0_VALU		0
#define PWM_50_VALU	    2048

#define PreHeat() 	{PreHeatFlag = true;Time1SCnt = 0;}			//Ԥ��

/****************************************************************/

void TaskFunc(void)
{
	static u8 StaFlick;
	switch(WorkMode)
	{
		case READY_MODE:							// Long_Press  ����
			if((Key_Forward_Long_val_Flag == 1)) 
			 {
			 	Key_Forward_Long_val_Flag = 0;
				KeyBuzzer(1);
				Open5V();
				Open5V200msCnt = 0;
				WorkMode = RUN_MODE;		//�������ģʽ
				PreHeat();					//Ԥ��
			 	TempDir = false;			//�¶ȳ�ʼ��������	
			 	_t0on = true;				// turn off timer 0 to change timer config
			 	IntCnt = 0;					//��ֹ��Ϊ���ػ�������жϼ��������º�������
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
				if((SleepCnt >= 5) && (!ChargIngFlag))	//��DC���룬5���Զ�˯��
				{
					LED_Breathe_OFF;
					SleepCnt = 0;
					Sleep();					
				 	IntCnt = 0;	
					BatSta = BATTRY_NORMAL;				//��ؼ�����¿�ʼ
					BATLow3p5Flag = false;
					BATUp4p1Flag = false;
					ChargeFullFlag = false;
				}								
			 }
			 break;
		case RUN_MODE:			 
			if((Key_Forward_Long_val_Flag))				// �жϹػ�
			{
				Key_Forward_Long_val_Flag = 0;
				KeyBuzzer(3);
				WorkMode = READY_MODE;
				Close5V();
			}
			if(Double_Click_Value_Flag)  		//�Ƿ��л�ģʽ
			{
				Double_Click_Value_Flag = 0;
				KeyBuzzer(1);
				NormalWinterFlag = !NormalWinterFlag;
				PreHeat();						//�ٴ�Ԥ��
			}
			if(PreHeatFlag)						//Ԥ������LED��
			{
				MOTOR_OFF;
				HeatCtrDuty(PWM_0_VALU);		//Ԥ���޷�Ӧ
		
				if(Time1SCnt >= PRE_HEAT)		//Ԥ�ȼ���ʱ��
				{
					Open5V();
					Open5V200msCnt = 0;		//��5V
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
						if(Sys1SCnt & 0x01)						//������˸����
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
						HeatCtrDuty(ntc_val);		// �����¶ȼ���
						break;
					case STAY_45:					//�����¶�ʱͣ���ȳ���һ��ʱ�����ȷ�ϼ��Ȼ��Ƿ��Ƚ���
						LEDSta[LED_BLUE_INDEX] = LED_OFF;
						LEDSta[LED_GREEN_INDEX] = LED_OFF;
						LEDSta[LED_RED_INDEX] = LED_ON;
						HeatCtrDuty(PWM_0_VALU);
						MOTOR_OFF;
						break;
					case UP_45:						//����
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
