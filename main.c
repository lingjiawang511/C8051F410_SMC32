/**********************************************
名称：智能药柜(标准版——全部电磁锁)     适用电路板：SMC_V2.13 & SMC_V2.14 PCB  

历史修改版本号：V2.13	  修改日期：2015-12-08
修改原因：1、将电磁锁的控制部分全部设置为控制全部电磁锁，作为标准程序
					2、增加了电磁锁防抖1S，避免抽屉在没有稳定状态下反馈数据
					3、修改了打开抽屉不回复数据，关上抽屉再回复数据
					4、增加了限制发送数据次数

历史修改版本号：V2.14   修改日期：2015-12-11
修改原因：1、增加上位机发送开门指令,下位机在执行完动作后向上位机发送电磁锁状态指令

最新版本号：V2.15   修改日期：2016-9-22 ~2016-09-26
修改原因：1、核对程序，通讯数据有误
					2、程序运行一段时间后会出现关闭柜门不向上位机主动传送数据的情况
					3、加入显示版本号
/**********************************************/

#include <C8051F410.h>                // SFR declarations
#include <zyy_declare.h>
#include <zyy_define.h>

void PORT_Init (void)
{
   P0MDIN  = 0xFF;                    // 按钮读入驱动 工作指示灯驱动 UART0 RS485_COM 电磁阀信号反馈读入驱动
   P0MDOUT = 0xFF;                    //TXD,RXD=PUSH-PULL

   P1MDIN  = 0xff;                    // 电磁阀 & LED指示灯  驱动
   P1MDOUT = 0xff;                   

   XBR0    = 0x01;                    // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                    // IO口使能
}
void SYSCLK_Init (void)	//49M时钟作为系统时钟
{
    unsigned char i;
    PCA0MD &= ~0x40; // WDTE = 0 (clear watchdog timer
    OSCICN = 0x80;                         // enable intosc
    CLKSEL = 0x00;                         // select intosc as sysclk source
// INTOSC configure
    OSCICN = 0x83;
// PLL configure
    CLKMUL = 0x00;                         // Reset Clock Multiplier
    CLKMUL &= ~0x03;                       // select INTOSC / 2 as PLL source，49M
    CLKMUL |= 0x80;                        // Enable 4x Multipler (MULEN = 1)
    for (i = 0; i < 200; i++);             // Delay for at least 5us
    CLKMUL |= 0xC0;                        // Initialize Multiplier
   while (!(CLKMUL & 0x20));              // Poll for Multiply Ready
// SYSCLK configure
    VDM0CN = 0x80;                         // enable VDD monitor
    RSTSRC = 0x06;                         // enable missing clock detector
    // and VDD monitor reset sources
    CLKSEL = 0x02;                         // select PLL as clock source
    XBR1|=0X40;                            //enable SKIP
}
void main (void)
{
//	PCA0MD &= ~0x40;                    // 关闭看门狗
//	
//	//内部振荡器选择 24.5MHZ
//	OSCICN = 0x87;						//24.5MHZ不分频 使能内部振荡器
//	CLKSEL = 0;								//选择内部振荡器 SYSCLK不分频
//	while((OSCICN & 0x40) == 0);		//等待内部时钟工作状态建立
	SYSCLK_Init ();
	PORT_Init();                    // Initialize Port I/O
	//关闭所有LOCK
	disable_lock(0xff);

	Timer0_Init();					    //定时器0初始化
	Timer2_Init();							//定时器2初始化
	UART0_Init();								//UART初始化
	EA = 1;											//允许中断
	Worker_Count = Timer0Value + 20;	//工作指示灯闪烁时间初始化 500ms
	UART0.MasterBusy = 0;		//清除主动发送忙标识

	//关闭所有LED
	disable_led(0xFF);
	
	//开货架指示灯
	ShelfLed_Enable();	
	
	variable_init();					//初始化变量
	//使能485处于接受状态
    RS485_REN();						
//	UART0.TranDataRec = 0;

/*test cmd
LED ENABLE 
	@W10001087		  1号LED
	40 57 31 30 30 30 31 30 38 37 0d 09

	@W1001B087		  27号LED
	40 57 31 30 30 31 42 30 37 35 0d 09
LED DISABLE ALL
	@W100FF05C		  熄灭所有LED
	40 57 31 30 30 46 46 30 35 43 0d 09
*/
   while (1)
   {
		Communication_Respone();		//通信检查
		lock_control();							//检查是否有需要打开的电磁阀
		Check_lock_500ms();					//电磁阀打开500ms时间检测
		Led_Check();								//使能LED检查
		State_input();							//LOCK && BUTTON 状态检查

		if (Timer0Value == Worker_Count)	//工作指示灯闪烁
		{
			Worker_Count = Timer0Value + 20;
			WORK_LED = ~WORK_LED;
		}
   }                                   
}                                      



