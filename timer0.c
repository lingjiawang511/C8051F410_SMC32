#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>
#define Timer0High (65535-25000)/256
#define Timer0Low  (65535-25000)%256
#define Timer2High (65535-1500*4)/256
#define Timer2Low  (65535-1500*4)%256

//25ms��ʱ��
void Timer0_Init(void)		//25����
{
//	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
  CKCON &=0X00;       //���ö�ʱ��ʱ��ѡ��
  CKCON |=0X02;       //���ö�ʱ��ʱ��system/48=1.02MHz
	TL0 = Timer0Low;		//���ö�ʱ��ֵ
	TH0 = Timer0High;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
  ET0=1;
  EA=1;
}

void Timer0_ISR (void) interrupt 1
{
   TH0 = Timer0High;           // Init Timer0 High register
   TL0 = Timer0Low;           // Init Timer0 Low register

   Timer0Value++;		//25ms�������Լ�
}

//1.5ms 16bit��ʱ��	0xf408
void Timer2_Init(void)  
{     
//	CKCON 	&= ~0x60;                     // Timer2 uses SYSCLK/12    
	
	TMR2CN 	&= ~0x01;    
	TMR2RLH	=	Timer2High;     
	TMR2RLL	=	Timer2Low;     

//	TR2 	= 1;     
	ET2 	= 1;                            // Timer2 interrupt enabled 
	EA=1;
}

void Timer2_ISR (void) interrupt 5 
{ 
	TF2H = 0;
	Timer2Value++;		//��ʱ��2������++ 

	//����ͨ�ż�� ����Ƿ�����Ҫ���͵�����ͨ��
	if ( (UART0.LockState[0] == 0) && (UART0.ButtonState[0] == 0) && (Return_Lock_ID == 0) )   //�������״̬Ϊ�ر� && û�а��������� && û��Ҫ���صĵ�������
	{
		TR2 = 0;								//��ʱ��2��ֹ����
		UART0.MasterBusy = 0;		//�����������æ��ʶ
	}
	else			 //�������״̬Ϊ�� || �а��������� || ��Ҫ���صĵ�������
	{
		if (UART0.Master_Tran_Timer	== Timer2Value)    //��������ģʽ��ʱ�� = ��ʱ��2�Ķ�ʱ����
		{
			if (UART0.RevBuf[0])												 //���
			{//��ͨ�Ž���״̬ �����ȴ�
				UART0.Master_Tran_Timer = Timer2Value + 1;
			}
			else
			{//��ͨ�Ž��� ���Է�������״̬
				UART0.MasterFlag 		= 1;		//ʹ������ͨ��
				Timer2Value 			= 0;
				UART0.Master_Tran_Timer = 150;		//�趨ͨ�ŷ���ʱ��Ϊ75ms
			}
		}	
	}
}