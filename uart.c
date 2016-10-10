#include <C8051F410.h>                 // SFR declarations
#include <intrins.h>                 // SFR declarations
#include <zyy_declare.h>

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE        9600           // Baud rate of UART in bps

//RS485 方向控制端延迟用
void delay(uchar time)
{
	while(time--);
}

//LRC校验 注意 参数length=通信接收的个数-
unsigned int LRC16(unsigned char *Pushdata,unsigned char length)
{
	unsigned char	LRCHi=0;
	unsigned char 	LRCLo;

	while(length--) 
	{ 
		LRCHi = LRCHi + *Pushdata++;
	}

	LRCHi = ~LRCHi + 1;
	LRCLo = AsciiCode[LRCHi & 0x0F];
	LRCHi	= LRCHi & 0xf0;
	LRCHi = (LRCHi >> 4);
	LRCHi	= AsciiCode[LRCHi];

	return ((LRCHi<<8) | LRCLo); 

}
//通信使能LOCK
void enable_lock(void)
{
	uint tmp;							//计算通信的LRC校验码
	uchar *ptr;						//数据接收指针

  ptr = &UART0.RevBuf[1];
	tmp = LRC16(ptr,UART0.RxdCnt-4);		//计算通信的LRC校验码
	if ( tmp == ((LRC_H << 8) | LRC_L) )
	{//LRC校验正确
		UART0.LOCK_ID 	= Lock_ID;				//通信要求打开的电磁阀编号 = 获得需要使能的电磁阀编号	Lock_ID:( (HexCode[UART0.RevBuf[3]] << 4) | HexCode[UART0.RevBuf[4]] )	   //通信协议 电子锁编号
	    
		//答复主机	与发送内容相同
		for(tmp=0;tmp<9;tmp++)
		{
			UART0.TraBuf[tmp] = UART0.RevBuf[tmp];
		}
		RS485_TEN();	   										//切换485芯片模式为 发送
		UART0.TxdCnt		= UART0.RxdCnt;		//设置发送长度
		UART0.TranData 	= &UART0.TraBuf[1];	//初始化数据发送指针
		SBUF0 					= UART0.TraBuf[0];	//发送第一个数据
	}
	else
	{//LRC校验错误
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.RevBuf[tmp] = 0;	//情况通信接收缓冲区
		}
		RS485_REN();		// 切换通讯为接受模式
	}
}

//通信使能Led
void enable_led(void)
{
	uint tmp;
	uchar *ptr;

  ptr = &UART0.RevBuf[1];
	tmp = LRC16(ptr,UART0.RxdCnt-4);		//计算通信的LRC校验码
	if ( tmp == ((LRC_H << 8) | LRC_L) )
	{//LRC校验正确
		UART0.LED_ID 	= Led_ID;			//获得需要使能的LED编号	
		UART0.LED_STATE	= Led_State;		//LED闪烁状态	

		if (UART0.LED_STATE == 1)
		{//LED闪烁
			LED_BLINK[(UART0.LED_ID-1)/8] |= 0x80 >> ((UART0.LED_ID-1)%8); 	//使能需要闪烁的LED的bit位
			UART0.LED_Blink_Cnt 	= Timer0Value + 20;	//500ms
			UART0.LED_Blink_state 	= 0;				//初始化闪烁状态为灭
		}
		else if (UART0.LED_STATE == 0)
		{
			LED_BLINK[(UART0.LED_ID-1)/8] &= ~(0x80 >> ((UART0.LED_ID-1)%8)); //失效不需闪烁的LED的bit位
		}
		else
		{//熄灭LED	led_state = 0xff
			disable_led(UART0.LED_ID);	 	//熄灭LED
			LED_BLINK[(UART0.LED_ID-1)/8] &= ~(0x80 >> ((UART0.LED_ID-1)%8)); //失效不需闪烁的LED的bit位
			UART0.LED_ID = 0;
		}

//		if(UART0.LED_STATE)
//		{//LED需要闪烁  初始化LED闪烁计时器
//			UART0.LED_Blink_Cnt 	= Timer0Value + 20;	//500ms
//			UART0.LED_Blink_state 	= 0;				//初始化闪烁状态为灭
//		}
		
		//答复主机	与发送内容相同
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.TraBuf[tmp] = UART0.RevBuf[tmp];
		}
		RS485_TEN();	   						//切换485芯片模式为 发送
		UART0.TxdCnt	= UART0.RxdCnt;		//设置发送长度
		UART0.TranData 	= &UART0.TraBuf[1];	//初始化数据发送指针
		SBUF0 			= UART0.TraBuf[0];	//发送第一个数据
	}
	else
	{//LRC校验错误
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.RevBuf[tmp] = 0;	//情况通信缓冲区
		}
		RS485_REN();		// 切换通讯为接受模式
	}
}

