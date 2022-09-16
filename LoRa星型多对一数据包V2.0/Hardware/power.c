#include "stm32f10x.h"

void PWM3_SetCom(u16 Compare)
{
	TIM_SetCompare3(TIM2,Compare);
	

}	


void Motor(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	
}
//void Motor_Speed(int8_t Speed)
//{
//	if(Speed>=0)
//	{
//		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//		PWM3_SetCom(Speed);
//	}
//	else
//	{
//		GPIO_SetBits(GPIOA,GPIO_Pin_5);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//		PWM3_SetCom(-Speed);
//	
//	}


//}

void Power(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//选择内部始时钟
	TIM_InternalClockConfig(TIM2);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//向上计数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	//1s
	TIM_TimeBaseInitStructure.TIM_Period=100-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;//PSC
	//高级定时器特有
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1 ;//PWM1模式
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	TIM_Cmd(TIM2,ENABLE);


}





