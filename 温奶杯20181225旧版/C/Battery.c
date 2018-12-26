#include "base.h"
#define EXTERN extern
#include "Var.h"


void TaskBatOp(void)
{
	if(TaskBatRdyFlag)
	{
/*		MOTOR_ON;*/
		TaskBatRdyFlag = false;
		GetBatValu();								//��ȡ��ص�ѹֵ		
/*		MOTOR_OFF;*/
		EnADCFlag = false;							//�򿪷���˿���
		if((ChargIngFlag)&&(!ChargeFullFlag))		//���ڳ��
		{			
			BatSta = BATTRY_CHARGE;
			Breathe_Flag = 1;
		}
		else 
		{
			Breathe_Flag = 0;
			if(BATLow3p5Flag)						//�Ƿ�׵�ѹ
			{
				BatSta = BATTRY_LOW;						
			}
			else
			{
				if(ChargeFullFlag)					//�Ƿ�䱥
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
				LED_Breathe_OFF;		//����״̬�ص�
				break;
			case BATTRY_CHARGE:
				break;			
			case BATTRY_FULL:
				LED_Breathe_ON;      	//�䱥���
				break;		
			case BATTRY_LOW:		
				if(LowBatShutDnSCnt <= BAT_LOW_TIP)    	//����ʱ�����
				{
					if(Sys1SCnt & 0x01)		//��˸
					{
						LED_Breathe_ON;
					}
					else
					{
						LED_Breathe_OFF;
					}			
					if(Task5SFlag)			//5��	
					{		
						Task5SFlag = false;			
						KeyBuzzer(4);		// ������
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