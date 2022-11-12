#include "stm32f10x.h"

//A2 为TIM2的CH3输出PWM


//设置CCR的值
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//选择内部始时钟TIM2挂载APB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	
	TIM_InternalClockConfig(TIM2);//选择内部时钟
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//时基单元
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Period=100-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=720-1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM2,ENABLE);//启动定时器
	
	
	//初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);//初始化结构体，高级定时期功能更多
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1 ;//PWM1模式，
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//CCR
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);//输出比较单元，CCR也即占空比部分，CNT与ccr比较看看是否正常翻转
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}





