#include "base.h"
#define EXTERN extern
#include "Var.h"

#define MIN_VALU		500		//��Сֵ�����������·���ܵ��·Ŵ��������,С��0.4V������˵��358����

#define Temp_20C		1820//2100		//   22c   11.4239  1912
#define Temp_22C		1911//2100		//   22c   11.4239  1912
#define Temp_37C		2396//2200		//   37c   6.0053   2560
#define Temp_45C		2478//2400		//   45c   4.3558   2853

#define Bat_4_2V		1789		//4.2v ��ֵ  
#define Bat_3_3V		1410		//3.3V ��ֵ

extern unsigned int ntc_val;
int k;
unsigned char adcsp = 0;
unsigned int k1,k2; 
unsigned char adcsp_2 = 0;
unsigned char adcsp_1 = 0;

#define MAX_TEMP_ARRAY	100
//�����¶�ֵ
//0-9
//10-19						  	
//20-29
//30-39
//40-49
//...
//90-99
const u16 AroundTemp[MAX_TEMP_ARRAY] = {2537,2488,2438,2389,2339,2289,2239,2190,2140,2091,
						    	        2041,1993,1944,1896,1848,1801,1755,1709,1663,1618,
						    			1574,1531,1488,1446,1405,1365,1325,1287,1249,1212,
						    			1176,1140,1106,1072,1040,1008,977,946,917,888,
						    			860,833,807,781,757,733,709,687,665,644,
						    			623,603,584,565,547,530,513,497,481,466,
						    			451,437,423,409,397,384,372,360,349,338,
						    			328,317,308,298,289,280,271,263,255,247,
						    			240,233,226,219,212,206,200,194,188,183,
						    			177,172,167,162,158,153,149,144,140,136};
//45��ʱ�������¶�						
//0-9
//10-19
//20-29
//30-39
//40-49 MAX_TEMP_ARRAY
#define POSITIVE_TEMP_ARRAY		81		//0-45��
const int ObjPositiveTemp[POSITIVE_TEMP_ARRAY] = {1117,1097,1077,1057,1037,1017,996,975,954,933,
						 			 			911,889,867,845,823,800,777,754,730,707,
						 			 			683,659,634,609,585,559,534,508,482,456,
						 			 			430,403,376,349,321,293,265,237,208,179,
						 			 			150,121,91,61,31,0,-31,-62,-94,-125,
						 			 			-157,-189,-210,-240,-270,-317,-347,-380,-410,-460,
						 			 			-489,-510.-540,-570.-600,-669,-700,-710,-740,-770,
						 			 			-857,-880,-910,-940,-970,-1054,-1080,-1120,-1150,
						 			 			-1259};	//Ŀ�����¶�
#define NAGATIVE_TEMP_ARRAY		46		//0-45��
						 			 
//const u16 ObjPositiveTemp[POSITIVE_TEMP_ARRAY] = {1117,1097,1077,1057,1037,1017,996,975,954,933,
//						 			 911,889,867,845,823,800,777,754,730,707,
//						 			 683,659,634,609,585,559,534,508,482,456,
//						 			 430,403,376,349,321,293,265,237,208,179,
//						 			 150,121,91,61,31,0};	//Ŀ�����¶�
						 
#define FOUR_DEGREES		150		//4�Ȳ�ֵ					  							  
#define BIAS_VALU			2047	//ƫ�õ�ѹ
#define TEMP_ADD_H			600		//��������ֵ��55�ȣ��ߵ�λ
#define TEMP_ADD_L			1000	//�������µ͵�	
#define TEMP_DEC			50		//���£�40��
u8 AroudTempIndex = 0;				//��������ֵ					  

