#include <stm32f10x.h>
#include"Delay.h"

//接收缓冲区容量
#define USART3_RX_BUFF_SIZEMAX 256
#define USART3_TX_BUFF_SIZEMAX 256

//USART设备控制结构体
typedef struct
    {
        u32			BaudRate;				//波特率
        u32			R_Idx;
        u8			RxBuf[USART3_RX_BUFF_SIZEMAX];
        u8			TxBuf[USART3_TX_BUFF_SIZEMAX];
        u8          data_length;
        u8          Get_length;
    } USART3_STRUCT;

USART3_STRUCT USART3_Real={115200,0,0,0,0,0};
/****************************************************************************
* 名	称：void USART3_GPIO_Init(void)
* 功	能：串口引脚初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_GPIO_Init(void)							//串口引脚初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;		//串口引脚结构
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//串口引脚分配时钟
    //配置串口 Tx (PB.10) 为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//串口发送引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	//频率2MHz : 115200足够
    GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化引脚
    // 配置串口 Rx (PB.11) 为浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			//串口接收引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);				//初始化引脚
}

/****************************************************************************
* 名	称：void USART3_Init(void)
* 功	能：串口初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_Init(u32 BaudRate)
{
    USART_InitTypeDef UART_InitStructure;		//串口结构
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //串口分配时钟
    USART3_Real.BaudRate = BaudRate;						//波特率
    //串口初始化
    UART_InitStructure.USART_BaudRate= USART3_Real.BaudRate;//波特率
    UART_InitStructure.USART_WordLength= USART_WordLength_8b;//数据位8bit
    UART_InitStructure.USART_StopBits= USART_StopBits_1;//停止位个数
    UART_InitStructure.USART_Parity = USART_Parity_No ;	//不进行奇偶效验
    UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//RTS和CTS使能(None不使用)
    UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送和接收使能
    USART_Init(USART3, &UART_InitStructure);	//初始化串口															
}

/****************************************************************************
* 名	称：void USART3_NVIC_Init(void)
* 功	能：串口中断向量表初始化
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 			//中断控制器变量
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//设置中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//主优先级设置
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//设置优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//打开串口中断
    NVIC_Init(&NVIC_InitStructure);							//初始化中断向量表
}

/****************************************************************************
* 名	称：void USART3_Config(void)
* 功	能：串口设置
* 入口参数：无
* 出口参数：无
* 说	明：默认为包数据接收
****************************************************************************/
void USART3_Config(void)
{
    USART3_Init(9600);						        //串口初始化
    USART3_GPIO_Init();			                	//串口引脚初始化
    USART3_NVIC_Init();				                //中断初始化

    USART_ClearITPendingBit(USART3, USART_IT_RXNE);	//清接收标志
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//开启接收中断

    USART_Cmd(USART3, ENABLE);  				   //使能串口外设
}

/****************************************************************************
* 名	称：void USART3_SendString(u8* Data, u32 Len)
* 功	能：多字符输出
* 入口参数：Data 	输出的单字符数据
			Len		字符个数
* 出口参数：无
* 说	明：无
****************************************************************************/
void USART3_SendString(u8* Data, u32 Len)
{
    u32 i=0;
    for(i=0; i<Len; i++)
    {
        USART_SendData(USART3, Data[i]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }
    memset(&USART3_Real.TxBuf, 0, USART3_TX_BUFF_SIZEMAX);
}


/****************************************************************************
* 名	称：void Lora_init()	
* 功	能：初始化与LORA模块相连的引脚
* 入口参数：无
* 出口参数：无
* 说	明：无
****************************************************************************/
void Lora_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//配置PC0,PC1为输出模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;			//M0,M1引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;				//频率2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);							//初始化引脚 
    GPIO_SetBits(GPIOC,GPIO_Pin_1);
    GPIO_SetBits(GPIOC,GPIO_Pin_0);
    
    //配置PC3为输入模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			            //AUX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;				//频率2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	                        //初始化引脚
}	

