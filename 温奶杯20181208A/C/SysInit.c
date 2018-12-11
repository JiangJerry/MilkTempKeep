#include "base.h"
#define EXTERN extern
#include "Var.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct
{
	u16 Min;
	u16 Max;
} __TEMP;
extern unsigned int PWM_VLAUE;
extern unsigned int k;
extern unsigned int k2;
extern unsigned char adcsp_2;
extern unsigned char adcsele_2; 
/*****************************************************
***************  Out = 0;	**************************
***************  In = 1;	**************************
******************************************************/

void InitSys(void)		//   OUT = 0     IN = 1
{
// System clock setting
	_smod = 0b11100001;					// fsys=fH=8MHz; IDLE mode Disable
	// Watchdog Timer Setting  
	_wdtc = 0b01010101;					// set WDT Timeout period= 2^16/fsub=2^16/32.768kHz=2sec; Enable WDT
    _lvrc = 0b00110011; //2.55V
    _cpc = 0x08;
/****** buzzer ***************/    
    _pcc0 = 0;
    _pcpu0 = 1;
    Buzz_OFF;
//*********** LED_IO *********************//    
    _pcc1 = 0;
    _pcpu1 = 1;
    LED_3_GREEN_OFF;  
    _pcc2 = 0;
    _pcpu2 = 1;
    LED_3_BLU_OFF;   
    _pac1 = 0;
    _papu1 = 1;
    LED_WHITE_1_OFF;
    
    _pac2 = 0;
    _papu2 = 1;
	LED_WHITE_2_OFF;	

    _pac7 = 0;
    _papu7 = 1;
	LED_Breathe_OFF;
			
    _pbc2 = 0;      
    _pbpu2 = 1;
    LED_3_RED_OFF;
/********** Read Bat_status **************************************/
	_pbc0 = 1;      
    _pbpu0 = 0;
    /*Bat_Status = Bat_Satiation;*/
/************ 风扇 ***************************************/
    _pac5 = 0;
    _papu5 = 1;
	MOTOR_OFF;	
    //******** Battery_AN5 ***************/
    _acerl = 0;
    _acerl |= 0b00101000;	//********* PA4 && PA6 = AD输入
/*    _adcr0 = ADC_BATTERY;*/
    _adcr0 = ADC_NTC;		//	011：AN3
    _adcr1 = 0x06;			//  110：fSYS /64
    STAR_ADC();
/************** Open_Out_5V *****************************************/
	_pbc1 = 0;      
	_pbpu1 = 0;
	Out_5V_IO_Close;
/******************************************************************/
//adc init   
//********** KEY_IO  *****************************//
	_pac3 = 1;
	_papu3 = 1;
	_pawu3 = 1;
	KEY = 1;
//*********** Heat_Wire ***********************************//    
	_pbc2 = 0;
	_pbpu2 = 1;
	Heat_Wire_OFF;	  
}

void Ram_Init(void)
{
	Forward_Value = 0;
	Forward_Value_Flag = 0;
	Double_Click_Value = 0;	
	Double_Click_Value_Flag = 0;
	Key_Forward_Long_val = 0;
	Key_Forward_Long_val_Flag = 0;
	Power_ON_OFF_flag = 0;
	WorkMode = READY_MODE;
	
	H_45C_Flag = 0;
	Breathe_Flag = 0;
	Buzzer.Times = 0;
	Buzz_OFF;
	TaskKeyRdy = false;
	Out_5V_IO_Close;
	k = 0;
	k2 = 0;
	
	TempSta = DOWN_22;
	Close_5V_Flag = false;
	Output5V.Times = 0;
	DCInFlag = false;
	NormalWinterFlag = false;
	ntc_val = 0;
	Open5V200msCnt = 0;	
	direc_flag = true;			//为真时呼吸灯才正常
	PreHeatFlag = false;		//预热时间
	SleepCnt = 0;
	SumOfEdge = 0;
}