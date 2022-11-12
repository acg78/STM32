#include <stdio.h>
#include "uart4g.h"



//int fputc(int ch, FILE* p)
//{
//	USART_SendData(USART3, (u8)ch);
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);//发送数据寄存器为空，已传至发送移位寄存器，但不一定发送完成
//	return ch;
//}


/*******************************************************************************
* 函 数 名         : USART3_Init
* 函数功能		   : USART3初始化函数
* 输    入         : bound:波特率
* 输    出         : 无
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //串口3时钟使能
 
 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11
	
	USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART3, &USART_InitStructure); //初始化串口	3
  
 
	USART_Cmd(USART3, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	

}
//初始化串口2
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1，使能 GPIOA，USART2，AFIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 	//GPIO复用时钟，外部中断的GPIO桥接时钟 
	
	
	//2，初始化GPIO引脚， PA2初始化为推挽复用	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX			//串口输出PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     	//推挽复用模式
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//PA3初始化为模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX			//串口输入PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	

	//3，初始化 USART2
	USART_InitStructure.USART_BaudRate = bound;	//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式，使用“|”进行同时选择
	USART_Init(USART2, &USART_InitStructure); //初始化串口 2
		
	//初始化 Usart2 的NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//串口1中断通道，“stm32f10x.h”
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);			
	
	//使能串口中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	//使能串口2
	USART_Cmd(USART2, ENABLE); 
}



//发送一个字节
void USART_SentByte2(u8 Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);

}
/*******************************************************************************
* 函数名  : UART1_SendString
* 描述    : USART1发送字符串
* 输入    : *s字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
//void UART3_SendString(char* s)
//{
//	while(*s)//检测字符串结束符
//	{
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
//		USART_SendData(USART1 ,*s++);//发送当前字符
//	}
//}


//串口2发送字符串函数
void UART2_SendString(char* s)
{
	printf("%s",s);
	while(*s)//检测字符串结束符
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//发送当前字符
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}


//串口3发送字符串函数
void UART3_SendString(char *pstr)
{
	char *p = pstr;
	while(*p!='\0')
	{
		USART_SendData(USART3, *p);
		while( USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);//等待发送完成
		USART_ClearFlag(USART3,USART_FLAG_TXE);//清空标志位、
		p++;  //指针偏移
	}
}
void USART3_IRQHandler(void)
{
//	uint8_t d;

	//检测标志位
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
	//	d=USART_ReceiveData(USART1);	//接收数据
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//清空标志位
	}
}
u8  U2_RX_BUF[1024]={0};//串口接收缓存区 	
u16  U2_RX_Cnt=0;
u8  U2_RX_Cmd = 0;
u8  U2_RX_Error = 0;
void USART2_IRQHandler(void)
{
	u8 d;
	//USART_SendData(USART1, 0x35);
	//检测标志位
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		//接收数据
		//USART_SendData(USART1, 0x35);
		d=USART_ReceiveData(USART2); //接收数据
		
		if(U2_RX_Cnt>1024) 
			U2_RX_Cnt = 0;
		U2_RX_BUF[U2_RX_Cnt++] = d;//记录接收到的值
		
//		//下发命令时，会收到：
//	/*
//	+QMTRECV: 0,0,"/sys/a1VPyJEJRjJ/test01/thing/service/property/set",92,"{"method":"thing.service.property.set","id":"44777986","params":{"LED":1},"version":"1.0.0"}"
//	*/
//		//判断有没有接收到命令，version
//		if(U2_RX_BUF[U2_RX_Cnt-7]=='v'&&U2_RX_BUF[U2_RX_Cnt-6]=='e'&&U2_RX_BUF[U2_RX_Cnt-5]=='r'&&U2_RX_BUF[U2_RX_Cnt-4]=='s'&&U2_RX_BUF[U2_RX_Cnt-3]=='i'&&U2_RX_BUF[U2_RX_Cnt-2]=='o'&&d == 'n')
//		{
//			U2_RX_Cmd = 1;  //接收到命令
//			U2_RX_Cnt = 0;
//		}
//		//判断有没有发生错误，ERROR
//		if(U2_RX_BUF[U2_RX_Cnt-5]=='E'&&U2_RX_BUF[U2_RX_Cnt-4]=='R'&&U2_RX_BUF[U2_RX_Cnt-3]=='R'&&U2_RX_BUF[U2_RX_Cnt-2]=='O'&&d == 'R')
//		{
//			U2_RX_Error = 1;  //接收到错误
//			U2_RX_Cnt = 0;
//		}
	
		//将接收到的数据，通过串口1返发给电脑
		USART_SendData(USART3, d);
	    while( USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET); //等待发送完成
		USART_ClearFlag(USART3,USART_FLAG_TXE);//清空标志位
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//清空标志位
	}
}

