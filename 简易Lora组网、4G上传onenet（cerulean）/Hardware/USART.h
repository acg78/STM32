#ifndef __USART_H
#define __USART_H	 


#include "stdio.h"
void USART_Config(int bot);
void USART_SentByte(u8 Byte);
void USART_SentArr(u8 *Arr,u8 Len);
void USART_SentString(char *string);
void USART_SendNumber(u32 Num,u8 Length);
u8 S_GetRxFlag(void);
u8 S_GetRxData(void);
void USART1_IRQHandler(void);

void S_SendPack(void);
void Lora_init(void);
void Lora_Mode(u8 mode);
void USART3_SendString(u8* Data, u32 Len);
void Lora_Write_REG(u8 Begin_Addr, u8 length, u8 ADDH, u8 ADDL, u8 NETID, u8 REG0, u8 REG1, u8 REG2, u8 REG3);
void Lora_Read_REG(u8 Begin_Addr, u8 length);
void USART3_SendString(u8* Data, u32 Len);
#endif
 
