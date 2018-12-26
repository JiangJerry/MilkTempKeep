#ifndef __VAR_H
#define __VAR_H
EXTERN _t_bits maiFlagA;
EXTERN _t_bits maiFlagB;

EXTERN _t_bits LEDG;
EXTERN _t_bits LEDH;
EXTERN u8 SleepCnt;

#define DOWN_30		0
#define IN_30_45 	1
#define STAY_45		2
#define UP_45		3

#define WAIT_TIME 	120			//ת���ȴ�ʱ��

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
#define LED_NUM	4

EXTERN u8 LEDSta[LED_NUM];

#define LED_RED_INDEX	0
#define LED_BLUE_INDEX	1
#define LED_WHITE_INDEX	2
#define LED_GREEN_INDEX	3

EXTERN unsigned char WorkMode;		//����ģʽ
EXTERN unsigned char TempSta;
EXTERN int ntc_val;
EXTERN volatile unsigned int Open5V200msCnt;
EXTERN unsigned char Time1SCnt,Sys1SCnt,LowBatShutDnSCnt;
EXTERN u8 BATChargeFullCnt;		//����ʱʱ��
EXTERN u8 SumOfEdge;			//ά���¶ȱ�Եʱ��
EXTERN u8 BatSta;
EXTERN u8 IntCnt;
EXTERN u8 Stay45Cnt;

#define READY_MODE		0
#define RUN_MODE		1

#define PRE_HEAT		10		//��λ���� ���������¶�ʱ��
#define CHARG_FULL_TIME	3		//��λ������ ����ʱ��

#define BATTRY_NORMAL	0		//����������
#define BATTRY_LOW		1		//�׵�ѹ
#define BATTRY_CHARGE	2		//�����
#define BATTRY_FULL		3		//����

#define BATTRY_WAIT     4
#define BAT_LOW_TIP		60		//�׵�ѹ����ʱ��

void 	Close5V(void);
void 	Open5V(void);
void 	TaskBuzzer(void);
void 	Sleep(void);
void 	GetAroudTempValu(void);
void 	GetBatValu(void);
void 	TaskBatOp(void);
void  	TaskKey(void);
void 	VarInit(void);
void 	TimerInit(void);
void	HeatCtrDuty(u16 step_15);
void 	TaskFunc(void);

#endif