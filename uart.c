#include <C8051F410.h>                 // SFR declarations
#include <intrins.h>                 // SFR declarations
#include <zyy_declare.h>

#define SYSCLK      24500000           // SYSCLK frequency in Hz
#define BAUDRATE        9600           // Baud rate of UART in bps

//RS485 ������ƶ��ӳ���
void delay(uchar time)
{
	while(time--);
}

//LRCУ�� ע�� ����length=ͨ�Ž��յĸ���-
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
//ͨ��ʹ��LOCK
void enable_lock(void)
{
	uint tmp;							//����ͨ�ŵ�LRCУ����
	uchar *ptr;						//���ݽ���ָ��

  ptr = &UART0.RevBuf[1];
	tmp = LRC16(ptr,UART0.RxdCnt-4);		//����ͨ�ŵ�LRCУ����
	if ( tmp == ((LRC_H << 8) | LRC_L) )
	{//LRCУ����ȷ
		UART0.LOCK_ID 	= Lock_ID;				//ͨ��Ҫ��򿪵ĵ�ŷ���� = �����Ҫʹ�ܵĵ�ŷ����	Lock_ID:( (HexCode[UART0.RevBuf[3]] << 4) | HexCode[UART0.RevBuf[4]] )	   //ͨ��Э�� ���������
	    
		//������	�뷢��������ͬ
		for(tmp=0;tmp<9;tmp++)
		{
			UART0.TraBuf[tmp] = UART0.RevBuf[tmp];
		}
		RS485_TEN();	   										//�л�485оƬģʽΪ ����
		UART0.TxdCnt		= UART0.RxdCnt;		//���÷��ͳ���
		UART0.TranData 	= &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
		SBUF0 					= UART0.TraBuf[0];	//���͵�һ������
	}
	else
	{//LRCУ�����
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.RevBuf[tmp] = 0;	//���ͨ�Ž��ջ�����
		}
		RS485_REN();		// �л�ͨѶΪ����ģʽ
	}
}

//ͨ��ʹ��Led
void enable_led(void)
{
	uint tmp;
	uchar *ptr;

  ptr = &UART0.RevBuf[1];
	tmp = LRC16(ptr,UART0.RxdCnt-4);		//����ͨ�ŵ�LRCУ����
	if ( tmp == ((LRC_H << 8) | LRC_L) )
	{//LRCУ����ȷ
		UART0.LED_ID 	= Led_ID;			//�����Ҫʹ�ܵ�LED���	
		UART0.LED_STATE	= Led_State;		//LED��˸״̬	

		if (UART0.LED_STATE == 1)
		{//LED��˸
			LED_BLINK[(UART0.LED_ID-1)/8] |= 0x80 >> ((UART0.LED_ID-1)%8); 	//ʹ����Ҫ��˸��LED��bitλ
			UART0.LED_Blink_Cnt 	= Timer0Value + 20;	//500ms
			UART0.LED_Blink_state 	= 0;				//��ʼ����˸״̬Ϊ��
		}
		else if (UART0.LED_STATE == 0)
		{
			LED_BLINK[(UART0.LED_ID-1)/8] &= ~(0x80 >> ((UART0.LED_ID-1)%8)); //ʧЧ������˸��LED��bitλ
		}
		else
		{//Ϩ��LED	led_state = 0xff
			disable_led(UART0.LED_ID);	 	//Ϩ��LED
			LED_BLINK[(UART0.LED_ID-1)/8] &= ~(0x80 >> ((UART0.LED_ID-1)%8)); //ʧЧ������˸��LED��bitλ
			UART0.LED_ID = 0;
		}

//		if(UART0.LED_STATE)
//		{//LED��Ҫ��˸  ��ʼ��LED��˸��ʱ��
//			UART0.LED_Blink_Cnt 	= Timer0Value + 20;	//500ms
//			UART0.LED_Blink_state 	= 0;				//��ʼ����˸״̬Ϊ��
//		}
		
		//������	�뷢��������ͬ
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.TraBuf[tmp] = UART0.RevBuf[tmp];
		}
		RS485_TEN();	   						//�л�485оƬģʽΪ ����
		UART0.TxdCnt	= UART0.RxdCnt;		//���÷��ͳ���
		UART0.TranData 	= &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
		SBUF0 			= UART0.TraBuf[0];	//���͵�һ������
	}
	else
	{//LRCУ�����
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.RevBuf[tmp] = 0;	//���ͨ�Ż�����
		}
		RS485_REN();		// �л�ͨѶΪ����ģʽ
	}
}

