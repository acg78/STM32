//main程序

#include "stdio.h"	
#include "stm32f10x.h"
#include "dht11.h"
#include "Delay.h"
#include "OLED.h"
#include "Timer.h"
#include "PWM.h"
#include "USART.h"
#include "Key.h"
#include "power.h"
#include "heart_uart.h"


#include "uart4g.h"
#include "EC03.h"


u8 heart=0;
u8 EC02_4G[10];
u8 RxData;
extern uint8_t dat[5];//extern，电脑会自动寻找变量位置
u8 JU;
u8 d=0;
extern u8 KeyNum;
extern uint8_t S_TxPack[3];
extern uint8_t S_RxPack[3];

enum Fun
{
	dht11,
	game
};


void MainInit(void);
void MENU(void);
void Dht11(void);
void Game(void);
void Buffer(void);

void MainInit(void)
{
	USART3_Init(115200);
	USART2_Init(115200);
	Timer_Init(2500,7200);
	Key_Init();
	USART_Config(9600);
	OLED_Init();//OLED初始化
}
//一级菜单，即主界面，包括跳转功能
void MENU(void)
{
	u8 j=1;
	OLED_ShowLogo(0,0,3);
	OLED_ShowLogo(0,20,4);
	OLED_ShowLogo(0,110,5);
	OLED_ShowLogo(0,40,6);
	OLED_ShowDinosaur(1,9);
	OLED_ShowLogo(0,90,6);
	//箭头
	OLED_ShowLogo(3,20,7);
	
	OLED_ShowLogo(3,36,0);
	OLED_ShowLogo(3,52,2);
	OLED_ShowLogo(3,68,1);
	//GAMe
	
	OLED_ShowLogo(5,36,8);
	OLED_ShowLogo(5,52,9);
	while(1)
	{
		if( Key_GetNum()==1&&(j%2)==1)
		{
			OLED_ShowLogo(3,20,10);
			OLED_ShowLogo(5,20,7);
			j++;
			KeyNum=0;
		}
		else if(Key_GetNum()==1&&(j%2)==0)
		{
			OLED_ShowLogo(5,20,10);
			OLED_ShowLogo(3,20,7);
			j++;
			KeyNum=0;
		}
		if(Key_GetConfirm()==1&&(j%2)==1)
		{
			OLED_Clear();
			Dht11();
		}
		if(Key_GetConfirm()==1&&(j%2)==0)
		{
			OLED_Clear();
			Game();
		}
	}
}	

void Dht11(void)
{
	u32 i=20000;
	//MENU部分，也是主机代码
		OLED_ShowLogo(0,32,0);
		OLED_ShowLogo(0,48,1);
	
		OLED_ShowChar(2,1,'1');
		OLED_ShowChar(2,2,'.');
	
		OLED_ShowChar(3,1,'2');
		OLED_ShowChar(3,2,'.');
	
		OLED_ShowLogo(0,80,2);
		OLED_ShowLogo(0,96,1);
	
		OLED_ShowChar(4,1,'3');
		OLED_ShowChar(4,2,'.');
	//
	//因为采用的既有套装lora，又有贴片，无法通过程序改变工作模式，因而无法实现最简单的多对一
	//轮询，因而采取双向通信的方式，即主机发送一串数据，收到不同应答从而确定是哪一个
	//传感器发送的数据，而主机也根据收到的数据来确定是否进入工作模式
	while(1)
	{
		S_TxPack[0]=0x10;
		S_TxPack[1]=0x10;
		S_TxPack[2]=0x10;
		S_SendPack();
		while(i--)
		{
			if(S_GetRxFlag()==1)
			{
				if(S_RxPack[0]==0x10)
				{
					OLED_ShowNum(2,10,S_RxPack[1],2);
					OLED_ShowNum(2,5,S_RxPack[2],2);
					ONENET_MQTT_MQTTPUBLISH(S_RxPack[1],S_RxPack[2]);
				}
				break;
			}
		}
		if(i==0)
		{
			OLED_ShowLogo(1,32,0);
		}
		i=2000000;
		S_TxPack[0]=0x20;
		S_TxPack[1]=0x20;
		S_TxPack[2]=0x20;
		S_SendPack();
		while(i--)
		{
			if(S_GetRxFlag()==1)
			{
				if(S_RxPack[0]==0x20)
				{
					OLED_ShowNum(3,10,S_RxPack[1],2);
					OLED_ShowNum(3,5,S_RxPack[2],2);
					ONENET_MQTT_MQTTPUBLISH(S_RxPack[1],S_RxPack[2]);
				}
				break;
			}
		}
		if(i==0)
		{
			OLED_ShowLogo(1,32,0);
		}
	    i=2000000;
		S_TxPack[0]=0x30;
		S_TxPack[1]=0x30;
		S_TxPack[2]=0x30;
		S_SendPack();
		while(i--)
		{
			if(S_GetRxFlag()==1)
			{
				if(S_RxPack[0]==0x30)
				{
					OLED_ShowNum(4,10,S_RxPack[1],2);
					OLED_ShowNum(4,5,S_RxPack[2],2);
					ONENET_MQTT_MQTTPUBLISH(S_RxPack[1],S_RxPack[2]);
				}
				break;
			}
		}
		
		if(i==0)
		{
			OLED_ShowLogo(1,32,0);
		}
		
		Delay_s(20);
		i=2000000;
	}
}

