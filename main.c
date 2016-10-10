/**********************************************
���ƣ�����ҩ��(��׼�桪��ȫ�������)     ���õ�·�壺SMC_V2.13 & SMC_V2.14 PCB  

��ʷ�޸İ汾�ţ�V2.13	  �޸����ڣ�2015-12-08
�޸�ԭ��1����������Ŀ��Ʋ���ȫ������Ϊ����ȫ�����������Ϊ��׼����
					2�������˵��������1S�����������û���ȶ�״̬�·�������
					3���޸��˴򿪳��벻�ظ����ݣ����ϳ����ٻظ�����
					4�����������Ʒ������ݴ���

��ʷ�޸İ汾�ţ�V2.14   �޸����ڣ�2015-12-11
�޸�ԭ��1��������λ�����Ϳ���ָ��,��λ����ִ���궯��������λ�����͵����״ָ̬��

���°汾�ţ�V2.15   �޸����ڣ�2016-9-22 ~2016-09-26
�޸�ԭ��1���˶Գ���ͨѶ��������
					2����������һ��ʱ������ֹرչ��Ų�����λ�������������ݵ����
					3��������ʾ�汾��
/**********************************************/

#include <C8051F410.h>                // SFR declarations
#include <zyy_declare.h>
#include <zyy_define.h>

void PORT_Init (void)
{
   P0MDIN  = 0xFF;                    // ��ť�������� ����ָʾ������ UART0 RS485_COM ��ŷ��źŷ�����������
   P0MDOUT = 0xFF;                    //TXD,RXD=PUSH-PULL

   P1MDIN  = 0xff;                    // ��ŷ� & LEDָʾ��  ����
   P1MDOUT = 0xff;                   

   XBR0    = 0x01;                    // Enable UART on P0.4(TX) and P0.5(RX)
   XBR1    = 0x40;                    // IO��ʹ��
}
void SYSCLK_Init (void)	//49Mʱ����Ϊϵͳʱ��
{
    unsigned char i;
    PCA0MD &= ~0x40; // WDTE = 0 (clear watchdog timer
    OSCICN = 0x80;                         // enable intosc
    CLKSEL = 0x00;                         // select intosc as sysclk source
// INTOSC configure
    OSCICN = 0x83;
// PLL configure
    CLKMUL = 0x00;                         // Reset Clock Multiplier
    CLKMUL &= ~0x03;                       // select INTOSC / 2 as PLL source��49M
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
//	PCA0MD &= ~0x40;                    // �رտ��Ź�
//	
//	//�ڲ�����ѡ�� 24.5MHZ
//	OSCICN = 0x87;						//24.5MHZ����Ƶ ʹ���ڲ�����
//	CLKSEL = 0;								//ѡ���ڲ����� SYSCLK����Ƶ
//	while((OSCICN & 0x40) == 0);		//�ȴ��ڲ�ʱ�ӹ���״̬����
	SYSCLK_Init ();
	PORT_Init();                    // Initialize Port I/O
	//�ر�����LOCK
	disable_lock(0xff);

	Timer0_Init();					    //��ʱ��0��ʼ��
	Timer2_Init();							//��ʱ��2��ʼ��
	UART0_Init();								//UART��ʼ��
	EA = 1;											//�����ж�
	Worker_Count = Timer0Value + 20;	//����ָʾ����˸ʱ���ʼ�� 500ms
	UART0.MasterBusy = 0;		//�����������æ��ʶ

	//�ر�����LED
	disable_led(0xFF);
	
	//������ָʾ��
	ShelfLed_Enable();	
	
	variable_init();					//��ʼ������
	//ʹ��485���ڽ���״̬
    RS485_REN();						
//	UART0.TranDataRec = 0;

/*test cmd
LED ENABLE 
	@W10001087		  1��LED
	40 57 31 30 30 30 31 30 38 37 0d 09

	@W1001B087		  27��LED
	40 57 31 30 30 31 42 30 37 35 0d 09
LED DISABLE ALL
	@W100FF05C		  Ϩ������LED
	40 57 31 30 30 46 46 30 35 43 0d 09
*/
   while (1)
   {
		Communication_Respone();		//ͨ�ż��
		lock_control();							//����Ƿ�����Ҫ�򿪵ĵ�ŷ�
		Check_lock_500ms();					//��ŷ���500msʱ����
		Led_Check();								//ʹ��LED���
		State_input();							//LOCK && BUTTON ״̬���

		if (Timer0Value == Worker_Count)	//����ָʾ����˸
		{
			Worker_Count = Timer0Value + 20;
			WORK_LED = ~WORK_LED;
		}
   }                                   
}                                      



