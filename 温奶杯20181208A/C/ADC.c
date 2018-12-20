#include "base.h"
#define EXTERN extern
#include "Var.h"

#define MIN_VALU		500		//最小值保护，如果短路可能导致放大器输出底,小于0.4V保护，说明358坏了

#define Temp_20C		1820//2100		//   22c   11.4239  1912
#define Temp_22C		1911//2100		//   22c   11.4239  1912
#define Temp_37C		2396//2200		//   37c   6.0053   2560
#define Temp_45C		2478//2400		//   45c   4.3558   2853

#define Bat_4_2V		1789		//4.2v 数值  
#define Bat_3_3V		1410		//3.3V 数值

extern unsigned int ntc_val;
int k;
unsigned char adcsp = 0;
unsigned int k1,k2; 
unsigned char adcsp_2 = 0;
unsigned char adcsp_1 = 0;

#define MAX_TEMP_ARRAY	100
//环境温度值
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
//45度时的物体温度						
//0-9
//10-19
//20-29
//30-39
//40-49 MAX_TEMP_ARRAY
#define POSITIVE_TEMP_ARRAY		81		//0-45度
const int ObjPositiveTemp[POSITIVE_TEMP_ARRAY] = {1117,1097,1077,1057,1037,1017,996,975,954,933,
						 			 			911,889,867,845,823,800,777,754,730,707,
						 			 			683,659,634,609,585,559,534,508,482,456,
						 			 			430,403,376,349,321,293,265,237,208,179,
						 			 			150,121,91,61,31,0,-31,-62,-94,-125,
						 			 			-157,-189,-210,-240,-270,-317,-347,-380,-410,-460,
						 			 			-489,-510.-540,-570.-600,-669,-700,-710,-740,-770,
						 			 			-857,-880,-910,-940,-970,-1054,-1080,-1120,-1150,
						 			 			-1259};	//目标正温度
#define NAGATIVE_TEMP_ARRAY		46		//0-45度
						 			 
//const u16 ObjPositiveTemp[POSITIVE_TEMP_ARRAY] = {1117,1097,1077,1057,1037,1017,996,975,954,933,
//						 			 911,889,867,845,823,800,777,754,730,707,
//						 			 683,659,634,609,585,559,534,508,482,456,
//						 			 430,403,376,349,321,293,265,237,208,179,
//						 			 150,121,91,61,31,0};	//目标正温度
						 
#define FOUR_DEGREES		150		//4度差值					  							  
#define BIAS_VALU			2047	//偏置电压
#define TEMP_ADD_H			600		//补正高温值：55度，高点位
#define TEMP_ADD_L			1000	//补正高温低点	
#define TEMP_DEC			50		//低温：40度
u8 AroudTempIndex = 0;				//室温索引值					  

//红外目标温度值
void GetPTCValu(void)  
{
	unsigned char i;
	unsigned int min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1c0)));  // 1C0-1d5

	_adcr0 = ADC_NTC;			//	NTC传感器
	STAR_ADC();
	while(_eocb);				// 转换结束标志
    
    k = _adrh;
    k <<= 8;
    k += _adrl;
    if(adcsp < 10) 
    	i = adcsp;
    else  
    	i = adcsp - 10;
    adcBuf[i] = k;
    if(adcsp < 11) 				//采集10次数据 
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
    k -= min; k -= max; k >>= 3;			//采样10次，去掉最大值和最小值，然后平均    
