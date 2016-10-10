#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>
#define Timer0High (65535-25000)/256
#define Timer0Low  (65535-25000)%256
#define Timer2High (65535-1500*4)/256
#define Timer2Low  (65535-1500*4)%256

//25ms定时器
void Timer0_Init(void)		//25毫秒
{
//	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
  CKCON &=0X00;       //设置定时器时钟选择
  CKCON |=0X02;       //设置定时器时钟system/48=1.02MHz
	TL0 = Timer0Low;		//设置定时初值
	TH0 = Timer0High;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
  ET0=1;
  EA=1;
}

void Timer0_ISR (void) interrupt 1
{
   TH0 = Timer0High;           // Init Timer0 High register
   TL0 = Timer0Low;           // Init Timer0 Low register

   Timer0Value++;		//25ms计数器自加
}

//1.5ms 16bit定时器	0xf408
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
	Timer2Value++;		//定时器2计数器++ 

	//主动通信检查 检查是否有需要发送的主动通信
	if ( (UART0.LockState[0] == 0) && (UART0.ButtonState[0] == 0) && (Return_Lock_ID == 0) )   //若抽屉的状态为关闭 && 没有按键被按下 && 没有要返回的电磁锁编号
	{
		TR2 = 0;								//定时器2禁止工作
		UART0.MasterBusy = 0;		//清除主动发送忙标识
	}
	else			 //若抽屉的状态为打开 || 有按键被按下 || 有要返回的电磁锁编号
	{
		if (UART0.Master_Tran_Timer	== Timer2Value)    //主动发送模式计时器 = 定时器2的定时次数
		{
			if (UART0.RevBuf[0])												 //如果
			{//有通信接受状态 继续等待
				UART0.Master_Tran_Timer = Timer2Value + 1;
			}
			else
			{//无通信接受 可以发送主动状态
				UART0.MasterFlag 		= 1;		//使能主动通信
				Timer2Value 			= 0;
				UART0.Master_Tran_Timer = 150;		//设定通信返回时间为75ms
			}
		}	
	}
}