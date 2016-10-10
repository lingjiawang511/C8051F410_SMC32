#ifndef ZYY_DEFINE_H
#define ZYY_DEFINE_H
//HEX转ASCII 0-F
unsigned char  code AsciiCode[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
//ASCII转HEX 0x30-0x46
unsigned char  code HexCode[71] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
								   0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
		 
//UART0 变量数据结构定义
UART volatile idata UART0;

//电磁阀状态记录寄存器	 初始状态默认柜门全部为关闭状态  
//编号顺序 数组0->4 高位->低位 1号->40号  eg: LOCK_STATE[0]-bit7->bit0=1#-8# , LOCK_STATE[1]-bit7->bit0=9#-16#.....
volatile uchar LOCK_STATE[5];

//LED闪烁设置寄存器
volatile uchar idata LED_BLINK[16];

//LOCK && BUTTON检测
volatile uchar Anti_Shake_Flag;	//检测防抖建立标识
volatile uchar Anti_Shake_Timer;	//检测防抖计数器

//发送次数检测
volatile uchar State_Trans_Cnt = 0;	//发送次数检测标识
									 
//当前电磁锁状态反馈次数
volatile uchar Return_Lock_State_Cnt = 0; 	 //检测当前电磁锁反馈次数
volatile uchar Return_Lock_State;		 		 //要反馈的电磁阀状态
volatile uchar Return_Lock_ID=0;		 		 //当前电磁阀的编号
volatile uchar Return_Lock_Flag=0;		 	 //检测当前电磁锁状态反馈标识
									 
volatile uchar LockAddr[5]; 			//电磁阀打开500ms编号数组  存储打开的电磁阀编号1-40
volatile uchar Lock_500ms[5];			//电磁阀打开500ms计时数组  存储打开的时间值（timer0value)
volatile uchar lock_1_8;					//电磁阀1-8的74HC595的当前值
volatile uchar lock_9_16;					//电磁阀9-16的74HC595的当前值
volatile uchar lock_17_24;				//电磁阀17-24的74HC595的当前值
volatile uchar lock_25_32;				//电磁阀25-32的74HC595的当前值
volatile uchar lock_33_40;				//电磁阀33-40的74HC595的当前值
volatile uchar door_time_value[8];	//电子门的3S打开时间定时值 电子门1号对应LOCK40 8号对应Lock_33

//volatile uchar LedAddr[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 		//LED打开编号数组  存储打开的LED编号1-128
volatile uchar led_1_8;					//电磁阀1-8的74HC595的当前值
volatile uchar led_9_16;				//电磁阀9-16的74HC595的当前值
volatile uchar led_17_24;				//电磁阀17-24的74HC595的当前值
volatile uchar led_25_32;				//电磁阀25-32的74HC595的当前值
volatile uchar led_33_40;				//电磁阀33-40的74HC595的当前值
volatile uchar led_41_48;				//电磁阀41-48的74HC595的当前值
volatile uchar led_49_56;				//电磁阀49-56的74HC595的当前值
volatile uchar led_57_64;				//电磁阀57-64的74HC595的当前值
volatile uchar led_65_72;				//电磁阀65-72的74HC595的当前值
volatile uchar led_73_80;				//电磁阀73-80的74HC595的当前值
volatile uchar led_81_88;				//电磁阀81-88的74HC595的当前值
volatile uchar led_89_96;				//电磁阀89-96的74HC595的当前值
volatile uchar led_97_104;				//电磁阀97-104的74HC595的当前值
volatile uchar led_105_112;				//电磁阀105-112的74HC595的当前值
volatile uchar led_113_120;				//电磁阀113-120的74HC595的当前值
volatile uchar led_121_128;				//电磁阀121-128的74HC595的当前值

volatile uchar Timer0Value;			   //TIMER0定时器里的计数变量 定时器0中断的次数  定时器0定时时间 = 25ms
volatile uchar Timer2Value;			   //TIMER2定时器里的计数变量 定时器2终端的次数  定时器2定时时间 = 1.5ms
volatile uchar Led_Blink_TimeValue;		//LED闪烁时间设置
volatile uchar Led_Blink_State;			//LED闪烁状态

volatile uchar Worker_Count;		   //工作指示灯计数器 500ms闪烁
#endif