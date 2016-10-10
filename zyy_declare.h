#ifndef ZYY_DECLARE_H
#define ZYY_DECLARE_H

#define uint unsigned int
#define uchar unsigned char

#define ENABLE 1
#define DISABLE 0

//ASCII ת��������
extern unsigned char code AsciiCode[];			//HEXתASCII
extern unsigned char code HexCode[];				//ASCIIתHEX


//RS485ͨ��IO����  ͨѶģ�飺RSM485ECHT
sbit RS485_COM = P0^6;					
#define  RS485_REN()	RS485_COM = 1					//���գ�COM = 1
#define  RS485_TEN()	RS485_COM = 0					//���ͣ�COM = 0

//UART0 ���������ȶ���
#define UART0_Rxd_Length 10
#define UART0_Txd_Length 10

//UART0 ͨ��Э�����ݶ���
#define Slave_ID	HexCode[UART0.RevBuf[2]]	     //ͨ��Э�� �ӻ���ַ = ����ͨѶ���յĵ�3���ֽ�
#define Lock_ID		( (HexCode[UART0.RevBuf[3]] << 4) | HexCode[UART0.RevBuf[4]] )	   //ͨ��Э�� ��������� =  ����ͨѶ���յĵ�4�ֽڡ���5�ֽ�ת����16����
#define Lock_State	HexCode[UART0.RevBuf[5]]		 //�����״̬ ����ָ���6���ֽ�
#define Led_ID		( (HexCode[UART0.RevBuf[3]] << 4) | HexCode[UART0.RevBuf[4]] )	   //ͨ��Э�� LED��� = ����ͨѶ���յĵ�4�ֽڡ���5�ֽ�ת����16����
#define Led_State	HexCode[UART0.RevBuf[5]]	     //ͨ��Э�� LED����״̬ 0=����˸ 1=��˸
#define LRC_H		UART0.RevBuf[UART0.RxdCnt-3]	   //ͨ��Э�� У������ֽ�
#define LRC_L		UART0.RevBuf[UART0.RxdCnt-2]	   //ͨ��Э�� У������ֽ�

//UARTͨ�����ݽṹ--ʹ�õı���
typedef struct 
{
	uchar RevBuf[10];			//���ڽ��ܻ�����
	uchar TraBuf[10];			//���ڷ��ͻ�����
	uchar LockState[5];		//��ŷ�״̬���� �洢�ı�״̬�ĵ�ŷ���� bit7λ��״̬˵�� 1=�� 0=�ر� bit6-bit0=��ŷ����
	uchar ButtonState[5];	//����״̬���� �洢�����µİ������
	uchar RxdCnt;					//���ڽ����ֽڸ���������
	uchar TxdCnt;					//���ڷ����ֽڸ���������
	uchar *TranData;			//�������ݷ���ָ��
	uchar RevFlag;				//ͨѶ��Ӧ ��Ϊ�ӻ���ͨ�Ŵ����ʶ
	uchar LOCK_ID;				//ͨ��Ҫ��򿪵ĵ�ŷ����
	uchar LED_ID;					//ͨ��Ҫ�������LED���
	uchar LED_STATE;			//ͨ���趨LED��״̬ 0=����˸ 1=��˸
	uchar LED_Blink_Cnt;	//LED��˸״̬��ʱ��
	uchar LED_Blink_state;	//LED��ǰ��˸״̬ 0=�� 1=��
//	uchar LOCK_State_Num;	//����ģʽ ���͸� �˻�����ĸı���״̬�Ĺ��ŵı��
//	uchar LOCK_State;		//����ģʽ ���͸� �˻�����Ĺ��ŵ�״̬ 1=�ر� 0=��
	uchar Master_Tran_Timer;//��������ģʽ��ʱ��  ���ڼ��ͨ��ռ�ú����·��Ͷ�ʱ
	uchar MasterFlag;		//�������ͱ�ʶ  1=��Ҫ���� 0=��
	uchar MasterBusy;			//��������æ��ʶ ���� 0=���������� 1=����������

}UART;
extern UART volatile idata UART0;

//LOCK && BUTTON���
extern volatile uchar Anti_Shake_Flag;	//������������ʶ
extern volatile uchar Anti_Shake_Timer;	//������������

//���ʹ������
extern volatile uchar State_Trans_Cnt;	//���ʹ�������ʶ

//��ǰ�����״̬��������
extern volatile uchar Return_Lock_State_Cnt;  //��⵱ǰ�����״̬��������
extern volatile uchar Return_Lock_State;			//��⵱ǰ�����״̬
extern volatile uchar Return_Lock_ID;		 	
extern volatile uchar Return_Lock_Flag;		 		//��⵱ǰ�����״̬������ʶ

//����ָʾ��IO�ڶ���
sbit WORK_LED   = P0^2;		 //����ָʾ��
#define WorkLed_Enable()	WORK_LED = 0;
#define WorkLed_Disable()	WORK_LED = 1;

//���ܹ���ָʾ�ƶ���
sbit  SHELF_LED = P1^3;			//���ܹ���ָʾ��
#define ShelfLed_Enable()		SHELF_LED = 0;
#define ShelfLed_Disable()	SHELF_LED = 1;