/****************************************************************************
* 名	称：void Lora_Write_REG(u8 Begin_Addr, u8 length, u8 ADDH, u8 ADDL, u8 NETID, u8 REG0, u8 REG1, u8 REG2, u8 REG3)	
* 功	能：LORA模块写多个寄存器
* 入口参数：
* Begin_Addr：开始地址； length：要写的数据长度
* ADDH：高地址；ADDL：低地址；NETID：网络地址； 
* REG0, REG1, REG2, REG3：寄存器
* 出口参数：无
* 说	明：无
****************************************************************************/
void Lora_Write_REG(u8 Begin_Addr, u8 length, u8 ADDH, u8 ADDL, u8 NETID, u8 REG0, u8 REG1, u8 REG2, u8 REG3)
{
    USART3_Real.TxBuf[0] = 0xC0;    	//设置数据头
    USART3_Real.TxBuf[1] = Begin_Addr;     
    USART3_Real.TxBuf[2] = length;    
    USART3_Real.TxBuf[3] = ADDH;          
    USART3_Real.TxBuf[4] = ADDL;      
    USART3_Real.TxBuf[5] = NETID;    
    USART3_Real.TxBuf[6] = REG0;   	 	 //配置串口速率，校验位、无线空中速率    	  
    USART3_Real.TxBuf[7] = REG1;	     //配置分包长度与发射功率
    USART3_Real.TxBuf[8] = REG2;		 //信道控制
    USART3_Real.TxBuf[9] = REG3;		 //配置传输方式等
    USART3_SendString(USART3_Real.TxBuf, length+3);      
    // Length指的是length后面参数的总长，所以要加上前面的参数头        
}
/****************************************************************************
* 名	称：void Lora_Read_REG(u8 Begin_Addr, u8 length)
* 功	能：读LORA模块的寄存器
* 入口参数：
* Begin_Addr：开始地址； length：要写的数据长度
* 出口参数：无
* 说	明：无
****************************************************************************/
void Lora_Read_REG(u8 Begin_Addr, u8 length)
{   
    USART3_Real.TxBuf[0] = 0xC1;
    USART3_Real.TxBuf[1] = Begin_Addr;   
    USART3_Real.TxBuf[2] = length;
    
    USART3_SendString(USART3_Real.TxBuf, 3);                        
}

/****************************************************************************
* 名	称：void Lora_Mode(u8 mode)	
* 功	能：配置LORA模块的工作模式
* 入口参数：
*  mode：模式
* 出口参数：无
* 说	明：无
****************************************************************************/
void Lora_Mode(u8 mode)
{   
    switch(mode)
    {
        case 0x00: //传输模式
	    GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	    GPIO_ResetBits(GPIOC,GPIO_Pin_0);
        break;
        case 0x01://WOR模式
            GPIO_ResetBits(GPIOC,GPIO_Pin_1);
            GPIO_SetBits(GPIOC,GPIO_Pin_0);
        break;
        case 0x02://配置模式
            GPIO_SetBits(GPIOC,GPIO_Pin_1);
            GPIO_ResetBits(GPIOC,GPIO_Pin_0);
        break;
        case 0x03://深度休眠模式
  		GPIO_SetBits(GPIOC,GPIO_Pin_1);
    	GPIO_SetBits(GPIOC,GPIO_Pin_0);
        break;
	}   
//等待AUX脚变为低电平（说明模式切换完成）
 while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3) == 0)                    
     Delay_ms(7);  
}