/*******************************************************************************************/
//	线性关系计算
    if(k > BIAS_VALU)
	{
		k = k - BIAS_VALU;
	}
	else
	{
		k = 0;
	}
	if(TempDir)											//下降？上升?实现红绿滞回
	{
		max = ObjPositiveTemp[AroudTempIndex] + TEMP_ADD_H;	//补正以后的值
		if(k >= max)									//目标温度索引值，大于40度，-150降5度
		{
	    	TempSta = UP_45;
	    	TempDir = true;								//下降过程
		}
		else
		{
	    	TempDir = false;							//转为上升
		}
    	TempDirDnFlag = true;
	}
    else
    {
    	max = ObjPositiveTemp[AroudTempIndex] + TEMP_ADD_L;
    	if(k >= max)	//目标大于45度转为上升，以奶水测试标准
    	{
	    	TempDir = true;								//红灯上升
    	}
    	else
    	{
    		if(TempDirDnFlag)							//实现绿蓝滞回
    		{
	    		if(k >= ObjPositiveTemp[AroudTempIndex])		//绿灯度数
	    		{
			    	TempSta = IN_22_37;
			    	TempDirDnFlag = true;				//转绿灯温度
			    	ntc_val = (k - ObjPositiveTemp[AroudTempIndex]) << 4;	//比例增加值
			    	if(ntc_val > 2048)
			    		ntc_val = 2048;	
	    		}
	    		else
	    		{
			    	TempSta = DOWN_22;
			    	TempDirDnFlag = false;				//转蓝灯温度	
			    	ntc_val = 2048;    			
	    		}
		    	TempDir = false;						//红灯下降过程			
    		}
    		else
    		{
    			max = ObjPositiveTemp[AroudTempIndex] + TEMP_DEC;
	    		if(k >= max)			//绿灯温度
	    		{
			    	TempSta = IN_22_37;
			    	TempDirDnFlag = true;				//转绿灯温度
	    		}
	    		else
	    		{
			    	TempSta = DOWN_22;					//转蓝灯温度
			    	TempDirDnFlag = false;
	    		}    			
		    	TempDir = false;						//红灯下降过程			
		    	ntc_val = 2048;    			
    		}    		    		    		
    	}
    }        		
}
//室温值
void GetAroudTempValu(void)  
{
	unsigned char i;
	unsigned int min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1eb)));  // 1eb -1d5

	_adcr0 = ADC_AROUD;			//室温
	STAR_ADC();
	while(_eocb);				// 转换结束标志
    
    k2 = _adrh;
    k2 <<= 8;
    k2 += _adrl;
    if(adcsp_2 < 10) 
    	i = adcsp_2;
    else  
    	i = adcsp_2 - 10;
    adcBuf[i] = k2;
    if(adcsp_2 < 11) 				//采集10次数据 
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
    k2 -= min; k2 -= max; k2 >>= 3;			//采样10次，去掉最大值和最小值，然后平均        
	for(i=0;i <MAX_TEMP_ARRAY;i++)			//最大对应值，用室温决定壁温控制
	{
		if(k2 >= AroundTemp[i])
		{			
			break;
		}
	}
	AroudTempIndex = i;						//室温索引是室温度值
}

#define LOW_BAT_VALU	2100		//1.65V，3.4V以下报底电压
#define LOW_BAT_VALU_UP	2300		//1.8V，3.6V以上才恢复
#define FULL_BAT_VALU	2556		//4.12V	4.12V以上

#define FULL_DOWN_VALU	2574		//小于4.15V判断断开
//电池电压值
void GetBatValu(void)  
{
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1a0)));  // 1a0-1b4
	static u8 TimesCnt = 0,TimesFullCnt = 0,TimesFullDnCnt = 0;
	_adcr0 = ADC_BAT;			//室温
	STAR_ADC();
	while(_eocb);				// 转换结束标志
    
    k1 = _adrh;
    k1 <<= 8;
    k1 += _adrl;
    if(adcsp_1 < 10) 
    	i = adcsp_1;
    else  
    	i = adcsp_1 - 10;
    adcBuf[i] = k1;
    if(adcsp_1 < 11) 				//采集10次数据 
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
    k1 -= min; k1 -= max; k1 >>= 3;			//采样10次，去掉最大值和最小值，然后平均
    if((k1 < LOW_BAT_VALU)&&(!ChargIngFlag)&&(WorkMode == RUN_MODE))	//无DC插入,且底电压时
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
    if(k1 < FULL_DOWN_VALU)						//判断用户拨掉，电压肯定小于4.15V
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
    if((k1 > FULL_BAT_VALU))					//大于4.1V开始计时，且有在充电时,不能增加充电判断
    {
    	TimesFullCnt++;
    	if(TimesFullCnt >= 20)					//有充过电的情况下，才开始置位
    	{			
	    	if(!BATUp4p1Flag)					//首次清零
	    	{
	    		BATChargeFullCnt = 0;
	    	}
	    	if(ChargIngFlag)					//只有在充电时才置充饱位
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


