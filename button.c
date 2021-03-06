#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

//LOCK && BUTTON 状态检查
void State_input(void)
{
	uchar i,j;

//按键状态读入
if (Anti_Shake_Flag == 0)
{//防抖之前的检测  检测到变化则使能防抖标识
	
	BUTTON_SL = 0;
	delay(5);
	BUTTON_SL = 1;
	delay(5);
//	Button_SL_RS();	 	//使能SL 获得按键状态
//	delay(5);
	
	if (~BUTTON)
	{//第一个读出的是第128号LED
		Anti_Shake_Flag = 1;		//建立防抖检测标识
		Anti_Shake_Timer = Timer0Value + 4;	//防抖时间 75ms-100ms
	}


	for(i=127;i>0;i--)
	{
		BUTTON_CLK = 0;
		delay(5);
		BUTTON_CLK = 1;
		delay(5);
//		Button_CLK_RS();	//读取按键输入

		if (~BUTTON)
		{//按键被按下则熄灭对应编号的LED
			Anti_Shake_Flag = 1;		//建立防抖检测标识
			Anti_Shake_Timer = Timer0Value + 4;	//防抖时间 75ms-100ms
		}
	}

//柜门状态读入
	FEEDBACK_SL = 0;
	delay(5);
	FEEDBACK_SL = 1;
	delay(5);
//	Feedback_SL_RS();	 	//使能SL 获得电磁阀状态
//	delay(5);
	
	if ( LOCK_FEEDBACK != (~LOCK_STATE[4] & 0x1) )
	{//第一个读出的是第40号电磁阀
			Anti_Shake_Flag = 1;		//建立防抖检测标识
			Anti_Shake_Timer = Timer0Value + 80;	//防抖时间 2s
	}

	for(i=39;i>0;i--)
	{
		FEEDBACK_CLK = 0;
		delay(5);
		FEEDBACK_CLK = 1;
		delay(5);
//		Feedban_CLK_RS();	//读取按键输入

		if (0 == (i%8))
		{
			if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8]) & 0x01) )
			{//改变了状态的电磁阀编号
					Anti_Shake_Flag = 1;		//建立防抖检测标识
					Anti_Shake_Timer = Timer0Value + 80;	//防抖时间 2s
			}
		}
		else
		{
			if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8] >> (8-i%8)) & 0x01) )
			{//改变了状态的电磁阀编号
					Anti_Shake_Flag = 1;		//建立防抖检测标识
					Anti_Shake_Timer = Timer0Value + 80;	//防抖时间 2s
			}
		}
	}
}
else
{//防抖检测  有变化后更新变化数组
	if (Anti_Shake_Timer == Timer0Value)
	{
		Anti_Shake_Flag = 0;		//清除防抖检测标识

		BUTTON_SL = 0;
		delay(5);
		BUTTON_SL = 1;
		delay(5);
//		Button_SL_RS();	 	//使能SL 获得按键状态
//		delay(5);
		
		if (~BUTTON)
		{//第一个读出的是第128号LED
			for(i=0;(i<i+UART0.ButtonState[i] && i<5);i++);	 //存储按键编号
			if (i < 5)
			{
				UART0.ButtonState[i] = 128;
			}
		}
	
	
		for(i=127;i>0;i--)
		{
			BUTTON_CLK = 0;
			delay(5);
			BUTTON_CLK = 1;
			delay(5);
//			Button_CLK_RS();	//读取按键输入
	
			if (~BUTTON)
			{//按键被按下则熄灭对应编号的LED
				for(j=0;(j<j+UART0.ButtonState[j] && j<5);j++);
				if (j < 5)
				{
					UART0.ButtonState[j] = i;
				}
				else
				{
					break;
				}
			}
		}
	
	//柜门状态读入
		FEEDBACK_SL = 0;
		delay(5);
		FEEDBACK_SL = 1;
		delay(5);
//		Feedback_SL_RS();	 	//使能SL 获得电磁阀状态
//		delay(5);
		
		if ( LOCK_FEEDBACK != (~LOCK_STATE[4] & 0x1) )
		{//第一个读出的是第40号电磁阀
	//		UART0.LOCK_State_Num = 40;
			for(i=0;(i<i+UART0.LockState[i] && i<5);i++);	 //存储 LOCK 编号和状态
			if (i < 5)
			{
				if(LOCK_FEEDBACK)
				{//LOCK关闭
					UART0.LockState[i] = 40;
					LOCK_STATE[4] &= ~0x01; //更新电磁阀状态
				}
				else
				{//LOCK打开
					UART0.LockState[i] = 0x80 | 40;
					LOCK_STATE[4] |= 0x01; //更新电磁阀状态
				}				
			}
	
		}
	
		for(i=39;i>0;i--)
		{
			FEEDBACK_CLK = 0;
			delay(5);
			FEEDBACK_CLK = 1;
			delay(5);
//			Feedban_CLK_RS();	//读取按键输入
	
			if (0 == (i%8))
			{
				if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8]) & 0x01) )
				{//改变了状态的电磁阀编号
	//					UART0.LOCK_State_Num = i;	
					for(j=0;(j<j+UART0.LockState[j] && j<5);j++);	 //存储 LOCK 编号和状态
					if (j < 5)
					{
						if(LOCK_FEEDBACK)
						{//LOCK关闭
							UART0.LockState[j] = i;
							LOCK_STATE[(i-1)/8] &= 0xfe; //更新电磁阀状�
						}
						else
						{//LOCK打开
							UART0.LockState[j] = 0x80 | i;
							LOCK_STATE[(i-1)/8] |= 1; //更新电磁阀状�
						}
					}
					else
					{
						break;
					}
	
				}
			}
			else
			{
				if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8] >> (8-i%8)) & 0x01) )
				{//改变了状态的电磁阀编号
	//					UART0.LOCK_State_Num = i;	
					for(j=0;(j<j+UART0.LockState[j] && j<5);j++);	 //存储 LOCK 编号和状态
					if (j < 5)
					{
						if(LOCK_FEEDBACK)
						{//LOCK关闭
							UART0.LockState[j] = i;
							LOCK_STATE[(i-1)/8] &= ~(1 << (8-i%8)); //更新电磁阀状态
						}
						else
						{//LOCK打开
							UART0.LockState[j] = 0x80 | i;
							LOCK_STATE[(i-1)/8] |= 1 << (8-i%8); //更新电磁阀状态
						}
					}
					else
					{
						break;
					}
	
				}
			}
		}


	}
}

}


