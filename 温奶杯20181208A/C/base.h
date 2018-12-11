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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEBUG_SWITCH
#define DEBUG_HOT_ON
#define DEBUG_TX

#define COM1 _pc0
#define COM2 _pa2
#define COM3 _pa7


#define COM1_ON		COM1 = true
#define COM1_OFF	COM1 = false
#define COM1_INPUT  _pcc0 = true;_pcpu0 = false;
#define COM1_OUTPUT  _pcc0 = false;_pcpu0 = true;
	
#define COM2_ON		COM2 = true
#define COM2_OFF	COM2 = false
#define COM2_INPUT	_pac2 = true;_papu2 = false;
#define COM2_OUTPUT  _pac2 = false;_papu2 = true;

#define COM3_ON		COM3 = true
#define COM3_OFF	COM3 = false
#define COM3_INPUT	_pac7 = true;_papu7 = false;
#define COM3_OUTPUT  _pac7 = false;_papu7 = true;


//~~~IO~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define LED_WHITE_1			_pa1	// °×É«µÆ¡ªLED_3
#define LED_WHITE_1_ON		LED_WHITE_1 = false
#define LED_WHITE_1_OFF		LED_WHITE_1 = true
#define LED_WHITE_1_REVERSE	LED_WHITE_1 = !LED_WHITE_1

#define LED_WHITE_2			_pa2	// °×É«µÆ¡ªLED_4
#define LED_WHITE_2_ON		LED_WHITE_2 = false
#define LED_WHITE_2_OFF		LED_WHITE_2 = true
#define LED_WHITE_2_REVERSE	LED_WHITE_2 = !LED_WHITE_2

#define Buzz					_pc0	// ·äÃùÆ÷
#define Buzz_ON					Buzz = true
#define Buzz_OFF				Buzz = false
#define Buzz_REVERSE			Buzz = !Buzz

#define LED_3_BLU			_pc2	// ÈýÉ«µÆ  À¶
#define LED_3_BLU_ON		LED_3_BLU = false
#define LED_3_BLU_OFF		LED_3_BLU = true
#define LED_3_BLU_REVERSE	LED_3_BLU = !LED_3_BLU
		
#define LED_3_GREEN			_pc1	// ÈýÉ«µÆ  ÂÌ
#define LED_3_GREEN_ON		LED_3_GREEN = false
#define LED_3_GREEN_OFF		LED_3_GREEN = true
#define LED_3_GREEN_REVERSE	LED_3_GREEN = !LED_3_GREEN

#define LED_3_RED			_pb2	// ÈýÉ«µÆ  ºì
#define LED_3_RED_ON		LED_3_RED = false
#define LED_3_RED_OFF		LED_3_RED = true
#define LED_3_RED_REVERSE	LED_3_RED = !LED_3_RED


#define LED_Breathe_light    _pa7	// ºôÎüµÆ
#define LED_Breathe_ON     LED_Breathe_light = false
#define LED_Breathe_OFF     LED_Breathe_light = true
#define LED_Breathe_light_REVERSE	LED_Breathe_light = !LED_Breathe_light
#define KEY		_pa3	//°´¼ü
		
/************************************************/
#define Power_ON		Power_ON_OFF_flag = 1
#define Power_OFF		Power_ON_OFF_flag = 0
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


/************************************************/
//#define CONFIG_IN_NUM	0x01
//#define KEY_MODE		0
//
//#define LONG_KEY       	10

//#define PWM_MAX         8000
//#define PWM_MIN         2048
//#define PWM_MID         5200
//#define AdjA        22  //22  //55 //65     //5  20
//#define AdjB        27 // 28  //75 //75    //3  16
//#define AdjC        8  // 8   //25 //15      //0  2

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

/***************************************************/

/**************************************************/

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
#define Power_ON_OFF_flag		maiFlagA.b7			// Tim_PWM


extern _t_bits maiFlagB;
#define Forward_Value_Flag	  		maiFlagB.b0
#define Double_Click_Value_Flag	  	maiFlagB.b1
#define	Key_Forward_Long_val_Flag	maiFlagB.b2
#define SleepModeFlag				maiFlagB.b3 
#define Open_5V_Flag				maiFlagB.b4
#define Close_5V_Flag				maiFlagB.b5
#define DCInFlag					maiFlagB.b6
#define SleepPrepareFlag      		maiFlagB.b7

#define ADC_NTC        			0x13	   // NTCÖµ
#define ADC_BATTERY        		0x15	   // Battery_dat
extern _t_bits LEDG;
#define Breathe_Flag	  		LEDG.b0
#define Power_Low_33V_Flag	  	LEDG.b1
#define Power_42V_Flag	  		LEDG.b2

#define TaskBuzzerRdy       	LEDG.b3
#define NormalWinterFlag		LEDG.b4
#define TaskKeyRdy	 			LEDG.b5
#define LED_direc 				EDG.b6
#define Opend_5V_flag 			LEDG.b7



#define H_45C_Flag     LEDH.b0
#define Task500msFlag      LEDH.b1
#define Bat_Power_Zero_flag      LEDH.b2
#define Bat_Power__flag      LEDH.b3

#define Pull_One 		200
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern void InitSys(void);
extern void Ram_Init(void);
extern void Timer_Init(void);
extern void GetKey(void);
extern void Run_Operation(void);
extern void LED_Breathe(void);
extern void KeyBuzzer(char Buzz_1);
extern void Buzz_Runing_1ms_Int(void);
extern void GetPTCValu();
extern void Get_Battery_Valu(void);
extern void Sleep_Mode(void);
extern void Battery_Status_Operation(void);
extern void Heat_Wire_Duty(u16 step_15);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif //__SS350_BASE
