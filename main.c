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


u8 RxData;
extern uint8_t dat[5];//extern，电脑会自动寻找变量位置
u8 JU;
u8 d=0;
extern u8 KeyNum;


/*
enum Fun
{
	dht11,
	game
};
void MainInit(void);
void MENU(void);
void Dht11(void);
void Game(void);
void MainInit(void)
{
	Timer_Init(2500,7200);
	Key_Init();
	//USART_Config();
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
	//MENU部分
		OLED_ShowLogo(0,32,2);
		OLED_ShowLogo(0,48,1);
	
		OLED_ShowChar(2,1,'1');
		OLED_ShowChar(2,2,'.');
	
		OLED_ShowChar(3,1,'2');
		OLED_ShowChar(3,2,'.');
	
		OLED_ShowLogo(0,80,0);
		OLED_ShowLogo(0,96,1);
	
		OLED_ShowChar(4,1,'3');
		OLED_ShowChar(4,2,'.');
	//读取两个温湿度
	while(1)
	{
		
		if(S_GetRxFlag()==1)
		{
			RxData=S_GetRxData();
			if(RxData>40)
			{
				//显示湿度
				OLED_ShowNum(2,5,RxData,2);
			}
			else
			{
				//显示温度
				OLED_ShowNum(2,10,RxData,2);	
			}
		}
		if(DHT_Read())
		{
			OLED_ShowNum(3, 5, dat[0], 2);
			OLED_ShowNum(3, 10, dat[2], 2);
		}
	
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
	
	
*/
int main(void)
{	
	//u8 chu[12]={0xC0,0x00,0x09,0x00,0x00,0x00,0xe2,0x00,0x17,0x03,0x00,0x00};
	
	u8 chu[12]={0xC1,0x00,0x01};
	
	USART_Config(9600);
	
	Lora_init();
	
	Lora_Mode(0x02);//进入配置模式

	USART_SentArr(chu);
	
	if(S_GetRxFlag()==1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
	//USART_Config(115200);

	Lora_Mode(0x00);
	while(1)
	{
		
		if(DHT_Read())
		{
			
			//GPIO_ResetBits(GPIOC,GPIO_Pin_13);
			Delay_ms(100);
			USART_SentByte(dat[0]);
			Delay_ms(50);
			USART_SentByte(dat[2]);
		}
	}


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


	