/******************************V2.15���Ӳ���(����)*********************************/
//��ʾ�汾��
void show_version(void)
{
	uint tmp;							//����ͨ�ŵ�LRCУ�������
	uchar *ptr;						//���ݽ���ָ��

  ptr = &UART0.RevBuf[1];
	tmp = LRC16(ptr,UART0.RxdCnt-4);		//����ͨ�ŵ�LRCУ����
	if ( tmp == ((LRC_H << 8) | LRC_L) )
	{//LRCУ����ȷ	    
		//������	�뷢��������ͬ
		RS485_TEN();	   										//�л�485оƬģʽΪ ����
		UART0.TxdCnt		= UART0.RxdCnt;		//���÷��ͳ���
		UART0.TranData 	= &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
		SBUF0 					= UART0.TraBuf[0];	//���͵�һ������

		UART0.TraBuf[0]	= 0x40;	   	//���ֽ�
		UART0.TraBuf[1]	= 0x56;		//������'V'
		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

		UART0.TraBuf[3]	= 0x31;	   	//�汾��-H   �汾�ţ�15 (�ɸı�)
		UART0.TraBuf[4]	= 0x35;	   	//�汾��-L

		ptr = &UART0.TraBuf[1];
		tmp	= LRC16(ptr,4);			//����ͨ�ŵ�LRCУ����

		UART0.TraBuf[5] = ((tmp & 0xff00)>>8); 	//LRC���ֽ�
		UART0.TraBuf[6] =	tmp & 0x00ff; 		//LRC���ֽ�

		UART0.TraBuf[7] = 0x0d;
		UART0.TraBuf[8] = 0x09;

		UART0.MasterFlag = 0;
		RS485_TEN();	   					//�л�485оƬģʽΪ ����
		UART0.TxdCnt = 8;					//���÷��ͳ���
		UART0.TranData = &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
		SBUF0 = UART0.TraBuf[0];			//���͵�һ������
	}
	
	else
	{//LRCУ�����
		for(tmp=0;tmp<10;tmp++)
		{
			UART0.RevBuf[tmp] = 0;	//���ͨ�Ž��ջ�����
		}
		RS485_REN();		// �л�ͨѶΪ����ģʽ
	}	
}
/*******************************V2.15���Ӳ���(����)**********************************/

//����lock״̬�ӻ��ķ�����Ϣ
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

//����button״̬�ӻ��ķ�����Ϣ
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

