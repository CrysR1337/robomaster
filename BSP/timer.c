#include "main.h"

//Timer 2 32-bit counter  
//Timer Clock is 168MHz / 4 * 2 = 84M

void TIM8_Configuration(void)
{

		TIM_TimeBaseInitTypeDef tim;	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);    
  	tim.TIM_Period = 0xFFFFFFFF;     
  	tim.TIM_Prescaler = 168-1;	 //1M 的时钟  
  	tim.TIM_ClockDivision = TIM_CKD_DIV1;	
  	tim.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_ARRPreloadConfig(TIM8, ENABLE);	
		
		//应用配置到TIM2 
  	TIM_TimeBaseInit(TIM8, &tim);
		
		// 使能TIM2重载寄存器ARR
  	TIM_ARRPreloadConfig(TIM8, ENABLE);	
	  TIM_PrescalerConfig(TIM8, 0, TIM_PSCReloadMode_Update);
		
		/* Disable the TIM2 Update event */
		TIM_UpdateDisableConfig(TIM8, ENABLE);
	
	  TIM_Cmd(TIM8,ENABLE);	   
}



void TIM6_Configuration(void)
{
			// todo 理解定时器定义，更改代码实现控制LED灯每秒闪烁1次
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    nvic.NVIC_IRQChannel  = TIM6_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	
	
		tim.TIM_Period = 0xFFFFFFFF; //设置自动重载周期
    tim.TIM_Prescaler = 168-1;      //设置预分频值  
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;   //向上计数模式
    TIM_TimeBaseInit(TIM6,&tim);//使能TIM6重载寄存器ARR 

    //TIM_Cmd(TIM6,ENABLE);
	
}

void TIM6_Start(void)
{
    TIM_Cmd(TIM6, ENABLE);	 
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);//使能TIM6更新中断
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
	  //todo change the led 
		GPIO_ResetBits( GPIOB , GPIO_Pin_9 );
}

void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);    
    tim.TIM_Period = 0xFFFFFFFF;     
    tim.TIM_Prescaler = 168-1;	 //1M 的时钟  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_ARRPreloadConfig(TIM4, ENABLE);	//操作TIM4_CR1寄存器，使能自动重装载的预装载寄存器
    //应用配置到TIM2 
    TIM_TimeBaseInit(TIM4, &tim);
    // 使能TIM2重载寄存器ARR
    TIM_PrescalerConfig(TIM4, 0, TIM_PSCReloadMode_Update);
    /* Disable the TIM2 Update event */
    TIM_UpdateDisableConfig(TIM4, ENABLE);

    TIM_Cmd(TIM4,ENABLE);	
}

void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    tim.TIM_Period = 0xFFFFFFFF;
    tim.TIM_Prescaler = 84 - 1;	 //1M 的时钟  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_ARRPreloadConfig(TIM2, ENABLE);	
    TIM_TimeBaseInit(TIM2, &tim);

    TIM_Cmd(TIM2,ENABLE);	
}
   
void TIM2_IRQHandler(void)
{
	  if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
		{
			  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        TIM_ClearFlag(TIM2, TIM_FLAG_Update);
			  BOTH_LED_TOGGLE();
		}
} 

void TIM6_DAC_IRQHandler(void)  
{
	
    if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	  {
				TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//清除中断标志
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
				Control_Task();         //底盘、云台控制任务
			 //todo 让板载led灯的状态发生反转 亮变灭 灭变暗
				GPIO_ToggleBits( GPIOB , GPIO_Pin_9 );
    }
}

int32_t ms_count = 0;
uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}


