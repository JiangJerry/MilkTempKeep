#include "base.h"
#define EXTERN extern
#include "Var.h"

extern unsigned char Time_5S_Cnt;
extern unsigned char Time_500MS_Cnt;
extern unsigned char Time_1Min_Cnt;
extern u16 Tim2_wave_delay;
u8 Eletricize_Mode = 0;
//��ʱ�� 1 �жϷ�������  
//0.1�����ж�һ��

/*u8 gaodi_bit=0;	//�ߵ��ź��л���־λ*/
//u8 gaodi_number=0;		//�ߵ��ź��л�����
u8 signal_hz_1s = 2;		//��¼�ź���1KHZ����1S   1��1S   0��1KHZ  2��Ĭ���Ǹߵ�ƽ
volatile static unsigned char gaodi_number __attribute__ ((at(0x1a3)));


void Battery_Status_Operation(void)
{
//	if(signal_hz_1s)   
//		Eletricize_Mode = 1;	
//	else	 
//		Eletricize_Mode = 2;
///***************************************************************************/	
//	switch(Eletricize_Mode)
//	{
//		case 1:		//**********  ���ڳ�� *****************//
////			Breathe_Flag = 1;	// ��������  3Sѭ��
////			LED_3_BLU_OFF;
////			LED_3_GREEN_OFF;
////			LED_3_RED_OFF;
////			MOTOR_OFF;
////			Heat_Wire_OFF;
//			break;
//			
//		case 2:		//*********** ���� ****************//					
////			LED_Breathe_light_ON;      //  �׵Ƴ���
////			Breathe_Flag = 0;
//			break;
//		
//		case 3:		//*********** �͵� ****************//				
//			if(Time_5S_Cnt <= 5)		
//			{					
//				KeyBuzzer(4);	// ������
//				if(Time_500MS_Cnt >= 5)		// �׵���˸2S
//				{
//					LED_Breathe_light_REVERSE;
//					if(Time_500MS_Cnt == 5) 
//					{
//						Time_500MS_Cnt = 0;
//						LED_Breathe_light_OFF;
//					}
//				}
//			}
//			else 
//			{
//				Time_5S_Cnt = 0;
//				Power_Low_33V_Flag = 0;	
//			}
//			if(Time_1Min_Cnt >= 60)    //   1���Ӻ�ػ�
//			{
//				Time_1Min_Cnt = 0;
//				Sleep_Mode();
//			}
//			break;
//		
//		default:
////			LED_Breathe_light_OFF;
//			break;
//	}
}