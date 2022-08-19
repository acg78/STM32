#ifndef __USART_H
#define __USART_H	 


#include "stdio.h"
void USART_Config(void);
void USART_SentByte(u8 Byte);
void USART_SentArr(u8 *Arr,u16 Length);
void USART_SentString(char *string);
void USART_SendNumber(u32 Num,u8 Length);
u8 S_GetRxFlag(void);
u8 S_GetRxData(void);
void USART1_IRQHandler(void);


#endif
 