/******************************V2.15增加部分(以下)*********************************/
//显示版本号
void show_version(void)
{
	uint tmp;							//计算通信的LRC校验码变量
	uchar *ptr;						//数据接收指针

  ptr = &UART0.RevBuf[1];
	tmp = LRC16(ptr,UART0.RxdCnt-4);		//计算通信的LRC校验码
	if ( tmp == ((LRC_H << 8) | LRC_L) )
	{//LRC校验正确	    
		//答复主机	与发送内容相同
		RS485_TEN();	   										//切换485芯片模式为 发送
		UART0.TxdCnt		= UART0.RxdCnt;		//设置发送长度
		UART0.TranData 	= &UART0.TraBuf[1];	//初始化数据发送指针
		SBUF0 					= UART0.TraBuf[0];	//发送第一个数据

		UART0.TraBuf[0]	= 0x40;	   	//首字节
		UART0.TraBuf[1]	= 0x56;		//命令字'V'
		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

		UART0.TraBuf[3]	= 0x31;	   	//版本号-H   版本号：15 (可改变)
		UART0.TraBuf[4]	= 0x35;	   	//版本号-L

		ptr = &UART0.TraBuf[1];
		tmp	= LRC16(ptr,4);			//计算通信的LRC校验码

		UART0.TraBuf[5] = ((tmp & 0xff00)>>8); 	//LRC高字节
		UART0.TraBuf[6] =	tmp & 0x00ff; 		//LRC低字节

		UART0.TraBuf[7] = 0x0d;
		UART0.TraBuf[8] = 0x09;

		UART0.MasterFlag = 0;
		RS485_TEN();	   					//切换485芯片模式为 发送
		UART0.TxdCnt = 8;					//设置发送长度
		UART0.TranData = &UART0.TraBuf[1];	//初始化数据发送指针
		SBUF0 = UART0.TraBuf[0];			//发送第一个数据
	}
	
	else
	{//LRC校验错误
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.RevBuf[tmp] = 0;	//情况通信接收缓冲区
		}
		RS485_REN();		// 切换通讯为接受模式
	}	
}
/*******************************V2.15增加部分(以上)**********************************/

//检验lock状态从机的返回信息
uchar lock_return(void)
{
	uchar i;

	for(i=0;i<10;i++)
	{
		if (UART0.RevBuf[i] == UART0.TraBuf[i])
		{
			;
		}
		else
		{
			i = 0;
			break;
		}
	}

	return i;
}

//检验button状态从机的返回信息
uchar button_return(void)
{
	uchar i;

	for(i=0;i<9;i++)
	{
		if (UART0.RevBuf[i] == UART0.TraBuf[i])
		{
			;
		}
		else
		{
			i = 0;
			break;
		}
	}

	return i;
}

