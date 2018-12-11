//products mode:   MW
//functional description: 
//MCU mode:        HT66F018
//MCU frequency:   8MHz
//designer:        Lan
//data:    2018/7/27 10:58
//record:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "base.h"
#define EXTERN
#define VAR_DEFINE
#include "Var.h"

extern unsigned int ntc_val;
void Dealy(void)
{//��ʱ����
	unsigned int i,j;
	for(i=0;i<50;i++)
	{
		for(j=0;j<50;j++)
		{
			 GCC_CLRWDT();
			_nop();
		}
	}	
}
//~~~~~~~~~~~~~~~~ ����ģʽ0 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void SleepMode(void)
{
	_idlen 	= 0;				//   IDLEN λΪ��ʱ��ϵͳ��������ģʽ
	_wdtc 	= 0b10101000;		// ���ܿ��Ź�
	_lvden 	= 0x00;		  		// ����LVD    //***  �͵�ѹ������λ
	GCC_HALT();	
}

void Sleep(void)
{
//	u8 i = 0;	
//	for(i=0;i<8;i++)		//�����ֹ����ʱ����������
//		LEDSta[i] = 0;
//	IOReset();
	_adoff = true;
	Dealy();
	do
	{
		_emi = 0;		
		SleepMode();		//��������
		_emi = 1;
		break;		
	}while(1);

	_adoff = false;
}

/********************************************************/
void main(void)
{	
    InitSys();					//ϵͳ��ʼ��
    Ram_Init();
    Timer_Init();				//��ʱ����ʼ��
    while(1)
    {
		GCC_CLRWDT();			//�忴�Ź�
		GetPTCValu();	     	//���NTC
		Get_Battery_Valu();		//����ص���      
		GetKey();				//��ȡ��ֵ
		Run_Operation();		//
		Battery_Status_Operation();
		TaskBuzzer();     
		if(DCInFlag)			//���ڳ��
		{			
			Breathe_Flag = 1;
		}
		else 
		{
			Breathe_Flag = 0;
		}
    }
}
