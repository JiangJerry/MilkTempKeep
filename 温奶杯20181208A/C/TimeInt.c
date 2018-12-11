#include "base.h"
#define EXTERN extern
#include "Var.h"

#define INIT_NO_DETECT_TIME	3
volatile static unsigned char Time4mSCnt __attribute__ ((at(0x1d5)));
//volatile static unsigned char Time1SCnt __attribute__ ((at(0x1d6)));
volatile static unsigned char Time1SClearCnt __attribute__ ((at(0x1d7)));
volatile static unsigned char Time_3S_Cnt __attribute__ ((at(0x1d8)));
volatile static unsigned char Time_10Min_Cnt __attribute__ ((at(0x1d9)));
volatile static unsigned char Time_30Min_Cnt __attribute__ ((at(0x1da)));

unsigned char Time500mSCnt = 0;
unsigned char Time_5S_Cnt = 0;
unsigned char TimeMinCnt = 0;

/********************************************************/
volatile unsigned int PWMCnt = 0;
volatile unsigned int HuxiCnt = 0;
volatile unsigned int PWMValu = 10;

#define HUXI_VAL 1300

/***************** ������ **********************************************/
void LED_Breathe(void)
{
	if((Breathe_Flag == 1))
	{
	    PWMCnt++;
	    HuxiCnt++;
	    if(PWMCnt == PWMValu)      //�ж��Ƿ��˵���LED��ʱ��
	    {
	        LED_Breathe_ON;       	//����LED
	    }
	    if(PWMCnt == 10)             //��ǰ���ڽ���
	    {
	        LED_Breathe_OFF;      	//Ϩ��LED
	        PWMCnt = 0;              //���¼�ʱ
	    }
	    if((HuxiCnt == HUXI_VAL) && (direc_flag == 0))
	    {                               //ռ�ձ�����10%
	        HuxiCnt = 0;
	        PWMValu++;
	        if(PWMValu == 10)          //ռ�ձȸ��ķ���
	            direc_flag = 1; 
	    }
 	    if((HuxiCnt == HUXI_VAL) && (direc_flag == 1))
	    {                               //ռ�ձȼ���10%
	        HuxiCnt = 0;
	        PWMValu--;
	        if(PWMValu == 1)          //ռ�ձȸ��ķ���
	            direc_flag = 0; 
	    } 	
	}
	else 
	{
		LED_Breathe_OFF;
	}	
}

/*****************************************************************/
extern unsigned int ntc_val;		
//ֵԽС���׵�ƽʱ��ԽС���ߵ�ƽʱ��Խ����MOS��ͨԽ��
void Heat_Wire_Duty(u16 step_15)
{	
	_tm0al = ((step_15&0xff));
	_tm0ah = ((step_15>>8));	
}
/*********************************************************/
void Timer_Init(void)
{               
//time0 PWM
	_wdtc = 0xA8;  	
	_pac0 = 0;//PA0 output--PWM�����	
	_t0cp = 1;//TP0(PA0)���ſ���λʹ��

	//ϵͳʱ��
//	_smod = 0x01;//fsys = fH   bit0 = 1
	
	//-------STM init-------
	//�������ͱȽ���
	_tm0al = 0x00;//ռ�ձ��趨
	_tm0ah = 0x00;//ռ�ձ��趨
	_tm0dl = 0x00;
	_tm0dh = 0x00; 
	_tm0rp = 0x08;//�����趨, 8λ����_tm0dh�Ƚ�
	_tm0c1 = 0xA4;//�趨PWMģʽ��CCRP - ���ڣ�CCRA - ռ�ձ�
	_tm0c0 = 0x28;//����timer0, ����ʱ��ѡ��fsys
/*********************  TM_2 INIT ******************************************************/
	_t2cp = 0;
	_tm2al = 0xc8;     //  03e8  500us  07d0 = 1ms   c8 = 100us
	_tm2ah = 0x00;
	_tm2rp = 0xff;

	_tm2c1 = 0b11000001;     // ƥ���� A 
	_tm2c0 = 0b00001000;		// Tm2 ���ؿ���λ  bit3 = 1  Open
	
	_t2af = 1;		// �ж������־λ
	_t2ae = 1;		// a ƥ���жϿ���λ
	_mf2e = 0;		//�๦���ж�2����λʹ��	
	
	_mf1f = 1;
	_mf1e = 1;
		
/**************	�ⲿ�ж� ************************************************************/ 
	_int0s0 = 1;_int0s1 = 1;			// 10 �½��ش���   11 ˫�ش���
    _int0e = true;  
	_emi = 1;      
	Time1SClearCnt = 0;      
}