//�������붨��
sbit BUTTON = P2^1;
sbit BUTTON_SL = P0^0;	    
sbit BUTTON_CLK = P0^1;
#define Button_SL_RS()		BUTTON_SL = 0; BUTTON_SL = 1;
#define Button_CLK_RS()		BUTTON_CLK = 0; BUTTON_CLK = 1;
//��ŷ������źŶ���
sbit LOCK_FEEDBACK = P2^0;
sbit FEEDBACK_SL = P0^3;
sbit FEEDBACK_CLK = P0^7;
#define Feedback_SL_RS()	FEEDBACK_SL = 0; FEEDBACK_SL = 1;
#define Feedban_CLK_RS()	FEEDBACK_CLK = 0; FEEDBACK_CLK = 1;

//��ŷ���������
sbit LOCK_SCLK = P1^0;
sbit LOCK_LCLK = P1^1;
sbit LOCK_DATA = P1^2;
#define Lock_Sclk_RS()  LOCK_SCLK = 0; LOCK_SCLK = 1
#define Lock_Lclk_RS()  LOCK_LCLK = 0; LOCK_LCLK = 1

//LED��������
sbit LED_SCLK = P1^4;
sbit LED_LCLK = P1^5;
sbit LED_DATA = P1^6;
#define Led_Sclk_RS()  LED_SCLK = 0; LED_SCLK = 1
#define Led_Lclk_RS()  LED_LCLK = 0; LED_LCLK = 1
//LED��˸���üĴ���
extern volatile uchar idata LED_BLINK[16];

//��ŷ�״̬��¼�Ĵ���	 ��ʼ״̬Ĭ�ϵ�ŷ�ȫ��Ϊ�ر�״̬
extern volatile uchar LOCK_STATE[];

//������&LED�� ��������
extern volatile uchar LockAddr[];		
extern volatile uchar Lock_500ms[];
extern volatile uchar door_time_value[];	//�����ŵ�3S��ʱ�䶨ʱֵ ������1�Ŷ�ӦLOCK40 8�Ŷ�ӦLock_33
//extern volatile uchar LedAddr[];
extern volatile uchar Timer0Value;
extern volatile uchar Timer2Value;			   //TIMER2��ʱ����ļ������� ����ͨ�ż�� 1.5ms��ʱ
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


//LED���ñ���
extern volatile uchar Led_Blink_TimeValue;		//LED��˸ʱ������
extern volatile uchar Led_Blink_State;			//LED��˸ʱ������


// �ⲿ�������
extern void lock_control(void);		//��ŷ��򿪼��
//�رյ�ŷ�
extern void disable_lock(uchar lock);
//������ʼ������
extern void variable_init(void);

extern void Timer0_Init(void);

extern void Timer2_Init(void);//1.5ms 16bit��ʱ��	0xf408  

extern void Check_lock_500ms(void);//����ŷ���ʱ��  500ms

extern void UART0_Init(void);//UART ��ʼ��
//����LED
extern void led_enable(uchar led);

//Ϩ��LED
extern void disable_led(uchar led);

//LOCK && BUTTON ״̬���
extern void State_input(void);

//����״̬���� & ����ģʽ����״̬
extern void Lock_Input(void);

//����Ƿ�����Ҫ������LED��
extern void Led_Check(void);

//LRCУ��
extern unsigned int LRC16(unsigned char *Pushdata,unsigned char length);

//ͨ�Ŵ������ ����--410��Ϊ�ӻ�ͨ�ź���Ϊ����ͨ��
extern void Communication_Respone(void);

//enable ��ŷ�
extern void lock_enable(uchar lock);

extern void delay(uchar time);

#endif	

/*-----------ASCII�� ͨ��Э��---------------
����д����  LED_State = 00 ��01 ����02 ��˸			 
0x40(start) + 0x57('W') + SLAVE_ID(Ĭ��01)  + LOCK(H+L)       + LED(H+L)     + LED_State + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
���ֽ�      + д('W')   + �ӻ���ַ          + ��������(H+L) + LED���(H+L) + LED_State + LRC(H+L) + ������1     + ������2
�ӻ��ظ�:
LRCУ����ȷ��ظ� 
0x40(start) + 0x57('W') + SLAVE_ID(Ĭ��01)  + LOCK(H+L)       + LED(H+L) + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
�ϼ� = 8���ֽ�
LRCУ����� �� ��������	
0x40(start) + 0x4F('E') + 0x4B('R') + LRC + 0x0d(END-1) + 0x09(END-2)

���������� 			 
0x40(start) + 0x44('R') + SLAVE_ID(Ĭ��01)  + LOCK(H+L)       + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
���ֽ�      + ��('R')   + �ӻ���ַ          + ��������(H+L) + LRC(H+L) + ������1     + ������2
�ӻ��ظ�:
LRCУ����ȷ��ظ� 
0x40(start) + 0x44('R') + SLAVE_ID(Ĭ��01)  + LOCK(H+L)       + LED(H+L) + LRC(H+L) + 0x0d(END-1) + 0x09(END-2)
�ϼ� = 8���ֽ�
LRCУ����� �� ��������	
0x40(start) + 0x4F('E') + 0x4B('R') + LRC + 0x0d(END-1) + 0x09(END-2)
**************************/