//����Ŀ���¶�ֵ
void GetPTCValu(void)  
{
	unsigned char i;
	unsigned int min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1c0)));  // 1C0-1d5

	_adcr0 = ADC_NTC;			//	NTC������
	STAR_ADC();
	while(_eocb);				// ת��������־
    
    k = _adrh;
    k <<= 8;
    k += _adrl;
    if(adcsp < 10) 
    	i = adcsp;
    else  
    	i = adcsp - 10;
    adcBuf[i] = k;
    if(adcsp < 11) 				//�ɼ�10������ 
    {
    	adcsp++; 
    	return;
    }
    adcsp = 0;
    k = adcBuf[0];
    min = k;
    max = k;
    for(i=1;i<10;i++)
    {
        if(min > adcBuf[i])  
        	min = adcBuf[i];
        if(max < adcBuf[i])  
        	max = adcBuf[i];
        k += adcBuf[i]; 
    }
    k -= min; k -= max; k >>= 3;			//����10�Σ�ȥ�����ֵ����Сֵ��Ȼ��ƽ��    
/*******************************************************************************************/
//	���Թ�ϵ����
    if(k > BIAS_VALU)
	{
		k = k - BIAS_VALU;
	}
	else
	{
		k = 0;
	}
	if(TempDir)											//�½�������?ʵ�ֺ����ͻ�
	{
		max = ObjPositiveTemp[AroudTempIndex] + TEMP_ADD_H;	//�����Ժ��ֵ
		if(k >= max)									//Ŀ���¶�����ֵ������40�ȣ�-150��5��
		{
	    	TempSta = UP_45;
	    	TempDir = true;								//�½�����
		}
		else
		{
	    	TempDir = false;							//תΪ����
		}
    	TempDirDnFlag = true;
	}
    else
    {
    	max = ObjPositiveTemp[AroudTempIndex] + TEMP_ADD_L;
    	if(k >= max)	//Ŀ�����45��תΪ����������ˮ���Ա�׼
    	{
	    	TempDir = true;								//�������
    	}
    	else
    	{
    		if(TempDirDnFlag)							//ʵ�������ͻ�
    		{
	    		if(k >= ObjPositiveTemp[AroudTempIndex])		//�̵ƶ���
	    		{
			    	TempSta = IN_22_37;
			    	TempDirDnFlag = true;				//ת�̵��¶�
			    	ntc_val = (k - ObjPositiveTemp[AroudTempIndex]) << 4;	//��������ֵ
			    	if(ntc_val > 2048)
			    		ntc_val = 2048;	
	    		}
	    		else
	    		{
			    	TempSta = DOWN_22;
			    	TempDirDnFlag = false;				//ת�����¶�	
			    	ntc_val = 2048;    			
	    		}
		    	TempDir = false;						//����½�����			
    		}
    		else
    		{
    			max = ObjPositiveTemp[AroudTempIndex] + TEMP_DEC;
	    		if(k >= max)			//�̵��¶�
	    		{
			    	TempSta = IN_22_37;
			    	TempDirDnFlag = true;				//ת�̵��¶�
	    		}
	    		else
	    		{
			    	TempSta = DOWN_22;					//ת�����¶�
			    	TempDirDnFlag = false;
	    		}    			
		    	TempDir = false;						//����½�����			
		    	ntc_val = 2048;    			
    		}    		    		    		
    	}
    }        		
}
//����ֵ
void GetAroudTempValu(void)  
{
	unsigned char i;
	unsigned int min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1eb)));  // 1eb -1d5

	_adcr0 = ADC_AROUD;			//����
	STAR_ADC();
	while(_eocb);				// ת��������־
    
    k2 = _adrh;
    k2 <<= 8;
    k2 += _adrl;
    if(adcsp_2 < 10) 
    	i = adcsp_2;
    else  
    	i = adcsp_2 - 10;
    adcBuf[i] = k2;
    if(adcsp_2 < 11) 				//�ɼ�10������ 
    {
    	adcsp_2++; 
    	return;
    }
    adcsp_2 = 0;
    k2 = adcBuf[0];
    min = k2;
    max = k2;
    for(i=1;i<10;i++)
    {
        if(min > adcBuf[i])  
        	min = adcBuf[i];
        if(max < adcBuf[i])  
        	max = adcBuf[i];
        k2 += adcBuf[i]; 
    }
    k2 -= min; k2 -= max; k2 >>= 3;			//����10�Σ�ȥ�����ֵ����Сֵ��Ȼ��ƽ��        
	for(i=0;i <MAX_TEMP_ARRAY;i++)			//����Ӧֵ�������¾������¿���
	{
		if(k2 >= AroundTemp[i])
		{			
			break;
		}
	}
	AroudTempIndex = i;						//�������������¶�ֵ
}

