#include "EC03.h"
#include "usart.h"
#include "Delay.h"

#define PRODECT_ID "540251"
#define DEVICE_ID "999664014"
#define PASSWORD "2002123"


//�����жϽ���ATָ���ģ����Ӧ
unsigned short U2_RX_CntPre = 0;
extern u16  U2_RX_Cnt;

void EC4Ginit(void)
{
	EC_clear();
	USART_SendData(USART3, 0x33);
	//whileһ��Ҫ�����������������ϸ�����c�����﷨
	while(EC_SendCmd("+++","+ERR=-1",200))
	{
		Delay_ms(100);
	}
	USART_SendData(USART3, 0x33);
	while(EC_SendCmd("AT+VER\r\n","OK",200))
	{
		Delay_ms(100);
	}
	while(EC_SendCmd("AT+IMEI\r","860602062524923",200))
	{
		Delay_ms(500);
	}
	while(EC_SendCmd("AT+SOCK=TCPC,183.230.40.39,6002\r","\r\n+OK\r\n",200))
	{
		Delay_ms(500);
	}
	while(EC_SendCmd("AT+HEARTINFO=0,0xC00x00\r","OK",200))
	{
		Delay_ms(500);
	}
	while(EC_SendCmd("AT+HEARTM=60\r","OK",200))
	{
		Delay_ms(500);
	}
	while(EC_SendCmd("AT+EXAT\r\n","OK",200))
	{
		Delay_ms(500);
	}

	
}	



void ONENET_MQTT_MQTTPUBLISH(void)
{
	u8 i=0;
	u8 Publish[49]={0x30,0x2F,
		0x00,0x03,0x24,0x64,0x70,0x03,
		0x00,0x27,0x7B,0x22,0x74,0x65,
		0x6D,0x70,0x65,0x72,0x61,0x74,
		0x75,0x72,0x65,0x22,0x3A,0x32,
		0x32,0x2E,0x35,0x2C,0x22,0x68,
	0x75,0x6D,0x69,0x64,0x69,0x74,0x79,
	0x22,0x3A,0x22,0x39,0x35,0x2E,0x32,0x25,0x22,0x7D};
	for(i=0;i<49;i++)
	{
	
		USART_SentByte(Publish[i]);
		//Delay_ms(10);
	}
	for(i=0;i<49;i++)
	{
		USART_SendData(USART3,Publish[i]);
		Delay_ms(50);
	}




}
/*************************************************************
MQTT��һ���֡����̶���ͷ
	������ڣ���������������Ϣ�Լ����������Լ����ĵȼ�
	�ֽ�1;����λ����λ������λMQTT��������
	�ֽ�2���������ĵĳ���
	0x10  0x??
	*/
	/*MQTT�ڶ����֡����ɱ䱨ͷ
	Э������protocol name�����ֽ�1���ֽ�6Ϊ���ȡ����ȡ�M,Q,T,T 0x00 0x04 0x4D 0x51 0x54 0x54
	Э�鼶��protocol level����һ�ֽ� 0x04
	���ӱ�־��һ�ֽ� 0xC2
	�������ӣ����ֽ� ��100�� 0x00 0x64
	
	
	*/
	//MQTT�������֡�����Ч�غ�
	/*
	�豸ID��00 ���� 16����ID
	��ƷID��00 ���� 16����ID
	��Ȩ��Կ��00 ���� 16������Կ
	
	********************************************************************/

void ONENET_MQTT_CONNECT_4G(void)
{

	u8 j=0;
	u8 i=11;
	char* p[3]={0};
	p[0]=DEVICE_ID;
	p[1]=PRODECT_ID;
	p[2]=PASSWORD;
	u8 connect[40]={0x10,0x26,//������Ϣ�Լ����������Լ����ĵȼ�
					0x00,0x04,0x4D,0x51,0x54,0x54,//Э����
					0x04,//Э�鼶��
					0xC2,//���ӱ�־
					0x00,0x64,//�������ӣ����ֽ� ��100�� 0x00 0x64
					0x00,0x09,0x39,0x39,0x39,0x36,0x36,0x34,0x30,0x31,0x34,
					0x00,0x06,0x35,0x34,0x30,0x32,0x35,0x31,
					0x00,0x07,0x32,0x30,0x30,0x32,0x31,0x32,0x33};
	
	for(j=0;j<3;j++)
	{
		connect[++i]=0x00;
		connect[++i]=strlen(p[j]);
		while(*p[j])
		{
			connect[++i]=*p[j];
			p[j]++;
		}
	
	}
	i=0;
	for(i=0;i<40;i++)
	{
	
		USART_SentByte(connect[i]);
		//Delay_ms(10);
	}
	
	//ע�⣬�ⲿ���ڷ�������MQTT��֮������Ҫע����λ����������20����֮ǰ
	for(i=0;i<40;i++)
	{
		USART_SendData(USART3,connect[i]);
		Delay_ms(50);
	}
}

_Bool EC_SendCmd(char *cmd, char *res, u16 time)
{
	UART2_SendString(cmd);
	while(time--)
	{	
		if(EC_WaitRecive() == Rec_ok)							//����յ�����
		{	
			if(strstr((const char *)U2_RX_BUF, (char *)res) != NULL)		//����������ؼ���
			{
				EC_clear();									//��ջ���
				return 0;
			}
		}
		Delay_ms(10);
	}
	
	return 1;
	
}

_Bool EC_WaitRecive(void)
{
	if(U2_RX_Cnt == 0) 							//������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
		return Rec_wait;
		
	if(U2_RX_Cnt == U2_RX_CntPre)				//�����һ�ε�ֵ�������ͬ����˵���������
	{
		U2_RX_Cnt = 0;							//��0���ռ���
			
		return Rec_ok;								//���ؽ�����ɱ�־
	}
		
	U2_RX_CntPre = U2_RX_Cnt;					//��Ϊ��ͬ
	
	return Rec_wait;								//���ؽ���δ��ɱ�־
}

void EC_clear(void)
{
	memset(U2_RX_BUF,0,sizeof(U2_RX_BUF));
	U2_RX_Cnt = 0;
}


