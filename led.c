#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

////LED闪烁状态 关闭所有LED
//void BlinkState_DisableLed(void)
//{//
//	uchar i;
//
//	for(i=0;i<128;i++)
//	{
//		LED_DATA = 0;
//		Led_Sclk_RS();
//
//	}
//	LED_DATA = 0;
//	LED_DATA = 0;
//	LED_DATA = 0;
//	LED_DATA = 0;
//	Led_Lclk_RS();
//
//}

//检查是否有需要点亮的LED灯
void Led_Check(void)
{
	uchar i;

	if(UART0.LED_ID == 0)
	{//没有LED需要点亮
		;
	}
	else if(UART0.LED_ID == 0xff)
	{//关闭所有LED
		disable_led(0xff);
		UART0.LED_ID = 0;
	}
	else
	{//点亮LED
		led_enable(UART0.LED_ID);
		UART0.LED_ID = 0;
	}

//	//所有LED一起闪烁设置
//	if ( (UART0.LED_STATE) && (UART0.LED_Blink_Cnt == Timer0Value) )
//	{
//		UART0.LED_Blink_Cnt = Timer0Value + 20;	//500ms
//
//		if (UART0.LED_Blink_state == 1)
//		{//LED点亮
//			led_enable(251);
//			UART0.LED_Blink_state = 0;
//		}
//		else
//		{//LED熄灭
//			BlinkState_DisableLed();
//			UART0.LED_Blink_state = 1;
//		}
//	}

	//单个LED闪烁设置
	UART0.LED_STATE = 0;
	for(i=0;i<16;i++)
	{//检查是否有需要闪烁的LED
		UART0.LED_STATE |= LED_BLINK[i];
	}
	if ( (UART0.LED_STATE) && (UART0.LED_Blink_Cnt == Timer0Value) )
	{
		UART0.LED_Blink_Cnt = Timer0Value + 20;	//500ms

		if (UART0.LED_Blink_state == 1)
		{//LED点亮
			led_enable(241);
			UART0.LED_Blink_state = 0;
		}
		else
		{//LED熄灭
			disable_led(241);
//			BlinkState_DisableLed();
			UART0.LED_Blink_state = 1;
		}
	}

}

//enable LED
void led_enable(uchar led)
{   
	uchar i;

	switch((led-1)/8)
	{
		case 0://点亮LED的位置编号 1-8 
				led_1_8 |= 0x80 >> ( (led-1)%8 );
				break;
		case 1://点亮LED的位置编号 9-16
				led_9_16 |= 0x80 >> ( (led-1)%8 );
				break;
		case 2://点亮LED的位置编号 17-24
				led_17_24 |= 0x80 >> ( (led-1)%8 );
				break;
		case 3://点亮LED的位置编号 25-32
				led_25_32 |= 0x80 >> ( (led-1)%8 );
				break;
		case 4://点亮LED的位置编号 33-40
				led_33_40 |= 0x80 >> ( (led-1)%8 );
				break;
		case 5://点亮LED的位置编号 41-48 
				led_41_48 |= 0x80 >> ( (led-1)%8 );
				break;
		case 6://点亮LED的位置编号 49_56
				led_49_56 |= 0x80 >> ( (led-1)%8 );
				break;
		case 7://点亮LED的位置编号 57_64
				led_57_64 |= 0x80 >> ( (led-1)%8 );
				break;
		case 8://点亮LED的位置编号 65_72
				led_65_72 |= 0x80 >> ( (led-1)%8 );
				break;
		case 9://点亮LED的位置编号 73_80
				led_73_80 |= 0x80 >> ( (led-1)%8 );
				break;
		case 10://点亮LED的位置编号 81_88
				led_81_88 |= 0x80 >> ( (led-1)%8 );
				break;
		case 11://点亮LED的位置编号89_96
				led_89_96 |= 0x80 >> ( (led-1)%8 );
				break;
		case 12://点亮LED的位置编号 97_104
				led_97_104 |= 0x80 >> ( (led-1)%8 );
				break;
		case 13://点亮LED的位置编号 105_112
				led_105_112 |= 0x80 >> ( (led-1)%8 );
				break;
		case 14://点亮LED的位置编号 113_120
				led_113_120 |= 0x80 >> ( (led-1)%8 );
				break;
		case 15://点亮LED的位置编号 121_128
				led_121_128 |= 0x80 >> ( (led-1)%8 );
				break;
		case 31://所有LED闪烁
				break;
		case 30://单个LED闪烁
				//LED点亮
			  	led_1_8  	|= LED_BLINK[0];
			  	led_9_16 	|= LED_BLINK[1];
			  	led_17_24 	|= LED_BLINK[2];									
			  	led_25_32 	|= LED_BLINK[3];
				led_33_40 	|= LED_BLINK[4];
			  	led_41_48  	|= LED_BLINK[5];	  
			  	led_49_56 	|= LED_BLINK[6];
			  	led_57_64 	|= LED_BLINK[7];									
			  	led_65_72 	|= LED_BLINK[8];
					led_73_80 	|= LED_BLINK[9];
			  	led_81_88  	|= LED_BLINK[10];	  
			  	led_89_96 	|= LED_BLINK[11];
			  	led_97_104 	|= LED_BLINK[12];									
			  	led_105_112 |= LED_BLINK[13];
					led_113_120 |= LED_BLINK[14];
					led_121_128 |= LED_BLINK[15];
				
				break;
		default:
		
			  	led_1_8  		= 0;	  //关闭所有
			  	led_9_16 		= 0;
			  	led_17_24 	= 0;									
			  	led_25_32 	= 0;
					led_33_40 	= 0;
			  	led_41_48  	= 0;	  
			  	led_49_56 	= 0;
			  	led_57_64 	= 0;									
			  	led_65_72		= 0;
				  led_73_80 	= 0;
			  	led_81_88  	= 0;	  
			  	led_89_96 	= 0;
			  	led_97_104 	= 0;									
			  	led_105_112 = 0;
					led_113_120 = 0;
					led_121_128 = 0;

				break;
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_121_128 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_113_120 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_105_112 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_97_104 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_89_96 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_81_88 >> i) &0x1;
		Led_Sclk_RS();
	}
	for(i=0;i<8;i++)
	{
		LED_DATA = (led_73_80 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_65_72 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_57_64 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_49_56 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_41_48 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_33_40 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_25_32 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_17_24 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_9_16 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_1_8 >> i) &0x1;
		Led_Sclk_RS();
	}

	Led_Lclk_RS();

}

