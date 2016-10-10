#ifndef ZYY_DECLARE_H
#define ZYY_DECLARE_H

#define uint unsigned int
#define uchar unsigned char

#define ENABLE 1
#define DISABLE 0

//ASCII 转换表声明
extern unsigned char code AsciiCode[];			//HEX转ASCII
extern unsigned char code HexCode[];				//ASCII转HEX


//RS485通信IO定义  通讯模块：RSM485ECHT
sbit RS485_COM = P0^6;					
#define  RS485_REN()	RS485_COM = 1					//接收：COM = 1
#define  RS485_TEN()	RS485_COM = 0					//发送：COM = 0

//UART0 缓冲器长度定义
#define UART0_Rxd_Length 10
#define UART0_Txd_Length 10

//UART0 通信协议内容定义
#define Slave_ID	HexCode[UART0.RevBuf[2]]	     //通信协议 从机地址 = 串口通讯接收的第3个字节
#define Lock_ID		( (HexCode[UART0.RevBuf[3]] << 4) | HexCode[UART0.RevBuf[4]] )	   //通信协议 电子锁编号 =  串口通讯接收的第4字节、第5字节转换成16进制
#define Lock_State	HexCode[UART0.RevBuf[5]]		 //电磁锁状态 发送指令第6个字节
#define Led_ID		( (HexCode[UART0.RevBuf[3]] << 4) | HexCode[UART0.RevBuf[4]] )	   //通信协议 LED编号 = 串口通讯接收的第4字节、第5字节转换成16进制
#define Led_State	HexCode[UART0.RevBuf[5]]	     //通信协议 LED设置状态 0=无闪烁 1=闪烁
#define LRC_H		UART0.RevBuf[UART0.RxdCnt-3]	   //通信协议 校验码高字节
#define LRC_L		UART0.RevBuf[UART0.RxdCnt-2]	   //通信协议 校验码低字节

//UART通信数据结构--使用的变量
typedef struct 
{
	uchar RevBuf[10];			//串口接受缓冲区
	uchar TraBuf[10];			//串口发送缓冲区
	uchar LockState[5];		//电磁阀状态数组 存储改变状态的电磁阀编号 bit7位是状态说明 1=打开 0=关闭 bit6-bit0=电磁阀编号
	uchar ButtonState[5];	//按键状态数组 存储被按下的按键编号
	uchar RxdCnt;					//串口接受字节个数计数器
	uchar TxdCnt;					//串口发送字节个数计数器
	uchar *TranData;			//串口数据发送指针
	uchar RevFlag;				//通讯响应 作为从机的通信处理标识
	uchar LOCK_ID;				//通信要求打开的电磁阀编号
	uchar LED_ID;					//通信要求点亮的LED编号
	uchar LED_STATE;			//通信设定LED的状态 0=无闪烁 1=闪烁
	uchar LED_Blink_Cnt;	//LED闪烁状态计时器
	uchar LED_Blink_state;	//LED当前闪烁状态 0=灭 1=亮
//	uchar LOCK_State_Num;	//主机模式 发送给 人机界面的改变了状态的柜门的编号
//	uchar LOCK_State;		//主机模式 发送给 人机界面的柜门的状态 1=关闭 0=打开
	uchar Master_Tran_Timer;//主动发送模式计时器  用于检查通信占用和重新发送定时
	uchar MasterFlag;		//主动发送标识  1=需要发送 0=无
	uchar MasterBusy;			//主动发送忙标识 防抖 0=无主动发送 1=主动发送中

}UART;
extern UART volatile idata UART0;

//LOCK && BUTTON检测
extern volatile uchar Anti_Shake_Flag;	//检测防抖建立标识
extern volatile uchar Anti_Shake_Timer;	//检测防抖计数器

//发送次数检测
extern volatile uchar State_Trans_Cnt;	//发送次数检测标识

//当前电磁锁状态反馈次数
extern volatile uchar Return_Lock_State_Cnt;  //检测当前电磁锁状态反馈次数
extern volatile uchar Return_Lock_State;			//检测当前电磁锁状态
extern volatile uchar Return_Lock_ID;		 	
extern volatile uchar Return_Lock_Flag;		 		//检测当前电磁锁状态反馈标识

//工作指示灯IO口定义
sbit WORK_LED   = P0^2;		 //工作指示灯
#define WorkLed_Enable()	WORK_LED = 0;
#define WorkLed_Disable()	WORK_LED = 1;

//货架工作指示灯定义
sbit  SHELF_LED = P1^3;			//货架工作指示灯
#define ShelfLed_Enable()		SHELF_LED = 0;
#define ShelfLed_Disable()	SHELF_LED = 1;

//按键输入定义
sbit BUTTON = P2^1;
sbit BUTTON_SL = P0^0;	    
sbit BUTTON_CLK = P0^1;
#define Button_SL_RS()		BUTTON_SL = 0; BUTTON_SL = 1;
#define Button_CLK_RS()		BUTTON_CLK = 0; BUTTON_CLK = 1;
//电磁阀反馈信号定义
sbit LOCK_FEEDBACK = P2^0;
sbit FEEDBACK_SL = P0^3;
sbit FEEDBACK_CLK = P0^7;
#define Feedback_SL_RS()	FEEDBACK_SL = 0; FEEDBACK_SL = 1;
#define Feedban_CLK_RS()	FEEDBACK_CLK = 0; FEEDBACK_CLK = 1;

