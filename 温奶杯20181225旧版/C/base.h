#ifndef __SS350_BASE
#define __SS350_BASE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "HT66F018.h"

#define u8 unsigned char
#define u16 unsigned int
//***************************************************//
typedef unsigned char 	uchar; 				//8bit
typedef unsigned int	uint16; 			//16bit
typedef unsigned long 	uint32; 			//32bit

#define BitOn(x)	(1<<x)

#define true	1
#define false	0

#define LED_ON	1
#define LED_OFF	0

#define DEBUG_SWITCH
#define DEBUG_HOT_ON
#define DEBUG_TX

#define COM1 _pc1
#define COM2 _pc2
#define COM3 _pa1

#define COM1_ON			COM1 = true
#define COM1_OFF		COM1 = false
#define COM1_INPUT  	_pcc1 = true;_pcpu1 = false;
#define COM1_OUTPUT  	_pcc1 = false;_pcpu1 = true;
	
#define COM2_ON			COM2 = true
#define COM2_OFF		COM2 = false
#define COM2_INPUT		_pcc2 = true;_pcpu2 = false;
#define COM2_OUTPUT  	_pcc2 = false;_pcpu2 = true;

#define COM3_ON			COM3 = true
#define COM3_OFF		COM3 = false
#define COM3_INPUT		_pac1 = true;_papu1 = false;
#define COM3_OUTPUT  	_pac1 = false;_papu1 = true;

#define LED_WHITE_2				_pa2	// °×É«µÆ¡ªLED_4
#define LED_WHITE_2_ON			LED_WHITE_2 = false
#define LED_WHITE_2_OFF			LED_WHITE_2 = true
#define LED_WHITE_2_REVERSE		LED_WHITE_2 = !LED_WHITE_2

#define Buzz					_pc0	// ·äÃùÆ÷
#define Buzz_ON					Buzz = true
#define Buzz_OFF				Buzz = false
#define Buzz_REVERSE			Buzz = !Buzz

#define LED_Breathe_light    _pa7	// ºôÎüµÆ
#define LED_Breathe_ON     LED_Breathe_light = false
#define LED_Breathe_OFF     LED_Breathe_light = true
#define LED_Breathe_light_REVERSE	LED_Breathe_light = !LED_Breathe_light
#define KEY		_pa3	//°´¼ü
		
/************************************************/
#define MOTOR			_pa5		
#define MOTOR_ON		MOTOR = true
#define MOTOR_OFF		MOTOR = false
/************************************************/
#define Heat_Wire			_pa0
#define Heat_Wire_ON		Heat_Wire = true
#define Heat_Wire_OFF		Heat_Wire = false
/************************************************/
/************************************************/
#define Out_5V_IO			_pb1
#define Out_5V_IO_Open		_pbc1 = 0;Out_5V_IO = 0
#define Out_5V_IO_Close		_pbc1 = 1
/************************************************/
#define Bat_Status			_pb0

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define STAR_ADC()    {_start=0; _start=1; _start=0;}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct {
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
} _t_bits;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~main.c~~~~~~~~~~~~~~~~~~~~
extern _t_bits maiFlagA;
#define PreHeatFlag				maiFlagA.b0		//********
#define Forward_Value			maiFlagA.b1		// µ¥»÷°´¼ü
#define Key_longPre_cut_click 	maiFlagA.b2
#define Double_Click_Value   	maiFlagA.b3		// Ë«»÷°´¼ü
#define Key_Forward_Long_val	maiFlagA.b4
#define Change_Key_flag			maiFlagA.b5
#define direc_flag				maiFlagA.b6			// Tim_PWM
#define TempDirDnFlag			maiFlagA.b7			// Tim_PWM


extern _t_bits maiFlagB;
#define Forward_Value_Flag	  		maiFlagB.b0
#define Double_Click_Value_Flag	  	maiFlagB.b1
#define	Key_Forward_Long_val_Flag	maiFlagB.b2
#define SleepModeFlag				maiFlagB.b3 
#define EnADCFlag					maiFlagB.b4
#define TaskADRdyFlag				maiFlagB.b5
#define ChargIngFlag				maiFlagB.b6
#define TaskBatRdyFlag      		maiFlagB.b7

#define ADC_NTC        			0x12	   	//ºìÍâÎÂ¶È²âÊÔÖµ
#define ADC_AROUD        		0x13	   	//ÊÒÎÂµç×è²âÊÔÖµ
#define ADC_BAT					0x15		//µç³Ø·ÖÑ¹²âÊÔÖµ
extern _t_bits LEDG;
#define Breathe_Flag	  		LEDG.b0
#define BATLow3p5Flag		  	LEDG.b1
#define BATUp4p1Flag	  		LEDG.b2
#define TaskBuzzerRdy       	LEDG.b3
#define NormalWinterFlag		LEDG.b4
#define TaskKeyRdy	 			LEDG.b5
#define LED_direc 				LEDG.b6
#define TempDir		 			LEDG.b7		//ÎÂ¶ÈÉÏÉý»òÕßÏÂ½µ



#define ChargeFullFlag 			LEDH.b0
#define Task500msFlag      		LEDH.b1
#define Task5SFlag		     	LEDH.b2

#define Pull_One 		200
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern void InitSys(void);
extern void Run_Operation(void);
extern void LED_Breathe(void);
extern void KeyBuzzer(char Buzz_1);
extern void GetPTCValu();
extern void Get_Battery_Valu(void);
extern void Sleep_Mode(void);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif //__SS350_BASE
