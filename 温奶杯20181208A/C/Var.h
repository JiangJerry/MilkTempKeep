#ifndef __VAR_H
#define __VAR_H
EXTERN _t_bits maiFlagA;
EXTERN _t_bits maiFlagB;

EXTERN _t_bits LEDG;
EXTERN _t_bits LEDH;
EXTERN u8 SleepCnt;

#define DOWN_22		0
#define IN_22_37 	1
#define IN_37_45	2
#define UP_45		3

#define BUZZER_ON_TIME 	(15)
#define BUZZER_OFF_TIME (8)

#define OPEN5V_ON_TIME	60
#define OPEN5V_OFF_TIME	60

typedef struct{
	 char Times;
	 int OnCnt;
	 int OffCnt;
}_BUZZER;

EXTERN _BUZZER Buzzer;
EXTERN _BUZZER Output5V; 

//sys
//EXTERN u16 sysTime;
//EXTERN unsigned char Time60Min;

//EXTERN unsigned char adcsp;
//EXTERN unsigned char adcsele;


EXTERN unsigned char WorkMode;		//工作模式
EXTERN unsigned char TempSta;
EXTERN unsigned int ntc_val;
EXTERN volatile unsigned int Open5V200msCnt;
EXTERN unsigned char Time1SCnt;
EXTERN u8 SumOfEdge;	//维持温度边缘时间

#define READY_MODE		0
#define RUN_MODE		1
#define CHARGE_MODE		2


void 	Close5V(void);
void 	Open5V(void);
void TaskBuzzer(void);
#endif