//��������LOCK״̬�Ͱ���״̬
void master_transmit(void)
{
	uchar *ptr;
	uint tmp;
	uchar i;

	if ( (UART0.LockState[0] != 0) && (UART0.MasterFlag == 1) &&(State_Trans_Cnt != 3) )		//���Ӽ�ⷢ�ʹ����ж�
	{//LOCK״̬����
		State_Trans_Cnt++;    //���Ӽ�ⷢ�ʹ���

		UART0.TraBuf[0]	= 0x40;	   	//���ֽ�
		UART0.TraBuf[1]	= 0x53;		//������'S'
		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

		UART0.TraBuf[3]	= AsciiCode[ (UART0.LockState[0] & 0x70) >> 4 ];	   	//LOCK���-H
		UART0.TraBuf[4]	= AsciiCode[ (UART0.LockState[0] & 0x0f)      ];	   	//LOCK���-L
		if(UART0.LockState[0] & 0x80)
		{//���Ŵ�
			UART0.TraBuf[5]	= 0x31;	   	//LOCK״̬  1=�� 0=�ر�
		}
		else
		{//���Źر�
			UART0.TraBuf[5]	= 0x30;	   	//LOCK״̬  1=�� 0=�ر�
		}

		ptr = &UART0.TraBuf[1];
		tmp	= LRC16(ptr,5);			//����ͨ�ŵ�LRCУ����

		UART0.TraBuf[6] = ((tmp & 0xff00)>>8); 	//LRC���ֽ�
		UART0.TraBuf[7] =	tmp & 0x00ff; 				//LRC���ֽ�

		UART0.TraBuf[8] = 0x0d;
		UART0.TraBuf[9] = 0x09;

		UART0.MasterFlag = 0;
		RS485_TEN();	   					//�л�485оƬģʽΪ ����
		UART0.TxdCnt = 9;					//���÷��ͳ���
		UART0.TranData = &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
		SBUF0 = UART0.TraBuf[0];			//���͵�һ������

	}
	else if ( (UART0.ButtonState[0] != 0) && (UART0.MasterFlag == 1) &&(State_Trans_Cnt != 3) )  //���Ӽ�ⷢ�ʹ����ж�
	{//BUTTON״̬����
		 State_Trans_Cnt++;    //���Ӽ�ⷢ�ʹ���
		
		UART0.TraBuf[0]	= 0x40;	   	//���ֽ�
		UART0.TraBuf[1]	= 0x54;		//������'S'
		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

		UART0.TraBuf[3]	= AsciiCode[ (UART0.ButtonState[0] & 0xf0) >> 4 ];	   	//BUTTON���-H
		UART0.TraBuf[4]	= AsciiCode[ (UART0.ButtonState[0] & 0x0f)      ];	   	//BUTTON���-L

		ptr = &UART0.TraBuf[1];
		tmp	= LRC16(ptr,4);			//����ͨ�ŵ�LRCУ����

		UART0.TraBuf[5] = ((tmp & 0xff00)>>8); 	//LRC���ֽ�
		UART0.TraBuf[6] =	tmp & 0x00ff; 		//LRC���ֽ�

		UART0.TraBuf[7] = 0x0d;
		UART0.TraBuf[8] = 0x09;

		UART0.MasterFlag = 0;
		RS485_TEN();	   					//�л�485оƬģʽΪ ����
		UART0.TxdCnt = 8;					//���÷��ͳ���
		UART0.TranData = &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
		SBUF0 = UART0.TraBuf[0];			//���͵�һ������
	}
	
//**************������λ�����Ϳ���ָ��,��λ����ִ���궯��������λ�����͵����״ָ̬��************/
//	else if ( (Return_Lock_ID) && (Return_Lock_State_Cnt != 3) && (UART0.MasterFlag == 1))
//	{
//		UART0.MasterFlag = 0;
//		Return_Lock_State_Cnt++;
//		UART0.TraBuf[0]	= 0x40;	   	//���ֽ�
//		UART0.TraBuf[1]	= 0x53;		//������'S'
//		UART0.TraBuf[2]	= 0x31;		//SLAVE_ID = 1

//		UART0.TraBuf[3]	= AsciiCode[ (Return_Lock_ID & 0x70) >> 4 ];	   	//LOCK���-H
//		UART0.TraBuf[4]	= AsciiCode[ (Return_Lock_ID & 0x0f)      ];	   	//LOCK���-L
//		if(Return_Lock_State)
//		{//���Ŵ�
//			UART0.TraBuf[5]	= 0x31;	   	//LOCK״̬  1=�� 0=�ر�
//		}
//		else
//		{//���Źر�
//			UART0.TraBuf[5]	= 0x30;	   	//LOCK״̬  1=�� 0=�ر�
//		}

//		ptr = &UART0.TraBuf[1];
//		tmp	= LRC16(ptr,5);			//����ͨ�ŵ�LRCУ����

//		UART0.TraBuf[6] = ((tmp & 0xff00)>>8); 	//LRC���ֽ�
//		UART0.TraBuf[7] =	tmp & 0x00ff; 		//LRC���ֽ�

//		UART0.TraBuf[8] = 0x0d;
//		UART0.TraBuf[9] = 0x09;

//		UART0.MasterFlag = 0;
//		RS485_TEN();	   					//�л�485оƬģʽΪ ����
//		UART0.TxdCnt = 9;					//���÷��ͳ���
//		UART0.TranData = &UART0.TraBuf[1];	//��ʼ�����ݷ���ָ��
//		SBUF0 = UART0.TraBuf[0];			//���͵�һ������
//	}
/*************************************************/			
	else if ( (State_Trans_Cnt == 3) && (UART0.MasterFlag == 1) )
	{//��������6����λ������Ӧ  �͵ȴ���λ������ָ��
		UART0.MasterFlag = 0;		
		TR2 = 0;								//��ʱ��2��ֹ����
		UART0.MasterBusy = 0;		//�����������æ��ʶ
		State_Trans_Cnt  = 0;
//		Timer2Value 			= 0;
//		UART0.Master_Tran_Timer = 150;		//�趨ͨ�ŷ���ʱ��Ϊ75ms

		for(i=0;i<10;i++)
		{
			UART0.RevBuf[i] = 0;	//���ͨ�Ż�����
		}
		RS485_REN();		// �л�ͨѶΪ����ģʽ

	}
	else
	{
		TR2 = 0;								//��ʱ��2��ֹ����
		UART0.MasterFlag = 0;
		UART0.MasterBusy = 0;		//�����������æ��ʶ
	}
}

