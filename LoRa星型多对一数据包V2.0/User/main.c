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

void Buffer(void)
{
	u8 i=0;
	u8 j=0;
	int a[32]={30,30,30,0,30,30,30,0,30,28,
		         25,30,28,25,25,22,25,28,30,38,
	           25,22,25,28,30,38,38,51,38,38,
	           51,38};//创建数组，f=72MHZ/[(arr+1)(psc+1)]求出的arr
	int b[32]={250,250,250,250,250,250,250,250,500,500,
		         1000,500,500,1000,250,250,250,250,500,500,
	           250,250,250,250,500,500,500,500,1000,500,};
	  
	while(1)
	{								 					 
		PWM_Init(a[i],7200,a[i]/2);
		i++;
		j++;
		if(i==32&&j==32)
			{
				i=0,j=0;

			}		
		Delay_ms(b[j]);
	}	

}
void MainInit(void)
{
	
	Timer_Init(2500,7200);
	Key_Init();
	USART_Config(115200);
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
		S_TxPack[0]=0x01;
		S_TxPack[1]=0x01;
		S_TxPack[2]=0x01;
		S_SendPack();
		while(1)
		{
			if(S_GetRxFlag()==1)
			{
				if(S_RxPack[0]==0x01)
				{
					OLED_ShowNum(2,10,S_RxPack[1],2);
					OLED_ShowNum(2,5,S_RxPack[2],2);
				}
				break;
			}
		}
		
		
		
		S_TxPack[0]=0x02;
		S_TxPack[1]=0x02;
		S_TxPack[2]=0x02;
		S_SendPack();
		
		while(1)
		{
			if(S_GetRxFlag()==1)
			{
				if(S_RxPack[0]==0x02)
				{
					OLED_ShowNum(3,10,S_RxPack[1],2);
					OLED_ShowNum(3,5,S_RxPack[2],2);
				}
				break;
			}
		}
//		



//		if(S_GetRxFlag()==1)
//		{
//			RxData=S_GetRxData();
//			if(RxData>40)
//			{
//				//显示湿度
//				OLED_ShowNum(2,5,RxData,2);
//			}
//			else
//			{
//				//显示温度
//				OLED_ShowNum(2,10,RxData,2);	
//			}
//		}
		
		//主机自带的温湿度传感器、
		if(DHT_Read())
		{
			OLED_ShowNum(4, 5, dat[2], 2);
			OLED_ShowNum(4, 10, dat[0], 2);
		}
		Delay_ms(1000);
	
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
	
	


int main(void)
{	
//	u8 chu[12]={0xC0,0x00,0x09,0x00,0x00,0x00,0xe2,0x00,0x14,0x03,0x00,0x00};
//	
//	USART_Config(9600);
//	
//	Lora_init();
//	
//	
//	Lora_Mode(0x02);
//	USART_SentArr(chu,12);//usart发送命令
//	Lora_Mode(0x00);
//	//USART_Config(115200);
//	
//	while(1)
//	{
//		//USART_SentByte(0x01);
//		if(S_GetRxFlag()==1&&S_RxPack[0]==0x02)
//		{
//			DHT_Read();
//			S_TxPack[0]=0x02;
//			S_TxPack[1]=dat[0];
//			S_TxPack[2]=dat[2];
//			S_SendPack();
//		}
//		
//	}




//	int j;
//	u8 chu[12]={0xC0,0x00,0x09,0x00,0x00,0x00,0xe2,0x00,0x17,0x03,0x00,0x00};
//	//lora参数命令

//	USART_Config(9600);//USART
//	
//	Lora_init();//初始化M1   M0   AUX
//	
//	Lora_Mode(0x02);//进入配置模式
//	
//	USART_SentArr(chu);//usart发送命令
//	
//	Lora_Mode(0x00);

//	while(1)
//	{
//		USART_SentByte(0x43);
//	}
//	//USART_Config(115200);
//	if(S_GetRxFlag()==1)
//	{
//		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//		j=S_GetRxData();
//	}
//	while(1)
//	{
//		if(DHT_Read())
//		{
//			//GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//			Delay_ms(100);
//			USART_SentByte(dat[0]);
//			Delay_ms(50);
//			USART_SentByte(dat[2]);
//		}
//	}

	Power();
	Motor();
	PWM3_SetCom(500);
	
//	Buffer();
//	MainInit();
//	MENU();
//	enum Fun seclect;

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
	/*while (1)
	{
	if(DHT_Read())
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

	}*/
	
}


	

