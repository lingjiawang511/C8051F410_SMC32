#ifndef ZYY_DEFINE_H
#define ZYY_DEFINE_H
//HEXתASCII 0-F
unsigned char  code AsciiCode[16] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46};
//ASCIIתHEX 0x30-0x46
unsigned char  code HexCode[71] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
								   0x08,0x09,0x00,0x00,0x00,0x00,0x00,0x00,
								   0x00,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
		 
//UART0 �������ݽṹ����
UART volatile idata UART0;

//��ŷ�״̬��¼�Ĵ���	 ��ʼ״̬Ĭ�Ϲ���ȫ��Ϊ�ر�״̬  
//���˳�� ����0->4 ��λ->��λ 1��->40��  eg: LOCK_STATE[0]-bit7->bit0=1#-8# , LOCK_STATE[1]-bit7->bit0=9#-16#.....
volatile uchar LOCK_STATE[5];

//LED��˸���üĴ���
volatile uchar idata LED_BLINK[16];

//LOCK && BUTTON���
volatile uchar Anti_Shake_Flag;	//������������ʶ
volatile uchar Anti_Shake_Timer;	//������������

//���ʹ������
volatile uchar State_Trans_Cnt = 0;	//���ʹ�������ʶ
									 
//��ǰ�����״̬��������
volatile uchar Return_Lock_State_Cnt = 0; 	 //��⵱ǰ�������������
volatile uchar Return_Lock_State;		 		 //Ҫ�����ĵ�ŷ�״̬
volatile uchar Return_Lock_ID=0;		 		 //��ǰ��ŷ��ı��
volatile uchar Return_Lock_Flag=0;		 	 //��⵱ǰ�����״̬������ʶ
									 
volatile uchar LockAddr[5]; 			//��ŷ���500ms�������  �洢�򿪵ĵ�ŷ����1-40
volatile uchar Lock_500ms[5];			//��ŷ���500ms��ʱ����  �洢�򿪵�ʱ��ֵ��timer0value)
volatile uchar lock_1_8;					//��ŷ�1-8��74HC595�ĵ�ǰֵ
volatile uchar lock_9_16;					//��ŷ�9-16��74HC595�ĵ�ǰֵ
volatile uchar lock_17_24;				//��ŷ�17-24��74HC595�ĵ�ǰֵ
volatile uchar lock_25_32;				//��ŷ�25-32��74HC595�ĵ�ǰֵ
volatile uchar lock_33_40;				//��ŷ�33-40��74HC595�ĵ�ǰֵ
volatile uchar door_time_value[8];	//�����ŵ�3S��ʱ�䶨ʱֵ ������1�Ŷ�ӦLOCK40 8�Ŷ�ӦLock_33

//volatile uchar LedAddr[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 		//LED�򿪱������  �洢�򿪵�LED���1-128
volatile uchar led_1_8;					//��ŷ�1-8��74HC595�ĵ�ǰֵ
volatile uchar led_9_16;				//��ŷ�9-16��74HC595�ĵ�ǰֵ
volatile uchar led_17_24;				//��ŷ�17-24��74HC595�ĵ�ǰֵ
volatile uchar led_25_32;				//��ŷ�25-32��74HC595�ĵ�ǰֵ
volatile uchar led_33_40;				//��ŷ�33-40��74HC595�ĵ�ǰֵ
volatile uchar led_41_48;				//��ŷ�41-48��74HC595�ĵ�ǰֵ
volatile uchar led_49_56;				//��ŷ�49-56��74HC595�ĵ�ǰֵ
volatile uchar led_57_64;				//��ŷ�57-64��74HC595�ĵ�ǰֵ
volatile uchar led_65_72;				//��ŷ�65-72��74HC595�ĵ�ǰֵ
volatile uchar led_73_80;				//��ŷ�73-80��74HC595�ĵ�ǰֵ
volatile uchar led_81_88;				//��ŷ�81-88��74HC595�ĵ�ǰֵ
volatile uchar led_89_96;				//��ŷ�89-96��74HC595�ĵ�ǰֵ
volatile uchar led_97_104;				//��ŷ�97-104��74HC595�ĵ�ǰֵ
volatile uchar led_105_112;				//��ŷ�105-112��74HC595�ĵ�ǰֵ
volatile uchar led_113_120;				//��ŷ�113-120��74HC595�ĵ�ǰֵ
volatile uchar led_121_128;				//��ŷ�121-128��74HC595�ĵ�ǰֵ

volatile uchar Timer0Value;			   //TIMER0��ʱ����ļ������� ��ʱ��0�жϵĴ���  ��ʱ��0��ʱʱ�� = 25ms
volatile uchar Timer2Value;			   //TIMER2��ʱ����ļ������� ��ʱ��2�ն˵Ĵ���  ��ʱ��2��ʱʱ�� = 1.5ms
volatile uchar Led_Blink_TimeValue;		//LED��˸ʱ������
volatile uchar Led_Blink_State;			//LED��˸״̬

volatile uchar Worker_Count;		   //����ָʾ�Ƽ����� 500ms��˸
#endif