//ͨ�Ŵ������ ����--410��Ϊ�ӻ�ͨ�ź���Ϊ����ͨ��
void Communication_Respone(void)
{
	uchar i;

	if (UART0.RevFlag)					//UART0.RevFlag��ͨѶ��Ӧ ��Ϊ�ӻ���ͨ�Ŵ����ʶ
	{//���ڽ�����λ��ͨ�Ŵ���
		if (Slave_ID == 1)				//Slave_ID:	HexCode[UART0.RevBuf[2]]	   //ͨ��Э�� �ӻ���ַ
		{//�ӻ���ַ��ȷ
			switch(UART0.RevBuf[1])
			{//������
				case 0x41://'A'ʹ�ܵ�ŷ�
						enable_lock();	
//						Return_Lock_ID = UART0.LOCK_ID;					 //��ǰ��ŷ��ı�� = ͨ��Ҫ��򿪵ĵ�ŷ����
//						if (0 == (UART0.LOCK_ID % 8))						 //�����������Ϊ8�ı���
//						{
//							Return_Lock_State = (LOCK_STATE[(UART0.LOCK_ID-1)/8]) & 0x01;    //�����ĵ�ǰ��ŷ�״̬
//						}
//						else
//						{
//							Return_Lock_State = (LOCK_STATE[(UART0.LOCK_ID-1)/8] >> ( (8-UART0.LOCK_ID) % 8)) & 0x01; 
//						}
						break;
				case 0x42://'B'ʹ��LED
						enable_led();	 	
						break;
				case 0x56://'V'��ʾ�汾��
					  show_version();
					  break;
				case 0x53://'S' ����ģʽlockͨ�ŷ���
						if ( lock_return())
						{//У��OK
							State_Trans_Cnt = 0;  //������ͼ���
							Return_Lock_ID 	= 0;
//							Return_Lock_State_Cnt = 0;
							TR2 = 0;
							UART0.MasterBusy = 0;		//�����������æ��ʶ
							RS485_REN();		// �л�ͨѶΪ����ģʽ
	
							//״̬�������							
							UART0.LockState[0] = UART0.LockState[1];
							UART0.LockState[1] = UART0.LockState[2];
							UART0.LockState[2] = UART0.LockState[3];
							UART0.LockState[3] = UART0.LockState[4];
							UART0.LockState[4] = 0;
						}
						else
						{//У��ʧ��	���·���

						}									
						for(i=0;i<10;i++)
						{
							UART0.RevBuf[i] = 0;	//���ͨ�Ż�����
						}
						break;
				case 0x54://'T' ����ģʽbuttonͨ�ŷ���
						if ( button_return())
						{//У��OK
							//�ر�TR2
							//״̬�������
							State_Trans_Cnt = 0;  //������ͼ���
							UART0.MasterBusy = 0;		//�����������æ��ʶ
							TR2 = 0;
							RS485_REN();		// �л�ͨѶΪ����ģʽ

							UART0.ButtonState[0] = UART0.ButtonState[1];
							UART0.ButtonState[1] = UART0.ButtonState[2];
							UART0.ButtonState[2] = UART0.ButtonState[3];
							UART0.ButtonState[3] = UART0.ButtonState[4];
							UART0.ButtonState[4] = 0;
						}
						else
						{//У��ʧ��	���·���

						}			

						for(i=0;i<10;i++)
						{
							UART0.RevBuf[i] = 0;	//���ͨ�Ż�����
						}
						break;
				default:
						for(i=0;i<10;i++)
						{
							UART0.RevBuf[i] = 0;	//���ͨ�Ż�����
						}
						RS485_REN();		// �л�ͨѶΪ����ģʽ
						break;
			}
		}
		else
		{//�ӻ���ַ����
			for(i=0;i<10;i++)
			{
				UART0.RevBuf[i] = 0;	//���ͨ�Ż�����
			}
			RS485_REN();		// �л�ͨѶΪ����ģʽ
		}	
		UART0.RevFlag = 0;	   //����ӻ���Ӧ�����ʶ
	}
	else if ( (UART0.LockState[0] || UART0.ButtonState[0] || (Return_Lock_ID)) && (UART0.MasterBusy == 0) )   //���洢�ı�״̬�ĵ�ŷ���� �� ���洢�ı�״̬�İ������ ��ǰ��ŷ���� ��һ��Ϊ1
	{//��������״̬
		UART0.MasterBusy = 1;
		Timer2Value = 0;
		UART0.Master_Tran_Timer = 1;		//1.5ms��ʱ�����ʱ�䷶Χ���Ƿ���ͨ�Ž��� ,��������ģʽ��ʱ��  ���ڼ��ͨ��ռ�ú����·��Ͷ�ʱ
		TR2 = 1;			//
	}
	else if (UART0.MasterFlag == 1)
	{//����ͨ�ſ�ʼ
		master_transmit();
	}
			
}