//熄灭LED
void disable_led(uchar led)
{
	uchar i;

	switch((led-1)/8)
	{
		case 0://熄灭LED的位置编号 1-8 
				led_1_8 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 1://熄灭LED的位置编号 9-16
				led_9_16 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 2://熄灭LED的位置编号 17-24
				led_17_24 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 3://熄灭LED的位置编号 25-32
				led_25_32 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 4://熄灭LED的位置编号 33-40
				led_33_40 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 5://熄灭LED的位置编号 41-48 
				led_41_48 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 6://熄灭LED的位置编号 49_56
				led_49_56 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 7://熄灭LED的位置编号 57_64
				led_57_64 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 8://熄灭LED的位置编号 65_72
				led_65_72 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 9://熄灭LED的位置编号 73_80
				led_73_80 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 10://熄灭LED的位置编号 81_88
				led_81_88 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 11://熄灭LED的位置编号89_96
				led_89_96 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 12://熄灭LED的位置编号 97_104
				led_97_104 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 13://熄灭LED的位置编号 105_112
				led_105_112 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 14://熄灭LED的位置编号 113_120
				led_113_120 &= ~(0x80 >> ( (led-1)%8 ));
				break;
		case 15://熄灭LED的位置编号 121_128
				led_121_128 &= ~(0x80 >> ( (led-1)%8 ));
				break;

		case 30://单个LED闪烁
				//LED点亮
			  	led_1_8  		&= ~LED_BLINK[0];
			  	led_9_16 		&= ~LED_BLINK[1];
			  	led_17_24 	&= ~LED_BLINK[2];									
			  	led_25_32 	&= ~LED_BLINK[3];
					led_33_40 	&= ~LED_BLINK[4];
			  	led_41_48  	&= ~LED_BLINK[5];	  
			  	led_49_56 	&= ~LED_BLINK[6];
			  	led_57_64 	&= ~LED_BLINK[7];									
			  	led_65_72 	&= ~LED_BLINK[8];
					led_73_80 	&= ~LED_BLINK[9];
			  	led_81_88  	&= ~LED_BLINK[10];	  
			  	led_89_96 	&= ~LED_BLINK[11];
			  	led_97_104 	&= ~LED_BLINK[12];									
			  	led_105_112 &= ~LED_BLINK[13];
					led_113_120 &= ~LED_BLINK[14];
					led_121_128 &= ~LED_BLINK[15];
				
				break;
		default:
			  	led_1_8  		= 0;	  //关闭所有
			  	led_9_16 		= 0;
			  	led_17_24 	= 0;									
			  	led_25_32 	= 0;
					led_33_40 	= 0;
			  	led_41_48  	= 0;	  
			  	led_49_56 	= 0;
			  	led_57_64 	= 0;									
					led_65_72 	= 0;
					led_73_80 	= 0;
			  	led_81_88  	= 0;	  
			  	led_89_96 	= 0;
			  	led_97_104 	= 0;									
			  	led_105_112 = 0;
					led_113_120 = 0;
					led_121_128 = 0;

				//清零 LED闪烁寄存器
				for(i=0;i<16;i++)
				{
					LED_BLINK[i] = 0;
				}

				break;
	}


	for(i=0;i<8;i++)
	{
		LED_DATA = (led_121_128 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_113_120 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_105_112 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_97_104 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_89_96 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_81_88 >> i) &0x1;
		Led_Sclk_RS();
	}
	for(i=0;i<8;i++)
	{
		LED_DATA = (led_73_80 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_65_72 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_57_64 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_49_56 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_41_48 >> i) &0x1;
		Led_Sclk_RS();
	}
	for(i=0;i<8;i++)
	{
		LED_DATA = (led_33_40 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_25_32 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_17_24 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_9_16 >> i) &0x1;
		Led_Sclk_RS();
	}

	for(i=0;i<8;i++)
	{
		LED_DATA = (led_1_8 >> i) &0x1;
		Led_Sclk_RS();
	}

	Led_Lclk_RS();
}


//

