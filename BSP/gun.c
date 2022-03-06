#include "stm32f4xx.h"
#include "gun.h"

#include "timer.h"

void PWM_Configuration(void)
{
		// todo PWM  port pa9 tim1 
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     tim;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
		//RCC_APB2PeriphClockCmd(RCC_APBlPeriph_AFIO,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
		Led_Configuration();	
	
		tim.TIM_Period = 0xFFFFFFFF;  //   数值不确定该写多少
    tim.TIM_Prescaler = 168-1;	 //1M 的时钟  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM3, &tim);
	
		TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;        // PWM??2:CNT>CCR?????
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// ????-??????
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// ????
    TIM_OC2Init(TIM3,&TIM_OCInitStrue);
	
		TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
		
		TIM_Cmd(TIM3,ENABLE);
}



//void PWM_Configuration(void)
//{
//	// todo PWM  port pa9 tim1 
//	//debug模式是不是必须要连接什么东西才能使用啊？本来想用这个来看一下代码有没有问题，但是发现还是操作不来
//	//使能一个时钟是不是就是激活开启时钟的意思？
//	//关于TIM时钟的预分频值和向上计数模式是什么意思
//	//nvic.NVIC_IRQChannel  = 应该配置什么，实在没有找到
//	//没有找到usart发送数据并接受的文档
//}
