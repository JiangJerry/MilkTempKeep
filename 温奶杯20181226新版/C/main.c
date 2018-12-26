//products mode:   X1
//functional description: 
//MCU mode:        HT66F018
//MCU frequency:   8MHz
//designer:        JerryJiang
//data:    2018/12/20
//record:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "base.h"
#define EXTERN
#define VAR_DEFINE
#include "Var.h"

extern  int ntc_val;
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
	_idlen 	= 0;				// IDLEN λΪ��ʱ��ϵͳ��������ģʽ
	_wdtc 	= 0b10101000;		// ���ܿ��Ź�
	_lvden 	= 0x00;		  		// ����LVD 
	GCC_HALT();	
}

void Sleep(void)
{
 	COM1_OFF;		 	
 	COM2_OFF;
 	COM3_OFF;			 	
	_adoff = true;
	Dealy();
	do
	{
		_emi = 0;		
		SleepMode();			//��������
		_emi = 1;
		break;		
	}while(1);
	_adoff = false;
}

void main(void)
{	
    InitSys();					//ϵͳ��ʼ��
    VarInit();					//������ʼ��
    TimerInit();				//��ʱ����ʼ��
    while(1)
    {
		GCC_CLRWDT();			//�忴�Ź�
		if(TaskADRdyFlag)		//4msһ��
		{
			TaskADRdyFlag = false;
			GetPTCValu();	     	//���Ŀ���¶�ֵ
			GetAroudTempValu();		//�������ֵ
		}
		TaskKey();				//��ȡ��ֵ
		TaskFunc();				//����
		TaskBatOp();			//��ѹ���
		TaskBuzzer();     		//��������5V��Դ������ƹ���
    }
}