//主动发送LOCK状态和按键状态
void master_transmit(void)
{
	uchar *ptr;
	uint tmp;
	uchar i;

	if ( (UART0.LockState[0] != 0) && (UART0.MasterFlag == 1) &&(State_Trans_Cnt != 3) )		//增加检测发送次数判断
	{//LOCK状态发送
		State_Trans_Cnt++;    //增加检测发送次数

		UART0.TraBuf[0]	= 0x40;	   	//首字节
		UART0.TraBuf[1]	= 0x53;		//命令字'S'
		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

		UART0.TraBuf[3]	= AsciiCode[ (UART0.LockState[0] & 0x70) >> 4 ];	   	//LOCK编号-H
		UART0.TraBuf[4]	= AsciiCode[ (UART0.LockState[0] & 0x0f)      ];	   	//LOCK编号-L
		if(UART0.LockState[0] & 0x80)
		{//柜门打开
			UART0.TraBuf[5]	= 0x31;	   	//LOCK状态  1=打开 0=关闭
		}
		else
		{//柜门关闭
			UART0.TraBuf[5]	= 0x30;	   	//LOCK状态  1=打开 0=关闭
		}

		ptr = &UART0.TraBuf[1];
		tmp	= LRC16(ptr,5);			//计算通信的LRC校验码

		UART0.TraBuf[6] = ((tmp & 0xff00)>>8); 	//LRC高字节
		UART0.TraBuf[7] =	tmp & 0x00ff; 				//LRC低字节

		UART0.TraBuf[8] = 0x0d;
		UART0.TraBuf[9] = 0x09;

		UART0.MasterFlag = 0;
		RS485_TEN();	   					//切换485芯片模式为 发送
		UART0.TxdCnt = 9;					//设置发送长度
		UART0.TranData = &UART0.TraBuf[1];	//初始化数据发送指针
		SBUF0 = UART0.TraBuf[0];			//发送第一个数据

	}
	else if ( (UART0.ButtonState[0] != 0) && (UART0.MasterFlag == 1) &&(State_Trans_Cnt != 3) )  //增加检测发送次数判断
	{//BUTTON状态发送
		 State_Trans_Cnt++;    //增加检测发送次数
		
		UART0.TraBuf[0]	= 0x40;	   	//首字节
		UART0.TraBuf[1]	= 0x54;		//命令字'S'
		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

		UART0.TraBuf[3]	= AsciiCode[ (UART0.ButtonState[0] & 0xf0) >> 4 ];	   	//BUTTON编号-H
		UART0.TraBuf[4]	= AsciiCode[ (UART0.ButtonState[0] & 0x0f)      ];	   	//BUTTON编号-L

		ptr = &UART0.TraBuf[1];
		tmp	= LRC16(ptr,4);			//计算通信的LRC校验码

		UART0.TraBuf[5] = ((tmp & 0xff00)>>8); 	//LRC高字节
		UART0.TraBuf[6] =	tmp & 0x00ff; 		//LRC低字节

		UART0.TraBuf[7] = 0x0d;
		UART0.TraBuf[8] = 0x09;

		UART0.MasterFlag = 0;
		RS485_TEN();	   					//切换485芯片模式为 发送
		UART0.TxdCnt = 8;					//设置发送长度
		UART0.TranData = &UART0.TraBuf[1];	//初始化数据发送指针
		SBUF0 = UART0.TraBuf[0];			//发送第一个数据
	}
	
//**************增加上位机发送开门指令,下位机在执行完动作后向上位机发送电磁锁状态指令************/
//	else if ( (Return_Lock_ID) && (Return_Lock_State_Cnt != 3) && (UART0.MasterFlag == 1))
//	{
//		UART0.MasterFlag = 0;
//		Return_Lock_State_Cnt++;
//		UART0.TraBuf[0]	= 0x40;	   	//首字节
//		UART0.TraBuf[1]	= 0x53;		//命令字'S'
//		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

//		UART0.TraBuf[3]	= AsciiCode[ (Return_Lock_ID & 0x70) >> 4 ];	   	//LOCK编号-H
//		UART0.TraBuf[4]	= AsciiCode[ (Return_Lock_ID & 0x0f)      ];	   	//LOCK编号-L
//		if(Return_Lock_State)
//		{//柜门打开
//			UART0.TraBuf[5]	= 0x31;	   	//LOCK状态  1=打开 0=关闭
//		}
//		else
//		{//柜门关闭
//			UART0.TraBuf[5]	= 0x30;	   	//LOCK状态  1=打开 0=关闭
//		}

//		ptr = &UART0.TraBuf[1];
//		tmp	= LRC16(ptr,5);			//计算通信的LRC校验码

//		UART0.TraBuf[6] = ((tmp & 0xff00)>>8); 	//LRC高字节
//		UART0.TraBuf[7] =	tmp & 0x00ff; 		//LRC低字节

//		UART0.TraBuf[8] = 0x0d;
//		UART0.TraBuf[9] = 0x09;

//		UART0.MasterFlag = 0;
//		RS485_TEN();	   					//切换485芯片模式为 发送
//		UART0.TxdCnt = 9;					//设置发送长度
//		UART0.TranData = &UART0.TraBuf[1];	//初始化数据发送指针
//		SBUF0 = UART0.TraBuf[0];			//发送第一个数据
//	}
/*************************************************/			
	else if ( (State_Trans_Cnt == 3) && (UART0.MasterFlag == 1) )
	{//主动发送6次上位机无响应  就等待上位机发送指令
		UART0.MasterFlag = 0;		
		TR2 = 0;								//定时器2禁止工作
		UART0.MasterBusy = 0;		//清除主动发送忙标识
		State_Trans_Cnt  = 0;
//		Timer2Value 			= 0;
//		UART0.Master_Tran_Timer = 150;		//设定通信返回时间为75ms

		for(i=0;i<10;i++)
		{
			UART0.RevBuf[i] = 0;	//情况通信缓冲区
		}
		RS485_REN();		// 切换通讯为接受模式

	}
	else
	{
		TR2 = 0;								//定时器2禁止工作
		UART0.MasterFlag = 0;
		UART0.MasterBusy = 0;		//清除主动发送忙标识
	}
}

