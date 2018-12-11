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
unsigned int k;
unsigned char adcsp = 0;
unsigned int k2; 
unsigned char adcsp_2 = 0;
//22��45��ֵ
const u16 AllTempTab[26] = {1820,1866,1911,1957,2002,2048,2092,
						  	2137,2181,2225,2269,
						  	2312,2354,2396,2301,2478,
						  	2519,2559,2598,2636,2671,
						  	2711,2747,2783,2818,2853};
						  
u8 MaxTempIndex = 15;	//37�ȵ�����					  

//********************** READ NTC_DAT **************************************//
void GetPTCValu(void)  
{
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1c0)));  // 1C0

	_adcr0 = ADC_NTC;			//	NTC������  #define ADC_NTC   0x13	AN3
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
	if((k > Temp_22C) && (k < Temp_37C))		//  22C - 37C
	{
		if(SumOfEdge >= 120)	//120����������Χ��Ч
			TempSta = IN_22_37;
	}    
	else if((k > Temp_37C) && (k < Temp_45C))		// ���� 37C	 1536
	{
		SumOfEdge = 0;
		TempSta = IN_37_45;
	}
	else if(k < Temp_22C)		// С�� 22C   2182
	{
		TempSta = DOWN_22;
	}
	else if(k > Temp_45C)
	{
		SumOfEdge = 0;
		TempSta = UP_45;
	}
//	���Թ�ϵ����	
	if(k > AllTempTab[MaxTempIndex])	//�����������ֵ
	{
    	ntc_val = 0;
	}
    else if(k > Temp_20C)
    {
    	ntc_val = (AllTempTab[MaxTempIndex] - k)<<2;
    	if(ntc_val > 2048)
    		ntc_val = 2048;	
    }		 

}
#define MAX_INDOOR_TMEP		17 //�������37�ȣ�����17
//********************** READ BATTERY_DAT **************************************//
void Get_Battery_Valu(void)  
{
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1eb)));  // 1C0 -1d5

	_adcr0 = ADC_BATTERY;			//	����ص�������  #define ADC_BATTERY   0x13	AN5
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
    
///*******************************************************************************************/    
	for(i = 0;i < MAX_INDOOR_TMEP;i++)		//����Ӧֵ�������¾������¿���
	{
		if(k2 <= AllTempTab[i])
		{			
			break;
		}
	}
	MaxTempIndex = i + 8;					//�������������¶�ֵ����8��
}

