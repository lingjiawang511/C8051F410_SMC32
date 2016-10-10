#include <C8051F410.h>                 // SFR declarations
#include <zyy_declare.h>

void variable_init(void)
{
	uchar i;

	//��ʼ�� LOCK && Button ״̬�Ĵ���
	UART0.LockState[0] = 0;
	UART0.LockState[1] = 0;
	UART0.LockState[2] = 0;
	UART0.LockState[3] = 0;
	UART0.LockState[4] = 0;
	UART0.ButtonState[0] = 0;
	UART0.ButtonState[1] = 0;
	UART0.ButtonState[2] = 0;
	UART0.ButtonState[3] = 0;
	UART0.ButtonState[4] = 0;

	//��ʼ�� LED��˸�Ĵ���
	for(i=0;i<16;i++)
	{
		LED_BLINK[i] = 0;
	}

	//��ʼ�� LOCK״̬�Ĵ��� Ϊ�ر�״̬
	LOCK_STATE[0] = 0;
	LOCK_STATE[1] = 0;
	LOCK_STATE[2] = 0;
	LOCK_STATE[3] = 0;
	LOCK_STATE[4] = 0;
	for(i=0;i<8;i++)
	{
		door_time_value[i] = 0;
	}
	//��ʼ�� ����ͨ��
	UART0.Master_Tran_Timer = 0;
	Timer2Value				= 0;
	UART0.MasterFlag 		= 0;

	//������������ʶ
	Anti_Shake_Flag = 0;	


}
