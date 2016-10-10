#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

//enable 电磁阀
void lock_enable(uchar lock)
{   
	uchar i;

	switch((lock-1)/8)
	{
		case 0://打开锁的位置编号 1-8 
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
			  	lock_1_8  = 0;	  //关闭所有
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

//关闭电磁阀
void disable_lock(uchar lock)
{
	uchar i;

	switch((lock-1)/8)
	{
		case 0://打开锁的位置编号 1-8 
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
			  	lock_1_8  = 0;	  //关闭所有
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

//检查是否有打开的电磁阀，并记录电磁阀编号和500ms定时时间
void lock_control(void)
{
	uchar i;					 
	
	if( (UART0.LOCK_ID >= 1) && (UART0.LOCK_ID <= 40) )
	{//存储打开的电磁阀编号和时间
			for(i=0;(i<i+LockAddr[i] && i<5);i++);       //LockAddr[i]：LockAddr[0]、LockAddr[1]、LockAddr[2]、LockAddr[3]、LockAddr[4]电磁阀打开500ms编号数组  存储打开的电磁阀编号1-40

			if (i <= 4)
			{
				LockAddr[i] 		= UART0.LOCK_ID;					//存储电磁阀编号
				Lock_500ms[i] 	= Timer0Value + 20;				//500ms定时

				lock_enable(UART0.LOCK_ID);								//enable solid
				UART0.LOCK_ID = 0;
			}
			else if (UART0.LOCK_ID == 0xff)
			{//关闭所有LOCK
				
				disable_lock(0xff);
				UART0.LOCK_ID = 0;
			}
			else
			{//5个已经打开的电磁阀没有关闭的情况下，不再开启另外的电磁阀
				;
			}
	}
	else
	{

	}


}

//检查电磁阀打开时间  500ms
void Check_lock_500ms(void)
{
	uchar i;

	for(i=0;i<5;i++)
	{
		if (LockAddr[i] == 0)
		{

		}
		else
		{//检查500ms时间是否已经到了
			if(Lock_500ms[i] == Timer0Value)
			{
				disable_lock(LockAddr[i]);	//关闭电磁阀
				LockAddr[i] = 0;			//清除电磁阀记录
			}
		}

	}
}



