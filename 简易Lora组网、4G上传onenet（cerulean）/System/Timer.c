
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
//extern uint16_t Num;//NumΪ�ⲿ�������Զ�Ѱ�Ҹñ���λ��
extern u8 j;
extern u8 JU;
extern u8 KeyNum;

void Timer_Init(uint16_t arr,uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//����APB1���ߣ���ʱ�������ڴ�������

	TIM_InternalClockConfig(TIM3);	//ѡ��ʱ����Ԫʱ�ӣ�Ĭ��Ϊ�ڲ�ʼʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ʱ�����ⲿ�ź��������ţ�����һ���˲������˵��������ţ�����Ƶ��f������������ֵ��ͬ���������ȶ��ˣ���Ƶ�����ڲ�ʱ�Ӷ�����Ҳ�ɷ�Ƶ���˲�����Ϊ��Ƶֵ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ
	TIM_TimeBaseInitStructure.TIM_Period=arr-1;//ARR�Զ���װ����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;//PSCԤ��Ƶ����ֵ
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;//�߼���ʱ�����У������59.9���ٽ���ѡ������
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//
	
	
	
	
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//����ж�
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//ʹ���ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//NVICѡ�����ȷ���
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;//�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);//������ʱ��
	
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


