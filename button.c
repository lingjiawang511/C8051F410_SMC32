#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

//LOCK && BUTTON ×´Ì¬¼ì²é
void State_input(void)
{
	uchar i,j;

//°´¼ü×´Ì¬¶ÁÈë
if (Anti_Shake_Flag == 0)
{//·À¶¶Ö®Ç°µÄ¼ì²â  ¼ì²âµ½±ä»¯ÔòÊ¹ÄÜ·À¶¶±êÊ¶
	
	BUTTON_SL = 0;
	delay(5);
	BUTTON_SL = 1;
	delay(5);
//	Button_SL_RS();	 	//Ê¹ÄÜSL »ñµÃ°´¼ü×´Ì¬
//	delay(5);
	
	if (~BUTTON)
	{//µÚÒ»¸ö¶Á³öµÄÊÇµÚ128ºÅLED
		Anti_Shake_Flag = 1;		//½¨Á¢·À¶¶¼ì²â±êÊ¶
		Anti_Shake_Timer = Timer0Value + 4;	//·À¶¶Ê±¼ä 75ms-100ms
	}


	for(i=127;i>0;i--)
	{
		BUTTON_CLK = 0;
		delay(5);
		BUTTON_CLK = 1;
		delay(5);
//		Button_CLK_RS();	//¶ÁÈ¡°´¼üÊäÈë

		if (~BUTTON)
		{//°´¼ü±»°´ÏÂÔòÏ¨Ãð¶ÔÓ¦±àºÅµÄLED
			Anti_Shake_Flag = 1;		//½¨Á¢·À¶¶¼ì²â±êÊ¶
			Anti_Shake_Timer = Timer0Value + 4;	//·À¶¶Ê±¼ä 75ms-100ms
		}
	}

//¹ñÃÅ×´Ì¬¶ÁÈë
	FEEDBACK_SL = 0;
	delay(5);
	FEEDBACK_SL = 1;
	delay(5);
//	Feedback_SL_RS();	 	//Ê¹ÄÜSL »ñµÃµç´Å·§×´Ì¬
//	delay(5);
	
	if ( LOCK_FEEDBACK != (~LOCK_STATE[4] & 0x1) )
	{//µÚÒ»¸ö¶Á³öµÄÊÇµÚ40ºÅµç´Å·§
			Anti_Shake_Flag = 1;		//½¨Á¢·À¶¶¼ì²â±êÊ¶
			Anti_Shake_Timer = Timer0Value + 80;	//·À¶¶Ê±¼ä 2s
	}

	for(i=39;i>0;i--)
	{
		FEEDBACK_CLK = 0;
		delay(5);
		FEEDBACK_CLK = 1;
		delay(5);
//		Feedban_CLK_RS();	//¶ÁÈ¡°´¼üÊäÈë

		if (0 == (i%8))
		{
			if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8]) & 0x01) )
			{//¸Ä±äÁË×´Ì¬µÄµç´Å·§±àºÅ
					Anti_Shake_Flag = 1;		//½¨Á¢·À¶¶¼ì²â±êÊ¶
					Anti_Shake_Timer = Timer0Value + 80;	//·À¶¶Ê±¼ä 2s
			}
		}
		else
		{
			if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8] >> (8-i%8)) & 0x01) )
			{//¸Ä±äÁË×´Ì¬µÄµç´Å·§±àºÅ
					Anti_Shake_Flag = 1;		//½¨Á¢·À¶¶¼ì²â±êÊ¶
					Anti_Shake_Timer = Timer0Value + 80;	//·À¶¶Ê±¼ä 2s
			}
		}
	}
}
else
{//·À¶¶¼ì²â  ÓÐ±ä»¯ºó¸üÐÂ±ä»¯Êý×é
	if (Anti_Shake_Timer == Timer0Value)
	{
		Anti_Shake_Flag = 0;		//Çå³ý·À¶¶¼ì²â±êÊ¶

		BUTTON_SL = 0;
		delay(5);
		BUTTON_SL = 1;
		delay(5);
//		Button_SL_RS();	 	//Ê¹ÄÜSL »ñµÃ°´¼ü×´Ì¬
//		delay(5);
		
		if (~BUTTON)
		{//µÚÒ»¸ö¶Á³öµÄÊÇµÚ128ºÅLED
			for(i=0;(i<i+UART0.ButtonState[i] && i<5);i++);	 //´æ´¢°´¼ü±àºÅ
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
//			Button_CLK_RS();	//¶ÁÈ¡°´¼üÊäÈë
	
			if (~BUTTON)
			{//°´¼ü±»°´ÏÂÔòÏ¨Ãð¶ÔÓ¦±àºÅµÄLED
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
	
	//¹ñÃÅ×´Ì¬¶ÁÈë
		FEEDBACK_SL = 0;
		delay(5);
		FEEDBACK_SL = 1;
		delay(5);
//		Feedback_SL_RS();	 	//Ê¹ÄÜSL »ñµÃµç´Å·§×´Ì¬
//		delay(5);
		
		if ( LOCK_FEEDBACK != (~LOCK_STATE[4] & 0x1) )
		{//µÚÒ»¸ö¶Á³öµÄÊÇµÚ40ºÅµç´Å·§
	//		UART0.LOCK_State_Num = 40;
			for(i=0;(i<i+UART0.LockState[i] && i<5);i++);	 //´æ´¢ LOCK ±àºÅºÍ×´Ì¬
			if (i < 5)
			{
				if(LOCK_FEEDBACK)
				{//LOCK¹Ø±Õ
					UART0.LockState[i] = 40;
					LOCK_STATE[4] &= ~0x01; //¸üÐÂµç´Å·§×´Ì¬
				}
				else
				{//LOCK´ò¿ª
					UART0.LockState[i] = 0x80 | 40;
					LOCK_STATE[4] |= 0x01; //¸üÐÂµç´Å·§×´Ì¬
				}				
			}
	
		}
	
		for(i=39;i>0;i--)
		{
			FEEDBACK_CLK = 0;
			delay(5);
			FEEDBACK_CLK = 1;
			delay(5);
//			Feedban_CLK_RS();	//¶ÁÈ¡°´¼üÊäÈë
	
			if (0 == (i%8))
			{
				if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8]) & 0x01) )
				{//¸Ä±äÁË×´Ì¬µÄµç´Å·§±àºÅ
	//					UART0.LOCK_State_Num = i;	
					for(j=0;(j<j+UART0.LockState[j] && j<5);j++);	 //´æ´¢ LOCK ±àºÅºÍ×´Ì¬
					if (j < 5)
					{
						if(LOCK_FEEDBACK)
						{//LOCK¹Ø±Õ
							UART0.LockState[j] = i;
							LOCK_STATE[(i-1)/8] &= 0xfe; //¸üÐÂµç´Å·§×´Ì
						}
						else
						{//LOCK´ò¿ª
							UART0.LockState[j] = 0x80 | i;
							LOCK_STATE[(i-1)/8] |= 1; //¸üÐÂµç´Å·§×´Ì
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
				{//¸Ä±äÁË×´Ì¬µÄµç´Å·§±àºÅ
	//					UART0.LOCK_State_Num = i;	
					for(j=0;(j<j+UART0.LockState[j] && j<5);j++);	 //´æ´¢ LOCK ±àºÅºÍ×´Ì¬
					if (j < 5)
					{
						if(LOCK_FEEDBACK)
						{//LOCK¹Ø±Õ
							UART0.LockState[j] = i;
							LOCK_STATE[(i-1)/8] &= ~(1 << (8-i%8)); //¸üÐÂµç´Å·§×´Ì¬
						}
						else
						{//LOCK´ò¿ª
							UART0.LockState[j] = 0x80 | i;
							LOCK_STATE[(i-1)/8] |= 1 << (8-i%8); //¸üÐÂµç´Å·§×´Ì¬
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


