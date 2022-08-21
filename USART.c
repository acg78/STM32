#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "Delay.h"
#include "lora.h"
#include  "string.h"


//接收缓冲区容量
#define USART1_RX_BUFF_SIZEMAX 256
#define USART1_TX_BUFF_SIZEMAX 256

//USART设备控制结构体
typedef struct
    {
        u32			BaudRate;				//波特率
        u32			R_Idx;
        u8			RxBuf[USART1_RX_BUFF_SIZEMAX];
        u8			TxBuf[USART1_TX_BUFF_SIZEMAX];
        u8          data_length;
        u8          Get_length;
    } USART1_STRUCT;


u8 S_RxData;
u8 S_Flag;
	
USART1_STRUCT USART3_Real={115200,0,0,0,0,0};


void USART_Config(int bot)
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
	USART_InitStructure.USART_BaudRate=bot;
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


void USART_SentArr(u8 *Arr)
{
	u16 i;
	for(i=0;i<12;i++)
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

void Lora_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//串口引脚分配时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//M0,M1引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//频率2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	
	//    M0为B3 M1为A15
	
	//配置PC0,PC1为输出模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;		//M0,M1引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//频率2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化引脚 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;			//M0,M1引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;				//输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//频率2MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);							//初始化引脚 

	//拉高引脚电平
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
    GPIO_SetBits(GPIOB,GPIO_Pin_3);
	
	//配置PC3为输入模式 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			            //AUX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//频率2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	                        //初始化引脚
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

void Lora_Mode(u8 mode)
{   

    switch(mode)
    {
		
        case 0x00: //传输模式
		{
	    GPIO_ResetBits(GPIOB,GPIO_Pin_3);//M0
	    GPIO_ResetBits(GPIOA,GPIO_Pin_15);//M1
        break;
		}
        case 0x01://WOR模式
		{
	    GPIO_SetBits(GPIOB,GPIO_Pin_3);//M0
	    GPIO_ResetBits(GPIOA,GPIO_Pin_15);//M1
        break;
		}
        case 0x02://配置模式
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_3);//M0 0
			GPIO_SetBits(GPIOA,GPIO_Pin_15);//M1 1
			break;
		}
        case 0x03://深度休眠模式
		{
  		GPIO_SetBits(GPIOB,GPIO_Pin_3);
    	GPIO_SetBits(GPIOA,GPIO_Pin_15);
        break;
		}
		
	}   
	//等待AUX脚变为低电平（说明模式切换完成）
	//while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3) == 0)                    
	Delay_ms(100);  
}

void USART3_SendString(u8* Data, u32 Len)
{
    u32 i=0;
    for(i=0; i<Len; i++)
    {
        USART_SendData(USART1, Data[i]);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    memset(&USART3_Real.TxBuf, 0, USART1_TX_BUFF_SIZEMAX);
}


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





