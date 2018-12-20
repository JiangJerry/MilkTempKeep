/**************************************************************************/

#include "base.h"
#define EXTERN extern
#include "Var.h"

/**********************************/
volatile static unsigned char Forward_Buf __attribute__ ((at(0x1dd)));
volatile static unsigned char Pre_Forward_Buf __attribute__ ((at(0x1de)));
volatile static unsigned char Key_Forward_Time __attribute__ ((at(0x1df)));
volatile static unsigned int Key_Forward_Delay __attribute__ ((at(0x1e0)));
volatile static unsigned int Key_Forward_Ms __attribute__ ((at(0x1e2)));
volatile static unsigned char Key_Forward_Mode __attribute__ ((at(0x1e4)));
volatile static unsigned int Double_Click_timer __attribute__ ((at(0x1e5)));
volatile static unsigned char Change_Key_Mode __attribute__ ((at(0x1e7)));
volatile static unsigned int Key_LongPre_timer __attribute__ ((at(0x1e8)));

/*********************************/
/**********************************/
extern unsigned char Key_ms;

/*******************************/
#define KEY_LONG	150
#define MAX_EFFECT_TIME	200	//2S
/********* 按键扫描 ********************************/
void TaskKey(void)
{
	if(TaskKeyRdy)
	{
		TaskKeyRdy = false;	
/************ Forward Key *************************************************/		
		if(KEY == 0) 
		{
			Forward_Buf=1;	
		}
		else 
		{
			Forward_Buf = 0;		
		}
		/*********** 单击 ******************************/
		if(Forward_Buf != Pre_Forward_Buf)
		{
			Key_Forward_Time++;
			if( Key_Forward_Time > 5)
			{
				if((Forward_Buf == 0)&&(Key_longPre_cut_click == 0))
				{	
					Change_Key_flag = 1;														
					Change_Key_Mode++;
										
				}
			/********************************************************************/	
				Pre_Forward_Buf = Forward_Buf;
			}
		}
		else 
		{
			/********************* Double_Click *******************************************/
			if((Double_Click_timer > MAX_EFFECT_TIME)) 
					Change_Key_flag = 0;
			if((Change_Key_flag == 1)&&(Double_Click_timer <= MAX_EFFECT_TIME))		
			{
				Double_Click_timer++;
				if(Change_Key_Mode == 3)
				{
					Change_Key_flag = 0;
					Change_Key_Mode = 0;
					Double_Click_timer = 0;
					Forward_Value = 0;
					Double_Click_Value = 1;	
				}
			}
			else
			{
				Change_Key_flag = 0;
				Change_Key_Mode = 0;
				Double_Click_timer = 0;
				Forward_Value = 0;
				Double_Click_Value = 0;
			}
			/*******************************************************************************************/			
		  /********** 长按3s *****************************/
			Key_Forward_Time = 0;
			if(Forward_Buf == 1)
			{
				if(Key_Forward_Delay <= KEY_LONG)	// 3s
				{
					Key_Forward_Delay++;
					if(Key_Forward_Delay == KEY_LONG)
					{
						Key_longPre_cut_click = 1;
						Double_Click_Value = 0;
						Forward_Value = 0;
						Key_Forward_Long_val = 1;	
					}
				}				
				/******************************/
				else Key_Forward_Ms++;
			}
			else 
			{
				Key_longPre_cut_click = 0;
				Key_Forward_Ms = 0;
				Key_Forward_Delay = 0;
			}
		}					
/*********************************************************************/		
	}
/************ 按键操作****************************************/
/************ One_Press *************************************/
	if( Forward_Value == 1)  // Click
	 {
	 	Forward_Value = 0;
	 	Double_Click_Value_Flag = 0;
		Key_Forward_Long_val_Flag = 0;
		Forward_Value_Flag = 1;
	 }
	 else
/***********************************************************/
	if( Key_Forward_Long_val == 1)  // Press_Long
	 {		
		Key_Forward_Long_val = 0;
		Forward_Value_Flag = 0;
		Double_Click_Value_Flag = 0;
		Key_Forward_Long_val_Flag = 1;
	 }
	 else
/************** Double_Click ***********************************/
	if(Double_Click_Value == 1)		// Double Click
	{
		Double_Click_Value = 0;
		Forward_Value_Flag = 0;
		Key_Forward_Long_val_Flag = 0;
		Double_Click_Value_Flag = 1;		
	}	
//	if(Bat_Power__flag == 1)
//	{
//		Key_Forward_Long_val = 0;
//	}
}