//电磁阀驱动定义
sbit LOCK_SCLK = P1^0;
sbit LOCK_LCLK = P1^1;
sbit LOCK_DATA = P1^2;
#define Lock_Sclk_RS()  LOCK_SCLK = 0; LOCK_SCLK = 1
#define Lock_Lclk_RS()  LOCK_LCLK = 0; LOCK_LCLK = 1

//LED驱动定义
sbit LED_SCLK = P1^4;
sbit LED_LCLK = P1^5;
sbit LED_DATA = P1^6;
#define Led_Sclk_RS()  LED_SCLK = 0; LED_SCLK = 1
#define Led_Lclk_RS()  LED_LCLK = 0; LED_LCLK = 1
//LED闪烁设置寄存器
extern volatile uchar idata LED_BLINK[16];

//电磁阀状态记录寄存器	 初始状态默认电磁阀全部为关闭状态
extern volatile uchar LOCK_STATE[];

//电子锁&LED灯 变量声明
extern volatile uchar LockAddr[];		
extern volatile uchar Lock_500ms[];
extern volatile uchar door_time_value[];	//电子门的3S打开时间定时值 电子门1号对应LOCK40 8号对应Lock_33
//extern volatile uchar LedAddr[];
extern volatile uchar Timer0Value;
extern volatile uchar Timer2Value;			   //TIMER2定时器里的计数变量 用于通信检查 1.5ms定时
extern volatile uchar lock_1_8;
extern volatile uchar lock_9_16;
extern volatile uchar lock_17_24;
extern volatile uchar lock_25_32;
extern volatile uchar lock_33_40;
extern volatile uchar led_1_8;
extern volatile uchar led_9_16;
extern volatile uchar led_17_24;
extern volatile uchar led_25_32;
extern volatile uchar led_33_40;
extern volatile uchar led_41_48;
extern volatile uchar led_49_56;
extern volatile uchar led_57_64;
extern volatile uchar led_65_72;
extern volatile uchar led_73_80;
extern volatile uchar led_81_88;
extern volatile uchar led_89_96;
extern volatile uchar led_97_104;
extern volatile uchar led_105_112;
extern volatile uchar led_113_120;
extern volatile uchar led_121_128;
extern volatile uchar Worker_Count;


//LED配置变量
extern volatile uchar Led_Blink_TimeValue;		//LED闪烁时间设置
extern volatile uchar Led_Blink_State;			//LED闪烁时间设置


// 外部程序调用
extern void lock_control(void);		//电磁阀打开检查
//关闭电磁阀
extern void disable_lock(uchar lock);
//变量初始化程序
extern void variable_init(void);

extern void Timer0_Init(void);

extern void Timer2_Init(void);//1.5ms 16bit定时器	0xf408  

extern void Check_lock_500ms(void);//检查电磁阀打开时间  500ms

extern void UART0_Init(void);//UART 初始化
//点亮LED
extern void led_enable(uchar led);

//熄灭LED
extern void disable_led(uchar led);

//LOCK && BUTTON 状态检查
extern void State_input(void);

//柜门状态读入 & 主机模式发送状态
extern void Lock_Input(void);

//检查是否有需要点亮的LED灯
extern void Led_Check(void);

//LRC校验
extern unsigned int LRC16(unsigned char *Pushdata,unsigned char length);

//通信处理程序 处理--410作为从机通信和作为主机通信
extern void Communication_Respone(void);

//enable 电磁阀
extern void lock_enable(uchar lock);

extern void delay(uchar time);

#endif	

/*-----------ASCII码 通信协议---------------
主机写命令  LED_State = 00 灭，01 亮，02 闪烁			 
0x40(start) + 0x57('W') + SLAVE_ID(默认01)  + LOCK(H+L)       + LED(H+L)     + LED_State + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
首字节      + 写('W')   + 从机地址          + 电磁锁编号(H+L) + LED编号(H+L) + LED_State + LRC(H+L) + 结束字1     + 结束字2
从机回复:
LRC校验正确则回复 
0x40(start) + 0x57('W') + SLAVE_ID(默认01)  + LOCK(H+L)       + LED(H+L) + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
合计 = 8个字节
LRC校验错误 或 其它错误	
0x40(start) + 0x4F('E') + 0x4B('R') + LRC + 0x0d(END-1) + 0x09(END-2)

主机读命令 			 
0x40(start) + 0x44('R') + SLAVE_ID(默认01)  + LOCK(H+L)       + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
首字节      + 读('R')   + 从机地址          + 电磁锁编号(H+L) + LRC(H+L) + 结束字1     + 结束字2
从机回复:
LRC校验正确则回复 
0x40(start) + 0x44('R') + SLAVE_ID(默认01)  + LOCK(H+L)       + LED(H+L) + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
合计 = 8个字节
LRC校验错误 或 其它错误	
0x40(start) + 0x4F('E') + 0x4B('R') + LRC + 0x0d(END-1) + 0x09(END-2)
**************************/