/*********************************************************************************************************
** ��������: ISR_tmr2
** ��������: ��ʱ��2�жϷ�����������ˮ�ó�ˮ��
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: �� .100us�ж�
********************************************************************************************************/
u16 Time100uSCnt = 0;
u16 ChargPlusDly = 0;
u8 LEDSta[4];
/**********************************************************

 * ����: void Led2Disp_Scan(void)
 * ����: led2�Ͱ���ɨ�躯��
 * �β�: ��
 * ����: ��
 * ˵��: ��
 **********************************************************/
unsigned char scan_num = 0;//ɨ��״̬����
u8 IntCnt = 0;
void LEDDispKeyScan(void)
{
	scan_num++;	

	switch(scan_num)	
	{
		case 1://״̬1ɨ��LED��
			if(LEDSta[0] == 1)
			{
				COM3_INPUT;
				COM1_OFF;
				COM2_ON;				
				COM1_OUTPUT;
				COM2_OUTPUT;
			}
			break;
		case 2:
			if(LEDSta[1] == 1)
			{				
				COM3_INPUT;
				COM1_ON;
				COM2_OFF;
				COM1_OUTPUT;
				COM2_OUTPUT;
			}
			break;
		case 3:	
			if(LEDSta[2] == 1)
			{
				COM1_INPUT;
				COM2_ON;
				COM3_OFF;
				COM2_OUTPUT;
				COM3_OUTPUT;
			}
			break;
      	case 4:
			if(LEDSta[3] == 1)
			{
				COM1_INPUT;
				COM2_OFF;
				COM3_ON;
				COM2_OUTPUT;
				COM3_OUTPUT;
			}
 			scan_num = 0;
	      	break;
      default:
	      break;
   }
}

#define CNT_4MS		40 	//4ms
#define CNT_1S		250	//1s
#define PRE_HEAT	10
void __attribute((interrupt(0x10))) ISR_tmr2 (void)
{
	static u8 i =0;
	_t2af = 0;
	Time100uSCnt++;	
	LED_Breathe();		//������
	if(Time100uSCnt >= CNT_4MS)	//4ms
	{
		Time100uSCnt = 0;
		TaskBuzzerRdy  = true;
		ChargPlusDly++;
		Time4mSCnt++;
		if((Time4mSCnt & 0x02) == 0x02)	//����8MS����ɨ��
			TaskKeyRdy = true;
		LEDDispKeyScan();
		Open5V200msCnt++;
		Time500mSCnt++;		
		if(Time4mSCnt >= CNT_1S)		//1s
		{
			Time4mSCnt = 0;
			Time1SCnt++;
			Time1SClearCnt++;
			SumOfEdge++;
			if(WorkMode == READY_MODE)	//����ģʽ��
			{
				SleepCnt++;
			}
		}
		if(Time500mSCnt >= 125)			//500ms
		{			
			Task500msFlag = true;
			Time500mSCnt = 0;
		}
		if(Time1SClearCnt >= 5)			//5S�����ʱ
		{
			Time1SClearCnt = 0;
			IntCnt = 0;
			DCInFlag = false;
		}
		if(Time1SCnt >= PRE_HEAT)		//1min
		{
			if(PreHeatFlag)	
				PreHeatFlag = false;
			TimeMinCnt++;
		}			
	} 	
}

/*********************************************************************************************************/

void __attribute((interrupt(0x0c))) ISR_tmr0 (void)
{
/*	_t0af=0;*/									// clr TM0 Comparator A match interrupt req flg.
	_t0pf = 0;//���жϺ�������жϱ�־
	_nop();	
}

/*********************************************************************************************************
** ��������: ISR_int0
** ��������: �ⲿ�ж�0����⵽��ź������ź�
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: ��
********************************************************************************************************/
void __attribute((interrupt(0x04))) ISR_int0 (void)
{
    _int0f=0;
	if((ChargPlusDly > 170 ) && (ChargPlusDly < 350))	//��⵽�׵�ƽ,��ʱ���������ֵ������ 1100ms����
	{		
		if(IntCnt++ >= 2)								//2�μ����Ч���������׳��ֿ��ػ�ʱ�ĵ׵�ƽ����
			DCInFlag = true;
		Time1SClearCnt = 0;								//�����DC�����ʱ
	}
	SleepCnt = 0;										//��ֹ����DC��������嵼��һ����һ��˯�����
	ChargPlusDly = 0;
}