//通信处理程序 处理--410作为从机通信和作为主机通信
void Communication_Respone(void)
{
	uchar i;

	if (UART0.RevFlag)					//UART0.RevFlag：通讯响应 作为从机的通信处理标识
	{//串口接收上位机通信处理
		if (Slave_ID == 1)				//Slave_ID:	HexCode[UART0.RevBuf[2]]	   //通信协议 从机地址
		{//从机地址正确
			switch(UART0.RevBuf[1])
			{//功能码
				case 0x41://'A'使能电磁阀
						enable_lock();	
//						Return_Lock_ID = UART0.LOCK_ID;					 //当前电磁阀的编号 = 通信要求打开的电磁阀编号
//						if (0 == (UART0.LOCK_ID % 8))						 //如果电磁锁编号为8的倍数
//						{
//							Return_Lock_State = (LOCK_STATE[(UART0.LOCK_ID-1)/8]) & 0x01;    //反馈的当前电磁阀状态
//						}
//						else
//						{
//							Return_Lock_State = (LOCK_STATE[(UART0.LOCK_ID-1)/8] >> ( (8-UART0.LOCK_ID) % 8)) & 0x01; 
//						}
						break;
				case 0x42://'B'使能LED
						enable_led();	 	
						break;
				case 0x56://'V'显示版本号
					  show_version();
					  break;
				case 0x53://'S' 主机模式lock通信反馈
						if ( lock_return())
						{//校验OK
							State_Trans_Cnt = 0;  //清除发送计数
							Return_Lock_ID 	= 0;
//							Return_Lock_State_Cnt = 0;
							TR2 = 0;
							UART0.MasterBusy = 0;		//清除主动发送忙标识
							RS485_REN();		// 切换通讯为接受模式
	
							//状态数组更新							
							UART0.LockState[0] = UART0.LockState[1];
							UART0.LockState[1] = UART0.LockState[2];
							UART0.LockState[2] = UART0.LockState[3];
							UART0.LockState[3] = UART0.LockState[4];
							UART0.LockState[4] = 0;
						}
						else
						{//校验失败	重新发送

						}									
						for(i=0;i<10;i++)
						{
							UART0.RevBuf[i] = 0;	//情况通信缓冲区
						}
						break;
				case 0x54://'T' 主机模式button通信反馈
						if ( button_return())
						{//校验OK
							//关闭TR2
							//状态数组更新
							State_Trans_Cnt = 0;  //清除发送计数
							UART0.MasterBusy = 0;		//清除主动发送忙标识
							TR2 = 0;
							RS485_REN();		// 切换通讯为接受模式

							UART0.ButtonState[0] = UART0.ButtonState[1];
							UART0.ButtonState[1] = UART0.ButtonState[2];
							UART0.ButtonState[2] = UART0.ButtonState[3];
							UART0.ButtonState[3] = UART0.ButtonState[4];
							UART0.ButtonState[4] = 0;
						}
						else
						{//校验失败	重新发送

						}			

						for(i=0;i<10;i++)
						{
							UART0.RevBuf[i] = 0;	//情况通信缓冲区
						}
						break;
				default:
						for(i=0;i<10;i++)
						{
							UART0.RevBuf[i] = 0;	//情况通信缓冲区
						}
						RS485_REN();		// 切换通讯为接受模式
						break;
			}
		}
		else
		{//从机地址错误
			for(i=0;i<10;i++)
			{
				UART0.RevBuf[i] = 0;	//情况通信缓冲区
			}
			RS485_REN();		// 切换通讯为接受模式
		}	
		UART0.RevFlag = 0;	   //清除从机响应程序标识
	}
	else if ( (UART0.LockState[0] || UART0.ButtonState[0] || (Return_Lock_ID)) && (UART0.MasterBusy == 0) )   //当存储改变状态的电磁阀编号 或 当存储改变状态的按键编号 或当前电磁阀编号 有一个为1
	{//主动发送状态
		UART0.MasterBusy = 1;
		Timer2Value = 0;
		UART0.Master_Tran_Timer = 1;		//1.5ms定时，检查时间范围内是否有通信接受 ,主动发送模式计时器  用于检查通信占用和重新发送定时
		TR2 = 1;			//
	}
	else if (UART0.MasterFlag == 1)
	{//主动通信开始
		master_transmit();
	}
			
}

