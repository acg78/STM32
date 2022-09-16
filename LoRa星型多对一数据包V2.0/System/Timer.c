
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
//extern uint16_t Num;//Num为外部变量，自动寻找该变量位置
extern u8 j;
extern u8 JU;
extern u8 KeyNum;

void Timer_Init(uint16_t arr,uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//选择内部始时钟
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	//向上计数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	//1s
	TIM_TimeBaseInitStructure.TIM_Period=arr-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;//PSC
	//高级定时器特有
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
	
	

}



void TIM3_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
		j++;
		
		if(JU<20&&KeyNum==1)
		{
			JU++;
		}
		
		else
		{
			JU=0;
			KeyNum=0;
		}
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}

}


