#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

//LOCK && BUTTON ״̬���
void State_input(void)
{
	uchar i,j;

//����״̬����
if (Anti_Shake_Flag == 0)
{//����֮ǰ�ļ��  ��⵽�仯��ʹ�ܷ�����ʶ
	
	BUTTON_SL = 0;
	delay(5);
	BUTTON_SL = 1;
	delay(5);
//	Button_SL_RS();	 	//ʹ��SL ��ð���״̬
//	delay(5);
	
	if (~BUTTON)
	{//��һ���������ǵ�128��LED
		Anti_Shake_Flag = 1;		//������������ʶ
		Anti_Shake_Timer = Timer0Value + 4;	//����ʱ�� 75ms-100ms
	}


	for(i=127;i>0;i--)
	{
		BUTTON_CLK = 0;
		delay(5);
		BUTTON_CLK = 1;
		delay(5);
//		Button_CLK_RS();	//��ȡ��������

		if (~BUTTON)
		{//������������Ϩ���Ӧ��ŵ�LED
			Anti_Shake_Flag = 1;		//������������ʶ
			Anti_Shake_Timer = Timer0Value + 4;	//����ʱ�� 75ms-100ms
		}
	}

//����״̬����
	FEEDBACK_SL = 0;
	delay(5);
	FEEDBACK_SL = 1;
	delay(5);
//	Feedback_SL_RS();	 	//ʹ��SL ��õ�ŷ�״̬
//	delay(5);
	
	if ( LOCK_FEEDBACK != (~LOCK_STATE[4] & 0x1) )
	{//��һ���������ǵ�40�ŵ�ŷ�
			Anti_Shake_Flag = 1;		//������������ʶ
			Anti_Shake_Timer = Timer0Value + 80;	//����ʱ�� 2s
	}

	for(i=39;i>0;i--)
	{
		FEEDBACK_CLK = 0;
		delay(5);
		FEEDBACK_CLK = 1;
		delay(5);
//		Feedban_CLK_RS();	//��ȡ��������

		if (0 == (i%8))
		{
			if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8]) & 0x01) )
			{//�ı���״̬�ĵ�ŷ����
					Anti_Shake_Flag = 1;		//������������ʶ
					Anti_Shake_Timer = Timer0Value + 80;	//����ʱ�� 2s
			}
		}
		else
		{
			if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8] >> (8-i%8)) & 0x01) )
			{//�ı���״̬�ĵ�ŷ����
					Anti_Shake_Flag = 1;		//������������ʶ
					Anti_Shake_Timer = Timer0Value + 80;	//����ʱ�� 2s
			}
		}
	}
}
else
{//�������  �б仯����±仯����
	if (Anti_Shake_Timer == Timer0Value)
	{
		Anti_Shake_Flag = 0;		//�����������ʶ

		BUTTON_SL = 0;
		delay(5);
		BUTTON_SL = 1;
		delay(5);
//		Button_SL_RS();	 	//ʹ��SL ��ð���״̬
//		delay(5);
		
		if (~BUTTON)
		{//��һ���������ǵ�128��LED
			for(i=0;(i<i+UART0.ButtonState[i] && i<5);i++);	 //�洢�������
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
//			Button_CLK_RS();	//��ȡ��������
	
			if (~BUTTON)
			{//������������Ϩ���Ӧ��ŵ�LED
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
	
	//����״̬����
		FEEDBACK_SL = 0;
		delay(5);
		FEEDBACK_SL = 1;
		delay(5);
//		Feedback_SL_RS();	 	//ʹ��SL ��õ�ŷ�״̬
//		delay(5);
		
		if ( LOCK_FEEDBACK != (~LOCK_STATE[4] & 0x1) )
		{//��һ���������ǵ�40�ŵ�ŷ�
	//		UART0.LOCK_State_Num = 40;
			for(i=0;(i<i+UART0.LockState[i] && i<5);i++);	 //�洢 LOCK ��ź�״̬
			if (i < 5)
			{
				if(LOCK_FEEDBACK)
				{//LOCK�ر�
					UART0.LockState[i] = 40;
					LOCK_STATE[4] &= ~0x01; //���µ�ŷ�״̬
				}
				else
				{//LOCK��
					UART0.LockState[i] = 0x80 | 40;
					LOCK_STATE[4] |= 0x01; //���µ�ŷ�״̬
				}				
			}
	
		}
	
		for(i=39;i>0;i--)
		{
			FEEDBACK_CLK = 0;
			delay(5);
			FEEDBACK_CLK = 1;
			delay(5);
//			Feedban_CLK_RS();	//��ȡ��������
	
			if (0 == (i%8))
			{
				if (LOCK_FEEDBACK != ((~LOCK_STATE[(i-1)/8]) & 0x01) )
				{//�ı���״̬�ĵ�ŷ����
	//					UART0.LOCK_State_Num = i;	
					for(j=0;(j<j+UART0.LockState[j] && j<5);j++);	 //�洢 LOCK ��ź�״̬
					if (j < 5)
					{
						if(LOCK_FEEDBACK)
						{//LOCK�ر�
							UART0.LockState[j] = i;
							LOCK_STATE[(i-1)/8] &= 0xfe; //���µ�ŷ�״�
						}
						else
						{//LOCK��
							UART0.LockState[j] = 0x80 | i;
							LOCK_STATE[(i-1)/8] |= 1; //���µ�ŷ�״�
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
				{//�ı���״̬�ĵ�ŷ����
	//					UART0.LOCK_State_Num = i;	
					for(j=0;(j<j+UART0.LockState[j] && j<5);j++);	 //�洢 LOCK ��ź�״̬
					if (j < 5)
					{
						if(LOCK_FEEDBACK)
						{//LOCK�ر�
							UART0.LockState[j] = i;
							LOCK_STATE[(i-1)/8] &= ~(1 << (8-i%8)); //���µ�ŷ�״̬
						}
						else
						{//LOCK��
							UART0.LockState[j] = 0x80 | i;
							LOCK_STATE[(i-1)/8] |= 1 << (8-i%8); //���µ�ŷ�״̬
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


