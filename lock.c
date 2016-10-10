#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

//enable ��ŷ�
void lock_enable(uchar lock)
{   
	uchar i;

	switch((lock-1)/8)
	{
		case 0://������λ�ñ�� 1-8 
				lock_1_8 |= 0x80 >> ( (lock-1)%8 );
				break;
		case 1:
				lock_9_16 |= 0x80 >> ( (lock-1)%8 );
				break;
		case 2:
				lock_17_24 |= 0x80 >> ( (lock-1)%8 );
				break;
		case 3:
				lock_25_32 |= 0x80 >> ( (lock-1)%8 );
				break;
		case 4:
				lock_33_40 |= 0x80 >> ( (lock-1)%8 );
				break;

		default:
			  	lock_1_8  = 0;	  //�ر�����
			  	lock_9_16 = 0;
			  	lock_17_24 = 0;									
			  	lock_25_32 = 0;
				lock_33_40 = 0;
				break;
	}


	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_33_40 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_25_32 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_17_24 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_9_16 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_1_8 >> i) &0x1;
		Lock_Sclk_RS();
	}
	Lock_Lclk_RS();
}

//�رյ�ŷ�
void disable_lock(uchar lock)
{
	uchar i;

	switch((lock-1)/8)
	{
		case 0://������λ�ñ�� 1-8 
				lock_1_8 &= ~(0x80 >> ( (lock-1)%8 ));
				break;
		case 1:
				lock_9_16 &= ~(0x80 >> ( (lock-1)%8 ));
				break;
		case 2:
				lock_17_24 &= ~(0x80 >> ( (lock-1)%8 ));
				break;
		case 3:
				lock_25_32 &= ~(0x80 >> ( (lock-1)%8 ));
				break;
		case 4:
				lock_33_40 &= ~(0x80 >> ( (lock-1)%8 ));
				break;

		default:
			  	lock_1_8  = 0;	  //�ر�����
			  	lock_9_16 = 0;
			  	lock_17_24 = 0;									
			  	lock_25_32 = 0;
				  lock_33_40 = 0;
				break;
	}


	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_33_40 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_25_32 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_17_24 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_9_16 >> i) &0x1;
		Lock_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LOCK_DATA = (lock_1_8 >> i) &0x1;
		Lock_Sclk_RS();
	}
	Lock_Lclk_RS();
}

//����Ƿ��д򿪵ĵ�ŷ�������¼��ŷ���ź�500ms��ʱʱ��
void lock_control(void)
{
	uchar i;					 
	
	if( (UART0.LOCK_ID >= 1) && (UART0.LOCK_ID <= 40) )
	{//�洢�򿪵ĵ�ŷ���ź�ʱ��
			for(i=0;(i<i+LockAddr[i] && i<5);i++);       //LockAddr[i]��LockAddr[0]��LockAddr[1]��LockAddr[2]��LockAddr[3]��LockAddr[4]��ŷ���500ms�������  �洢�򿪵ĵ�ŷ����1-40

			if (i <= 4)
			{
				LockAddr[i] 		= UART0.LOCK_ID;					//�洢��ŷ����
				Lock_500ms[i] 	= Timer0Value + 20;				//500ms��ʱ

				lock_enable(UART0.LOCK_ID);								//enable solid
				UART0.LOCK_ID = 0;
			}
			else if (UART0.LOCK_ID == 0xff)
			{//�ر�����LOCK
				
				disable_lock(0xff);
				UART0.LOCK_ID = 0;
			}
			else
			{//5���Ѿ��򿪵ĵ�ŷ�û�йرյ�����£����ٿ�������ĵ�ŷ�
				;
			}
	}
	else
	{

	}


}

//����ŷ���ʱ��  500ms
void Check_lock_500ms(void)
{
	uchar i;

	for(i=0;i<5;i++)
	{
		if (LockAddr[i] == 0)
		{

		}
		else
		{//���500msʱ���Ƿ��Ѿ�����
			if(Lock_500ms[i] == Timer0Value)
			{
				disable_lock(LockAddr[i]);	//�رյ�ŷ�
				LockAddr[i] = 0;			//�����ŷ���¼
			}
		}

	}
}