void Game(void)
{
	while(1)
	{
		OLED_Clound();
		OLED_Ground();
		if( Key_GetNum()==1&&d<16)
		{
			
			OLED_ShowDinosaurJump(0);
			d++;
			
		}
		
		else
		{
			
			OLED_ShowDinosaur(4,1);
			d=0;
			KeyNum=0;
			
		}
		
		Delay_ms(40);
	}
	
	
}

/**
  * @brief  电机驱动
  */
//int main(void)
//{
//  
//	Power();
//	Motor();
//	PWM3_SetCom(500);

//}
/****************************************************************
******            此部分为轮询方案                        *******
******                                                    *******
******                                                    *******
******                                                    *******
******************************************************************/

/*
第一个从机——实际信道为0x20，虚拟地址为0x10
*/
//int main()
//{
//	u8 chu[4]={0xC0,0x05,0x01,0x20};
//	USART_Config(9600);
//	Lora_init();
//	Lora_Mode(0x02);
//	Delay_s(1);
//	USART_SentArr(chu,4);//usart发送命令
//	Delay_s(1);
//	Lora_Mode(0x00);
//	
//	while(1)
//	{
//		if(S_GetRxFlag()==1&&S_RxPack[0]==0x10)
//		{
//			DHT_Read();
//			S_TxPack[0]=0x10;
//			S_TxPack[1]=dat[0];
//			S_TxPack[2]=dat[2];
//			S_SendPack();
//		}
//		
//	}
//}


/*
第二个从机——实际信道为0x20，虚拟地址为0x20
贴片从机标号为1，注意M1，M0，A5,
已调试完成
*/
//int main()
//{
//	u8 chu[4]={0xC0,0x05,0x01,0x20};
//	USART_Config(9600);
//	Lora_init();
//	Lora_Mode(0x02);
//	Delay_s(1);
//	USART_SentArr(chu,4);//usart发送命令
//	Delay_s(1);
//	Lora_Mode(0x00);
//	
//	while(1)
//	{
//		if(S_GetRxFlag()==1&&S_RxPack[0]==0x20)
//		{
//			DHT_Read();
//			S_TxPack[0]=0x20;
//			S_TxPack[1]=dat[0];
//			S_TxPack[2]=dat[2];
//			S_SendPack();
//		}
//	}
//}

/*此处为 从机三
以调试成功，为TBL套件
*/

//int main()
//{
//	USART_Config(9600);
//	while(1)
//	{
//		if(S_GetRxFlag()==1&&S_RxPack[0]==0x30)
//		{
//			DHT_Read();
//			S_TxPack[0]=0x30;
//			S_TxPack[1]=dat[0];
//			S_TxPack[2]=dat[2];
//			S_SendPack();
//		}
//	}
//}




