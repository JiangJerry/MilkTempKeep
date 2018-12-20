#include "base.h"
#define EXTERN extern
#include "Var.h"

/************* BUZZ ·äÃùÆ÷*************************************/

void Open5V(void)
{
	Output5V.Times = 1;
	Output5V.OnCnt = OPEN5V_ON_TIME;
	Output5V.OffCnt = 0;	
}

void Close5V(void)
{
	Output5V.Times = 2;
	Output5V.OnCnt = OPEN5V_ON_TIME;
	Output5V.OffCnt = OPEN5V_OFF_TIME;	
}

/*************************************************************/
void KeyBuzzer(char Buzz_1)	
{
	switch(Buzz_1)
	{		
		case 1:		//  ÃùÒ»Éù
			Buzzer.Times = 1;
			Buzzer.OnCnt = 15;
			Buzzer.OffCnt = 8;
			break;
		case 2:		// µÎµÎÁ½Éù
			Buzzer.Times = 2;
			Buzzer.OnCnt = 13;
			Buzzer.OffCnt = 8;
			break;
		case 3:		// ³¤ÃùÒ»Éù
			Buzzer.Times = 1;
			Buzzer.OnCnt = 20;
			Buzzer.OffCnt = 10;
			break;
		case 4:		// ÃùÈýÉù
			Buzzer.Times = 3;
			Buzzer.OnCnt = 10;
			Buzzer.OffCnt = 8;
			break;
		default:
			
			break;
	}
}

void TaskBuzzer(void)
{
	if(TaskBuzzerRdy)
	{
		TaskBuzzerRdy = false;
		if(Buzzer.Times > 0)	//·äÃùÆ÷ÏìÓ¦
		{
			if(Buzzer.OnCnt-- > 0)
			{
				Buzz_ON;
			}
			else
			{
				if(Buzzer.OffCnt-- > 0)
				{
					Buzz_OFF;
				}
				else
				{
					if(Buzzer.Times > 0)
					{
						Buzzer.Times--;
						Buzzer.OnCnt = BUZZER_ON_TIME;
						Buzzer.OffCnt = BUZZER_OFF_TIME;
					}				
				}
			}		
		}
		else
		{		
			Buzz_OFF;
		}
		
		if(Output5V.Times > 0)	//5vÊä³ö
		{
			if(Output5V.OnCnt-- > 0)
			{
				Out_5V_IO_Open;
			}
			else
			{
				if(Output5V.OffCnt-- > 0)
				{
					Out_5V_IO_Close;
				}
				else
				{
					if(Output5V.Times > 0)
					{
						Output5V.Times--;
						Output5V.OnCnt = OPEN5V_ON_TIME;
						Output5V.OffCnt = OPEN5V_OFF_TIME;
					}				
				}
			}		
		}
		else
		{		
			Out_5V_IO_Close;
		}			
						
	}
}
