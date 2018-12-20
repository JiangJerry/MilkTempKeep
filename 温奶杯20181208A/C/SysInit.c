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
    _pcc2 = 0;
    _pcpu2 = 1;
    _pac1 = 0;
    _papu1 = 1;
    _pac2 = 0;
    _papu2 = 1;
	LED_WHITE_2_OFF;	
	COM1_OUTPUT;
	COM2_OUTPUT;
	COM3_OUTPUT;
    _pac7 = 0;
    _papu7 = 1;
	LED_Breathe_OFF;			
/********** Read Bat_status **************************************/
	_pbc0 = 1;      
    _pbpu0 = 0;	//一定不能上拉
    /*Bat_Status = Bat_Satiation;*/
/************ 风扇 ***************************************/
    _pac5 = 0;
    _papu5 = 1;
	MOTOR_OFF;	
	
	_pac4 = 1;
	_papu5 = 0;
    //******** Battery_AN5 ***************/
    _acerl = 0;
    _acerl |= 0b00101100;	//********* PA4 && PA6 && PB2 = AD输入
    _adcr0 = ADC_NTC;		//	011：AN3
    _adcr1 = 0x06;			//  110：fSYS /64
    STAR_ADC();
/************** Open_Out_5V *****************************************/
	_pbc1 = 0;      
	_pbpu1 = 0;				//一定不能上拉
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

void VarInit(void)
{
	Forward_Value = 0;
	Forward_Value_Flag = 0;
	Double_Click_Value = 0;	
	Double_Click_Value_Flag = 0;
	Key_Forward_Long_val = 0;
	Key_Forward_Long_val_Flag = 0;
	TempDirDnFlag = 0;
	WorkMode = READY_MODE;
	
	Breathe_Flag = 0;
	Buzzer.Times = 0;
	Buzz_OFF;
	TaskKeyRdy = false;
	Out_5V_IO_Close;
	k = 0;
	k2 = 0;
	
	TempSta = DOWN_22;
	Output5V.Times = 0;
	ChargIngFlag = false;
	NormalWinterFlag = false;
	ntc_val = 0;
	Open5V200msCnt = 0;	
	direc_flag = true;			//为真时呼吸灯才正常
	PreHeatFlag = false;		//预热时间
	SleepCnt = 0;
	SumOfEdge = 0;
	LEDSta[LED_RED_INDEX] = 0;
	LEDSta[LED_BLUE_INDEX] = 0;
	LEDSta[LED_WHITE_INDEX] = 0;
	LEDSta[LED_GREEN_INDEX] = 0;
			
	BatSta = BATTRY_NORMAL;		//电池检测重新开始
	BATLow3p5Flag = false;		//低电压标志清零
	ChargeFullFlag = false;		//充满标志清零
	LowBatShutDnSCnt = 0;		//低电压时间清零
	BATChargeFullCnt = 0;		//充满时间清零

	IntCnt = 0;
}