//�����ж�
void Uart0_Interrupt (void) interrupt 4
{
	uchar tmp;
	if (RI0 == 1)												//���������жϱ�־λ = 1
	{//�����ж�
		RI0 = 0;                           // Clear interrupt flag
		tmp = SBUF0;											//���������ݻ�����SBUF0�е�ֵ������tmp
		
		switch(tmp)
		{
			case 0x40://���ֽڽ��ܳ�ʼ��
					UART0.RevBuf[0] = tmp;					//�����ֽڷ���������ݻ�������1���ֽ�
					UART0.RxdCnt = 1;								//���ڽ����ֽڸ��������������ռ����� = 1    
					break;
			case 0x09://������
					if (UART0.RevBuf[UART0.RxdCnt-1] == 0x0d)  //�����ֵĵ�1���ֽ� = 0x0d
					{//��������ȷ
						UART0.RevBuf[UART0.RxdCnt] = tmp;
						UART0.RevFlag = ENABLE;		//������λ��ͨ�Ž��ܳ���
						RS485_TEN();	//	�л�ͨѶΪ����״̬ �����ڴ����굱ǰͨѶ֮֡ǰ����ֹ����ͨѶ����
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
					{//���������ճ��� 10���ֽ�
						UART0.RxdCnt = 0;
					}
					break;
		}
		
	}
	
	if (TI0 == 1)                   // Check if transmit flag is set
	{//�����ж�
		TI0 = 0;                      // Clear interrupt flag
		
		if(UART0.TxdCnt)							//���ڷ����ֽڸ���������
		{
			UART0.TxdCnt--;
			SBUF0 =  *UART0.TranData++;	//�������ݷ���ָ��
		}
		else
		{
			for(tmp=0;tmp<10;tmp++)
			{
				UART0.RevBuf[tmp] = 0;	//��ս��ջ�����
			}
//			delay(50);			//485оƬģʽת���ӳ�
			RS485_REN();		//������� �л�Ϊ����ģʽ
		}
	
	}
}

//���ڳ�ʼ��
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

