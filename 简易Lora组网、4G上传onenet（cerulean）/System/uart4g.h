#ifndef __UART4G_H
#define __UART4G_H

#include "stm32f10x.h"    
// Device header

//�ⲿ���ô��ڽ���������ɵı���
extern u8 U2_RX_BUF[1024];  //���ջ����� 
extern  u16  U2_RX_Cnt;    //���ռ���
extern u8  U2_RX_Cmd ;      //���������־λ U2_RX_Cmd = 1
extern u8  U2_RX_Error;    // ���յ�����

void USART3_Init(u32 bound);

void USART2_Init(u32 bound);

void UART2_SendString(char* s);

void UART3_SendString(char *pstr);

void USART_SentByte2(u8 Byte);



#endif