/*

#include <stm32f10x.h>
#include "lora.h"
//#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "led.h"
#include "key.h"
//#include "lcd.h"
 
//???????(???????lora_cfg.h??)
_LoRa_CFG LoRa_CFG=
{
	.addr = LORA_ADDR,       //????
	.power = LORA_POWER,     //????
	.chn = LORA_CHN,         //??
	.wlrate = LORA_RATE,     //????
	.wltime = LORA_WLTIME,   //????
	.mode = LORA_MODE,       //????
	.mode_sta = LORA_STA,    //????
	.bps = LORA_TTLBPS ,     //?????
	.parity = LORA_TTLPAR    //?????
};
 
//????
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
 
//??????(????????)
u8 Lora_mode=0;//0:???? 1:???? 2:????
//??????
static u8 Int_mode=0;//0:?? 1:??? 2:???
 
 
//usmart????
//????AT?????????????
//mode:0,???USART3_RX_STA;
//     1,??USART3_RX_STA;
void lora_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//????????
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//?????
		printf("%s",USART3_RX_BUF);	//?????
		if(mode)USART3_RX_STA=0;
	} 
}
//lora?????,????????
//str:???????
//???:0,???????????
//??,?????????(str???)
u8* lora_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//????????
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//?????
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//lora????
//cmd:????????(????????),?cmd<0XFF???,????(????0X1A),??????????.
//ack:???????,????,??????????
//waittime:????(??:10ms)
//???:0,????(??????????)
//       1,????
u8 lora_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART3->SR&0X40)==0);//???????????  
		USART3->DR=(u32)cmd;
	}else u3_printf("%s\r\n",cmd);//????
	
	if(ack&&waittime)		//??????
	{
	   while(--waittime)	//?????
	   { 
		  Delay_ms(10);
		  if(USART3_RX_STA&0X8000)//??????????
		  {
			  if(lora_check_cmd(ack))break;//?????? 
			  USART3_RX_STA=0;
		  } 
	   }
	   if(waittime==0)res=1; 
	}
	return res;
}
 
 
//AUX????
//mode:????? 0:?? 1:??? 2:???
void Aux_Int(u8 mode)
{
    if(!mode)
	{
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;//????
		NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
	}else
	{
		if(mode==1)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //???
	    else if(mode==2)
			 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//???
		
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	}
	Int_mode = mode;//??????
	EXTI_Init(&EXTI_InitStructure);
	NVIC_Init(&NVIC_InitStructure);  
    
}
 
//LoRa?????
//???: 0,????
//        1,???? 
u8 LoRa_Configure(void)
{
	 u8 retry=0;
	 u8 temp=1;
	
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//??PA????
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//????????
 
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//??JTAG,??PA15?????IO??,??PA15?????IO!!!	
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	    		 //LORA_MD0
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //???? ,IO????50MHz
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LORA_AUX
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		     //????
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //?????????GPIOA.4
	
	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	
	 EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	 EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	 EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //?????
  	 EXTI_InitStructure.EXTI_LineCmd = DISABLE;              //?????(????????)
  	 EXTI_Init(&EXTI_InitStructure);//??EXTI_InitStruct???????????EXTI???
	
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//LORA_AUX
  	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//?????2, 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		//????3
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; //????????(?????)
   	 NVIC_Init(&NVIC_InitStructure); 
	 
	 LORA_MD0=0;
	 LORA_AUX=0;
	
	 while(LORA_AUX)//??LORA????????(LORA_AUX=0)
	 {
		 Delay_ms(600); 
	 }
	 usart3_init(115200);//?????3
	 
	 LORA_MD0=1;//??AT??
	 Delay_ms(40);
	 retry=3;
	 while(retry--)
	 {
		 if(!lora_send_cmd("AT","OK",70))
		 {
			 temp=0;//????
			 break;
		 }	
	 }
	 if(retry==0) temp=1;//????
	 return temp;
}
void LoRa_Init(void)
{
	while(LoRa_Configure())//???ATK-LORA-01??
	{	
		Delay_ms(300);
	}
	LoRa_Set();
}
 
//Lora??????
void LoRa_Set(void)
{
	u8 sendbuf[20];
	u8 lora_addrh,lora_addrl=0;
	
	usart3_set(LORA_TTLBPS_115200,LORA_TTLPAR_8N1);//??????????????????(115200 8??? 1??? ?????)
	usart3_rx(1);//????3??
	
	while(LORA_AUX);//??????
	LORA_MD0=1; //??????
	Delay_ms(40);
	Lora_mode=0;//??"????"
	
	lora_addrh =  (LoRa_CFG.addr>>8)&0xff;
	lora_addrl = LoRa_CFG.addr&0xff;
	
	sprintf((char*)sendbuf,"AT+ADDR=%02x,%02x",lora_addrh,lora_addrl);//??????
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+WLRATE=%d,%d",LoRa_CFG.chn,LoRa_CFG.wlrate);//?????????
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+TPOWER=%d",LoRa_CFG.power);//??????
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+CWMODE=%d",LoRa_CFG.mode);//??????
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+TMODE=%d",LoRa_CFG.mode_sta);//??????
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+WLTIME=%d",LoRa_CFG.wltime);//??????
	lora_send_cmd(sendbuf,"OK",50);
	
	sprintf((char*)sendbuf,"AT+UART=%d,%d",LoRa_CFG.bps,LoRa_CFG.parity);//?????????????
	lora_send_cmd(sendbuf,"OK",50);
 
	LORA_MD0=0;//????,????
	Delay_ms(40);
	while(LORA_AUX);//??????(?????????)
	USART3_RX_STA=0;
	Lora_mode=1;//??"????"
	usart3_set(LoRa_CFG.bps,LoRa_CFG.parity);//????,????????(?????????)
	Aux_Int(1);//??LORA_AUX?????	
	
}
 
u8 Dire_Date[]={0x11,0x22,0x33,0x44,0x55};//??????
u8 date[30]={0};//????
 
 
 
u32 obj_addr = 0;//??????????
u8 obj_chn = 0;//??????????
 
u8 wlcd_buff[10]={0}; //LCD????????
//Lora??????
void LoRa_SendData(u8 Data[])
{      
	static u8 num=0;
    u16 addr;
	u8 chn;
	u16 i=0; 
		
	if(LoRa_CFG.mode_sta == LORA_STA_Tran)//????
	{
		u3_printf("%s\r\n",Data);
		num++;
		if(num==255) num=0;
		
	}else if(LoRa_CFG.mode_sta == LORA_STA_Dire)//????
	{
		
		addr = (u16)obj_addr;//????
		chn = obj_chn;//????
		
		date[i++] =(addr>>8)&0xff;//????
		date[i++] = addr&0xff;//????
		date[i] = chn;//????
		
		for(i=0;i<Dire_DateLen;i++)//??????BUFF
		{
			date[3+i] = Dire_Date[i];
		}	
		for(i=0;i<(Dire_DateLen+3);i++)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);//????,??????   
			USART_SendData(USART3,date[i]); 
		}	
		
        //?????????????????lcd_buff??
		sprintf((char*)wlcd_buff,"%x %x %x %x %x %x %x %x",
				date[0],date[1],date[2],date[3],date[4],date[5],date[6],date[7]);
	    Dire_Date[4]++;//Dire_Date[4]????		
	}
			
}
 
u8 rlcd_buff[10]={0}; //LCD????????
//Lora??????
u8* LoRa_ReceData(void)
{
 
    u16 len=0;
   
	//?????
	if(USART3_RX_STA&0x8000)
	{
		LED1 =0;
		memset((char*)USART3_RX_BUF,0x00,len);//????????0
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//?????
		USART3_RX_STA=0;
 
		if(LoRa_CFG.mode_sta==LORA_STA_Tran)//????
		{	
 
		}else if(LoRa_CFG.mode_sta==LORA_STA_Dire)//????
		{
			//?????????????????lcd_buff??
			sprintf((char*)rlcd_buff,"%x %x %x %x %x",
			USART3_RX_BUF[0],USART3_RX_BUF[1],USART3_RX_BUF[2],USART3_RX_BUF[3],USART3_RX_BUF[4]);
		}
		
	}
	return USART3_RX_BUF;
 
}
 
 
 
//LORA_AUX??????
void EXTI4_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line4))
	{  
	   if(Int_mode==1)//???(??:?????? ??:??????)     
	   {
		  if(Lora_mode==1)//????
		  {
			 USART3_RX_STA=0;//?????0
		  }
		  Int_mode=2;//???????
		  LED0=0;//DS0?
	   }
       else if(Int_mode==2)//???(??:?????? ??:??????)	
	   {
		  if(Lora_mode==1)//????
		  {
			 USART3_RX_STA|=1<<15;//????????
		  }else if(Lora_mode==2)//????(????????)
		  {
			 Lora_mode=1;//??????
		  }
		  Int_mode=1;//???????
          LED0=1;//DS0?		   
	   }
       Aux_Int(Int_mode);//????????
	   EXTI_ClearITPendingBit(EXTI_Line4); //??LINE4???????  
	}	
}
 */
 