#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void PWM_Init(u16 arr,u16 psc,u16 Num)	//��������
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	
	TIM_InternalClockConfig(TIM2);//ѡ���ڲ�ʼʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=arr-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
//	TIM_ClearFlag(TIM2,TIM_FLAG_Update);����жϱ�־λ
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//ʹ���ж�
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	TIM_OCStructInit(&TIM_OCInitStructure);//���ṹ�帽��ʼֵ
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1 ;//PWM1ģʽ
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//����Ƚϼ���
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=Num;//CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//������������������ŵĿ���Ȩ����Ƭ�����裬��OC�������ǼĴ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��

}

void Buffer(void)
{
	u8 i=0;
	u8 j=0;
	int a[37]={
		28,28,28,0,
		28,28,28,0 ,   
		28,22,34,30,
		28,0,0,0,
		25,25,25,25,
		25,28,28,0,
		28,30,30,34,
		30,0,22,0,
		22,22,25,30,34};//�������飬f=72MHZ/[(arr+1)(psc+1)]�����arr
	int b[37]={250,250,250,250,
		       250,250,250,250,
		       250,250,250,250,
		       250,250,250,250,
	           250,250,250,250,
		      250,250,250,250,
		       250,250,250,250,
		       250,250,250,250,
			   250,250,250,250,250};
	while(1)
	{
		PWM_Init(a[i],7200,a[i]/2);
		Delay_ms(b[j]);
		
		if(i==37&&j==37)
		{
			break;
		}	
		i++;
		j++;
	}	

}


