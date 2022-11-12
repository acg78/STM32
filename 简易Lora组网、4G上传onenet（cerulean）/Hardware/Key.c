#include "stm32f10x.h"                  // Device header
#include "Delay.h"

u8 KeyNum=0;
extern u8 JU;
void Key_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void)
{

	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
	{
		
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0);
		Delay_ms(20);
		KeyNum = 1;
		
	}
	return KeyNum;
	
}

u8 Key_GetConfirm(void)
{
	u8 Confirm=0;
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0)
	{
		
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0);
		Delay_ms(20);
		Confirm = 1;
		
	}
	return Confirm;
	

}



