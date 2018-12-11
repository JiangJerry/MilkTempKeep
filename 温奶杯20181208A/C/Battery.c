#include "base.h"
#define EXTERN extern
#include "Var.h"

extern unsigned char Time_5S_Cnt;
extern unsigned char Time_500MS_Cnt;
extern unsigned char Time_1Min_Cnt;
extern u16 Tim2_wave_delay;
u8 Eletricize_Mode = 0;
//定时器 1 中断服务程序⑥  
//0.1毫秒中断一次

/*u8 gaodi_bit=0;	//高低信号切换标志位*/
//u8 gaodi_number=0;		//高低信号切换次数
u8 signal_hz_1s = 2;		//记录信号是1KHZ还是1S   1是1S   0是1KHZ  2是默认是高电平
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
//		case 1:		//**********  正在充电 *****************//
////			Breathe_Flag = 1;	// 开呼吸灯  3S循环
////			LED_3_BLU_OFF;
////			LED_3_GREEN_OFF;
////			LED_3_RED_OFF;
////			MOTOR_OFF;
////			Heat_Wire_OFF;
//			break;
//			
//		case 2:		//*********** 满电 ****************//					
////			LED_Breathe_light_ON;      //  白灯常亮
////			Breathe_Flag = 0;
//			break;
//		
//		case 3:		//*********** 低电 ****************//				
//			if(Time_5S_Cnt <= 5)		
//			{					
//				KeyBuzzer(4);	// 鸣三声
//				if(Time_500MS_Cnt >= 5)		// 白灯闪烁2S
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
//			if(Time_1Min_Cnt >= 60)    //   1分钟后关机
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