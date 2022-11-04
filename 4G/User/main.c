#include "stm32f10x.h"                  // Device header

#include "usart.h"
#include "EC03.h"
#include "Delay.h"
#include "heart.h"

u8 heart=0;

int main()
{
//	u8 i=0;
	USART3_Init(115200);
	USART2_Init(115200);
	Timer_Init4(65530,65530);
	
	Delay_s(20);
	//请留给硬件反应时间
	EC4Ginit();
	
	Delay_s(20);
	
    ONENET_MQTT_CONNECT_4G();
	
	Delay_s(10);
//	ONENET_MQTT_MQTTPUBLISH();
	
	while(1)
	{
			Delay_s(40);
		    USART_SentByte(0xC0);
			USART_SentByte(0x00);
		
		
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	while(*str)
//	{
//		p[i]=*str;
//		USART_SendData(USART1 ,p[i]);
//		//连续发送注意需要延时
//		Delay_ms(100);
//		i++;
//		str++;
//	
//	}
//	u8 i=0;

//	//Delay_s(30);
//	u8 connect[40]={0x10,0x26,0x00,0x04,0x4D,0x51,0x54,0x54,
//		0x04,0xC2,0x00,0x64,0x00,0x09,0x39,0x39,0x39,0x36,
//		0x36,0x34,0x30,0x31,0x34,0x00,0x06,0x35,0x34,0x30,0x32,
//	0x35,0x31,0x00,0x07,0x32,0x30,0x30,0x32,0x31,0x32,0x33};
//	for(i=0;i<40;i++)
//	{
//	
//		USART_SentByte(connect[i]);
//		//Delay_ms(10);
//	}
	
	//防止跑飞，单片机一定要有一个while循环，
	//否则程序执行会出现问题，不同型号单片机会跑向不同位置
	while(1);
}
