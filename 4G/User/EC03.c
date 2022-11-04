#include "EC03.h"
#include "usart.h"
#include "Delay.h"

#define PRODECT_ID "540251"
#define DEVICE_ID "999664014"
#define PASSWORD "2002123"


//用来判断接受AT指令的模块响应
unsigned short U2_RX_CntPre = 0;
extern u16  U2_RX_Cnt;

void EC4Ginit(void)
{
	EC_clear();
	USART_SendData(USART3, 0x33);
	//while一定要跟个大括号声明，严格遵守c语言语法
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
MQTT第一部分——固定报头
	必须存在，用于描述报文信息以及报文类型以及报文等级
	字节1;低四位保留位，高四位MQTT报文类型
	字节2：整个报文的长度
	0x10  0x??
	*/
	/*MQTT第二部分——可变报头
	协议名（protocol name）：字节1至字节6为长度、长度、M,Q,T,T 0x00 0x04 0x4D 0x51 0x54 0x54
	协议级别（protocol level）：一字节 0x04
	连接标志：一字节 0xC2
	保持连接：两字节 如100秒 0x00 0x64
	
	
	*/
	//MQTT第三部分——有效载荷
	/*
	设备ID：00 ？？ 16进制ID
	产品ID：00 ？？ 16进制ID
	鉴权秘钥：00 ？？ 16进制秘钥
	
	********************************************************************/

void ONENET_MQTT_CONNECT_4G(void)
{

	u8 j=0;
	u8 i=11;
	char* p[3]={0};
	p[0]=DEVICE_ID;
	p[1]=PRODECT_ID;
	p[2]=PASSWORD;
	u8 connect[40]={0x10,0x26,//报文信息以及报文类型以及报文等级
					0x00,0x04,0x4D,0x51,0x54,0x54,//协议名
					0x04,//协议级别
					0xC2,//连接标志
					0x00,0x64,//保持连接：两字节 如100秒 0x00 0x64
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
	
	//注意，这部分在发送连接MQTT包之后，所以要注意上位机串口助手20在这之前
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
		if(EC_WaitRecive() == Rec_ok)							//如果收到数据
		{	
			if(strstr((const char *)U2_RX_BUF, (char *)res) != NULL)		//如果检索到关键词
			{
				EC_clear();									//清空缓存
				return 0;
			}
		}
		Delay_ms(10);
	}
	
	return 1;
	
}

_Bool EC_WaitRecive(void)
{
	if(U2_RX_Cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return Rec_wait;
		
	if(U2_RX_Cnt == U2_RX_CntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		U2_RX_Cnt = 0;							//清0接收计数
			
		return Rec_ok;								//返回接收完成标志
	}
		
	U2_RX_CntPre = U2_RX_Cnt;					//置为相同
	
	return Rec_wait;								//返回接收未完成标志
}

void EC_clear(void)
{
	memset(U2_RX_BUF,0,sizeof(U2_RX_BUF));
	U2_RX_Cnt = 0;
}


