#include <stdio.h>
#include "uart4g.h"



//int fputc(int ch, FILE* p)
//{
//	USART_SendData(USART3, (u8)ch);
//	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);//�������ݼĴ���Ϊ�գ��Ѵ���������λ�Ĵ���������һ���������
//	return ch;
//}


/*******************************************************************************
* �� �� ��         : USART3_Init
* ��������		   : USART3��ʼ������
* ��    ��         : bound:������
* ��    ��         : ��
*******************************************************************************/ 
void USART3_Init(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //����3ʱ��ʹ��
 
 	USART_DeInit(USART3);  //��λ����3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11
	
	USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������	3
  
 
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
	//ʹ�ܽ����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	

}
//��ʼ������2
void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1��ʹ�� GPIOA��USART2��AFIO ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 	//GPIO����ʱ�ӣ��ⲿ�жϵ�GPIO�Ž�ʱ�� 
	
	
	//2����ʼ��GPIO���ţ� PA2��ʼ��Ϊ���츴��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX			//�������PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     	//���츴��ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//PA3��ʼ��Ϊģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//RX			//��������PA3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	

	//3����ʼ�� USART2
	USART_InitStructure.USART_BaudRate = bound;	//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ��ʹ�á�|������ͬʱѡ��
	USART_Init(USART2, &USART_InitStructure); //��ʼ������ 2
		
	//��ʼ�� Usart2 ��NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		//����1�ж�ͨ������stm32f10x.h��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);			
	
	//ʹ�ܴ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
	
	//ʹ�ܴ���2
	USART_Cmd(USART2, ENABLE); 
}



//����һ���ֽ�
void USART_SentByte2(u8 Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);

}
/*******************************************************************************
* ������  : UART1_SendString
* ����    : USART1�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
//void UART3_SendString(char* s)
//{
//	while(*s)//����ַ���������
//	{
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); 
//		USART_SendData(USART1 ,*s++);//���͵�ǰ�ַ�
//	}
//}


//����2�����ַ�������
void UART2_SendString(char* s)
{
	printf("%s",s);
	while(*s)//����ַ���������
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET); 
		USART_SendData(USART2 ,*s++);//���͵�ǰ�ַ�
	}
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
}


//����3�����ַ�������
void UART3_SendString(char *pstr)
{
	char *p = pstr;
	while(*p!='\0')
	{
		USART_SendData(USART3, *p);
		while( USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET);//�ȴ��������
		USART_ClearFlag(USART3,USART_FLAG_TXE);//��ձ�־λ��
		p++;  //ָ��ƫ��
	}
}
void USART3_IRQHandler(void)
{
//	uint8_t d;

	//����־λ
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
	//	d=USART_ReceiveData(USART1);	//��������
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//��ձ�־λ
	}
}
u8  U2_RX_BUF[1024]={0};//���ڽ��ջ����� 	
u16  U2_RX_Cnt=0;
u8  U2_RX_Cmd = 0;
u8  U2_RX_Error = 0;
void USART2_IRQHandler(void)
{
	u8 d;
	//USART_SendData(USART1, 0x35);
	//����־λ
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		//��������
		//USART_SendData(USART1, 0x35);
		d=USART_ReceiveData(USART2); //��������
		
		if(U2_RX_Cnt>1024) 
			U2_RX_Cnt = 0;
		U2_RX_BUF[U2_RX_Cnt++] = d;//��¼���յ���ֵ
		
//		//�·�����ʱ�����յ���
//	/*
//	+QMTRECV: 0,0,"/sys/a1VPyJEJRjJ/test01/thing/service/property/set",92,"{"method":"thing.service.property.set","id":"44777986","params":{"LED":1},"version":"1.0.0"}"
//	*/
//		//�ж���û�н��յ����version
//		if(U2_RX_BUF[U2_RX_Cnt-7]=='v'&&U2_RX_BUF[U2_RX_Cnt-6]=='e'&&U2_RX_BUF[U2_RX_Cnt-5]=='r'&&U2_RX_BUF[U2_RX_Cnt-4]=='s'&&U2_RX_BUF[U2_RX_Cnt-3]=='i'&&U2_RX_BUF[U2_RX_Cnt-2]=='o'&&d == 'n')
//		{
//			U2_RX_Cmd = 1;  //���յ�����
//			U2_RX_Cnt = 0;
//		}
//		//�ж���û�з�������ERROR
//		if(U2_RX_BUF[U2_RX_Cnt-5]=='E'&&U2_RX_BUF[U2_RX_Cnt-4]=='R'&&U2_RX_BUF[U2_RX_Cnt-3]=='R'&&U2_RX_BUF[U2_RX_Cnt-2]=='O'&&d == 'R')
//		{
//			U2_RX_Error = 1;  //���յ�����
//			U2_RX_Cnt = 0;
//		}
	
		//�����յ������ݣ�ͨ������1����������
		USART_SendData(USART3, d);
	    while( USART_GetFlagStatus(USART3, USART_FLAG_TXE)==RESET); //�ȴ��������
		USART_ClearFlag(USART3,USART_FLAG_TXE);//��ձ�־λ
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//��ձ�־λ
	}
}