int main(void)
{	
	MainInit();
	
	u8 i=0;
    //Buffer();//开机音乐
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	//Delay_s(20);
	//请留给硬件反应时间
	EC4Ginit();
	
	Delay_s(20);
	
    ONENET_MQTT_CONNECT_4G();
	
	Delay_ms(50);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//	
	MENU();
	
	
//	while(1)
//	{
//	   ONENET_MQTT_MQTTPUBLISH(34+i,35+i);
//		i++;
//		if(i==10)
//		{
//			i=0;
//		}
//		Delay_s(5);
//	}
	
	while(1)
	{
	
	}
/*
	while(1)
	{
		OLED_ShowChinese(1,1,2);
		OLED_ShowChinese(1,3,1);
		OLED_ShowChar(1,5,':');
		OLED_ShowChinese(2,1,0);
		OLED_ShowChinese(2,3,1);
		OLED_ShowChar(2,5,':');
		
		OLED_Clound();
		OLED_Ground();
		if( Key_GetNum()==1&&d<16)
		{
			
			OLED_ShowDinosaurJump(0);
			d++;
			
		}
		
		else
		{
			
			OLED_ShowDinosaur(4,1);
			d=0;
			KeyNum=0;
			
		}
		
		Delay_ms(40);
	
//		
//		if(DHT_Read())
//		{
//			USART_SentByte(dat[0]);
//			Delay_ms(50);
//			USART_SentByte(dat[2]);
//		}
		if(S_GetRxFlag()==1)
		{
			
			RxData=S_GetRxData();
			if(RxData>40)
			{
				//显示湿度
			
				OLED_ShowNum(1,8,RxData,2);
			}
			else
			{
				//显示温度
				OLED_ShowNum(2,8,RxData,2);	
			}
			
		}
	}
*/
	
	
	
		
	
//	uint16_t a[7]={38,34,30,28,25,22,20};
//	uint16_t i=0;
//    vu8 key=0;	  
//   while(1)
//  {								 					 
//      Timer_Init(a[i],7200);	 //不同的频率放入数组，输出不同的声音
//	  PWM_Init(a[i]/2);
//	  i++;
//	  if(i==7)
//	 {
//		i=0;
//	 }		
//	  Delay_ms(500);
//  }	 
/*
	


	//OLED_ShowPicture128x64();
	//USART_Config();
	while(1)
	{
//		if(S_GetRxFlag()==1)
//		{
//			RxData=S_GetRxData();
//			USART_SentByte(RxData);
//			//OLED_ShowHexNum(1,8,RxData,2);
//		}
		
		OLED_Clound();
		OLED_Ground();
		if( Key_GetNum()==1&&d<16)
		{
			
			OLED_ShowDinosaurJump(0);
			d++;
			
		}
		
		else
		{
			
			OLED_ShowDinosaur(4,1);
			d=0;
			KeyNum=0;
			
		}
		
		Delay_ms(40);
	}
	*/
	/*
	简易温湿度读取
	*/
	


/*
简易温湿度读取

int mian(void)
{
	while(1)
	{
		if(DHT_Read())//如果校验和不对，此次读取数据将会丢弃
			{
			OLED_ShowString(2,  3, "HUM:");
			
			OLED_ShowNum(2, 7, dat[0], 2);
			//printf("：%d%%,温度：%d度\r\n",dat[0],dat[2]);
			OLED_ShowNum(3, 8, dat[2], 2);
			OLED_ShowString(3,  3, "TEMP:");
			OLED_ShowString(3,  10, "C");
			//OLED_ShowString(4,  3, "NUM:");
			//OLED_ShowNum(4,7,Num,4);
			if(dat[0]>=70||dat[2]>=35)
						Timer_Init(38,7200);	 //不同的频率放入数组，输出不同的声音
						PWM_Init(38/2);
			if(dat[0]<70&&dat[2]<35)
				PWM_Init(38);//冻结模式
          //printf("湿度：%d.%d%%,温度：%d.%d度\r\n",dat[0],dat[1],dat[2],dat[3]);  显示小数
		    Delay_ms(30);
	        }
	}
}
*/

}





	

