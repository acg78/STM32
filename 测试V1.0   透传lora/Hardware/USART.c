#include "stm32f10x.h"                  // Device header
#include "stdio.h"



u8 S_RxData;
u8 S_Flag;
void USART_Config(void)
{
	//      开启时钟串口和针脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	
	//复用GPIOA9为USART的发送端TX
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//复用GPIOA10为USART的接收端RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	//配置串口参数
	USART_InitTypeDef USART_InitStructure;
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	
	USART_Init(USART1,&USART_InitStructure);
	
	//使能串口
	USART_Cmd(USART1,ENABLE);
	
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	//USART中断设置
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

}


void USART_SentByte(u8 Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);

}

void USART_SentArr(u8 *Arr,u16 Length)
{
	u16 i;
	for(i=0;i<Length;i++)
	{
		USART_SentByte(Arr[i]);
	}
	
}


void USART_SentString(char *string)
{
	u8 i;
	for(i=0;string[i]!=0;i++)
	{
		USART_SentByte(string[i]);
	}
	
	
	
	
}

u32 Pow(u32 X,u32 Y)
{
	
	uint32_t Result=1;
	while(Y--)
	{
		Result *=X;
	}
	
	return Result;
	
	
}

void USART_SendNumber(u32 Num,u8 Length)
{
	u8 i;
	for(i=0;i<Length;i++)
	{
		USART_SentByte(Num/Pow(10,Length-i-1)%10+'0');
	}
	
	
}


int fputc(int ch,FILE *f)
{
	USART_SentByte(ch);
	return ch;

}
u8 S_GetRxFlag(void)
{
	if(S_Flag==1)
	{
		S_Flag=0;
		return 1;
	}
	else
		return 0;
}

u8 S_GetRxData(void)
{
	return S_RxData;

}

//只能使用特定函数名，因为使用不同的中断类型

void USART1_IRQHandler(void)
{
		if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
		{
			S_RxData=USART_ReceiveData(USART1);
			S_Flag=1;
			USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
		}
}