#define LOW_BAT_VALU	2100		//1.65V��3.4V���±��׵�ѹ
#define LOW_BAT_VALU_UP	2300		//1.8V��3.6V���ϲŻָ�
#define FULL_BAT_VALU	2556		//4.12V	4.12V����

#define FULL_DOWN_VALU	2574		//С��4.15V�ж϶Ͽ�
//��ص�ѹֵ
void GetBatValu(void)  
{
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1a0)));  // 1a0-1b4
	static u8 TimesCnt = 0,TimesFullCnt = 0,TimesFullDnCnt = 0;
	_adcr0 = ADC_BAT;			//����
	STAR_ADC();
	while(_eocb);				// ת��������־
    
    k1 = _adrh;
    k1 <<= 8;
    k1 += _adrl;
    if(adcsp_1 < 10) 
    	i = adcsp_1;
    else  
    	i = adcsp_1 - 10;
    adcBuf[i] = k1;
    if(adcsp_1 < 11) 				//�ɼ�10������ 
    {
    	adcsp_1++; 
    	return;
    }
    adcsp_1 = 0;
    k1 = adcBuf[0];
    min = k1;
    max = k1;
    for(i=1;i<10;i++)
    {
        if(min > adcBuf[i])  
        	min = adcBuf[i];
        if(max < adcBuf[i])  
        	max = adcBuf[i];
        k1 += adcBuf[i]; 
    }
    k1 -= min; k1 -= max; k1 >>= 3;			//����10�Σ�ȥ�����ֵ����Сֵ��Ȼ��ƽ��
    if((k1 < LOW_BAT_VALU)&&(!ChargIngFlag)&&(WorkMode == RUN_MODE))	//��DC����,�ҵ׵�ѹʱ
    {
    	TimesCnt++;
    	if(TimesCnt >= 20)
    	{			
	    	if(!BATLow3p5Flag)
	    	{
				LowBatShutDnSCnt = 0;
	    	}
			BATLow3p5Flag = true;
    	}
    }
    else
    {
    	if((k1 > LOW_BAT_VALU_UP) && BATLow3p5Flag)
			BATLow3p5Flag = false;
		if(ChargIngFlag)    	
			BATLow3p5Flag = false;
		TimesCnt = 0;	
    }
    if(k1 < FULL_DOWN_VALU)						//�ж��û���������ѹ�϶�С��4.15V
    {
    	TimesFullDnCnt++;
    	if(TimesFullDnCnt >= 20)	
    	{
 	    	if(!ChargIngFlag)
	    	{
				ChargeFullFlag = false;
	    	}   		
    	}
    }
    if((k1 > FULL_BAT_VALU))					//����4.1V��ʼ��ʱ�������ڳ��ʱ,�������ӳ���ж�
    {
    	TimesFullCnt++;
    	if(TimesFullCnt >= 20)					//�г���������£��ſ�ʼ��λ
    	{			
	    	if(!BATUp4p1Flag)					//�״�����
	    	{
	    		BATChargeFullCnt = 0;
	    	}
	    	if(ChargIngFlag)					//ֻ���ڳ��ʱ���ó䱥λ
	    	{
				BATUp4p1Flag = true;
	    	}
    	}  	
    }
    else
    {
		BATUp4p1Flag = false;
		TimesFullCnt = 0;
    }
}


