#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

void variable_init(void)
{
	uchar i;

	//初始化 LOCK && Button 状态寄存器
	UART0.LockState[0] = 0;
	UART0.LockState[1] = 0;
	UART0.LockState[2] = 0;
	UART0.LockState[3] = 0;
	UART0.LockState[4] = 0;
	UART0.ButtonState[0] = 0;
	UART0.ButtonState[1] = 0;
	UART0.ButtonState[2] = 0;
	UART0.ButtonState[3] = 0;
	UART0.ButtonState[4] = 0;

	//初始化 LED闪烁寄存器
	for(i=0;i<16;i++)
	{
		LED_BLINK[i] = 0;
	}

	//初始化 LOCK状态寄存器 为关闭状态
	LOCK_STATE[0] = 0;
	LOCK_STATE[1] = 0;
	LOCK_STATE[2] = 0;
	LOCK_STATE[3] = 0;
	LOCK_STATE[4] = 0;
	for(i=0;i<8;i++)
	{
		door_time_value[i] = 0;
	}
	//初始化 主动通信
	UART0.Master_Tran_Timer = 0;
	Timer2Value				= 0;
	UART0.MasterFlag 		= 0;

	//检测防抖建立标识
	Anti_Shake_Flag = 0;	


}
