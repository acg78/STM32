#include "stm32f10x.h"                  // Device header

extern u8 heart;
void Timer_Init4(uint16_t arr,uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//开启APB1总线，定时器挂载在次总线上
	TIM_InternalClockConfig(TIM4);	//选择时基单元时钟，默认为内部始时钟
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//定时器的外部信号输入引脚，会有一个滤波器过滤掉抖动干扰，即在频率f下连续采样若值相同，即代表稳定了，该频率由内部时钟而来，也可分频，此参数即为分频值
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr-1;//ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;//PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//高级定时器特有，对最大59.9秒再进行选择周期
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//
	
	
	
	
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);//清除中断
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//使能中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVIC选择优先分组
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4,ENABLE);//启动定时器
	
}

void TIM4_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		heart=1;
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	}
	
	
}