//串口中断
void Uart0_Interrupt (void) interrupt 4
{
	uchar tmp;
	if (RI0 == 1)												//接收数据中断标志位 = 1
	{//接收中断
		RI0 = 0;                           // Clear interrupt flag
		tmp = SBUF0;											//将串行数据缓冲器SBUF0中的值给变量tmp
		
		switch(tmp)
		{
			case 0x40://首字节接受初始化
					UART0.RevBuf[0] = tmp;					//将首字节放入接收数据缓冲器第1个字节
					UART0.RxdCnt = 1;								//串口接收字节个数计数器，接收计数器 = 1    
					break;
			case 0x09://结束字
					if (UART0.RevBuf[UART0.RxdCnt-1] == 0x0d)  //结束字的第1个字节 = 0x0d
					{//结束字正确
						UART0.RevBuf[UART0.RxdCnt] = tmp;
						UART0.RevFlag = ENABLE;		//处理上位机通信接受程序
						RS485_TEN();	//	切换通讯为发送状态 即：在处理完当前通讯帧之前，禁止接受通讯数据
					}
					else
					{
						UART0.RxdCnt = 0; 			
					}
					break;
			default:
					UART0.RevBuf[UART0.RxdCnt] = tmp;
					UART0.RxdCnt++;
					if(UART0.RxdCnt == UART0_Rxd_Length)			// UART0_Rxd_Length = 10
					{//乱码最大接收长度 10个字节
						UART0.RxdCnt = 0;
					}
					break;
		}
		
	}
	
	if (TI0 == 1)                   // Check if transmit flag is set
	{//发送中断
		TI0 = 0;                      // Clear interrupt flag
		
		if(UART0.TxdCnt)							//串口发送字节个数计数器
		{
			UART0.TxdCnt--;
			SBUF0 =  *UART0.TranData++;	//串口数据发送指针
		}
		else
		{
			for(tmp=0;tmp<10;tmp++)
			{
				UART0.RevBuf[tmp] = 0;	//清空接收缓冲区
			}
//			delay(50);			//485芯片模式转换延迟
			RS485_REN();		//发送完毕 切换为接受模式
		}
	
	}
}

//串口初始化
void UART0_Init(void)
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
                                       //        clear RI0 and TI0 bits
//   if (SYSCLK/BAUDRATE/2/256 < 1) 
//   {
//      TH1 = -(SYSCLK/BAUDRATE/2);
//      CKCON |=  0x08;                  // T1M = 1; SCA1:0 = xx
//   } 
//   else if (SYSCLK/BAUDRATE/2/256 < 4) 
//   {
//      TH1 = -(SYSCLK/BAUDRATE/2/4);
//      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
//      CKCON |=  0x01;
//   } 
//   else if (SYSCLK/BAUDRATE/2/256 < 12) 
//   {
//      TH1 = -(SYSCLK/BAUDRATE/2/12);
//      CKCON &= 0xF4;                  // T1M = 0; SCA1:0 = 00
//   } 
//   else if (SYSCLK/BAUDRATE/2/256 < 48) 
//   
//   {
//      TH1 = -(SYSCLK/BAUDRATE/2/48);
//      CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
//      CKCON |=  0x02;
//   } else 
//   {
//      ;                       		  // Error.  Unsupported baud rate
//   }
//   CKCON = 0x00;                       // Timer0 uses a 1:12 prescaler
//   TH1 = 0x96;
//   TL1 = 0x96;                          // init Timer1
	 TH1 = 0xCB;
   TL1 = 0xCB;                          // init Timer1
   TMOD &= 0x0f;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |= 0x20;
   TR1 	 = 1;                            // START Timer1
   IP 	|= 0x10;                         // Make UART high priority
   ES0 	 = 1;                            // Enable UART0 interrupts
	 EA = 1;
}

