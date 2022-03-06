#include "main.h"

//Timer 2 32-bit counter  
//Timer Clock is 168MHz / 4 * 2 = 84M

void TIM8_Configuration(void)
{

		TIM_TimeBaseInitTypeDef tim;	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);    
  	tim.TIM_Period = 0xFFFFFFFF;     
  	tim.TIM_Prescaler = 168-1;	 //1M ��ʱ��  
  	tim.TIM_ClockDivision = TIM_CKD_DIV1;	
  	tim.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_ARRPreloadConfig(TIM8, ENABLE);	
		
		//Ӧ�����õ�TIM2 
  	TIM_TimeBaseInit(TIM8, &tim);
		
		// ʹ��TIM2���ؼĴ���ARR
  	TIM_ARRPreloadConfig(TIM8, ENABLE);	
	  TIM_PrescalerConfig(TIM8, 0, TIM_PSCReloadMode_Update);
		
		/* Disable the TIM2 Update event */
		TIM_UpdateDisableConfig(TIM8, ENABLE);
	
	  TIM_Cmd(TIM8,ENABLE);	   
}



void TIM6_Configuration(void)
{
			// todo ���ⶨʱ�����壬���Ĵ���ʵ�ֿ���LED��ÿ����˸1��
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    nvic.NVIC_IRQChannel  = TIM6_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
	
	
		tim.TIM_Period = 0xFFFFFFFF; //�����Զ���������
    tim.TIM_Prescaler = 168-1;      //����Ԥ��Ƶֵ  
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;   //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM6,&tim);//ʹ��TIM6���ؼĴ���ARR 

    //TIM_Cmd(TIM6,ENABLE);
	
}

void TIM6_Start(void)
{
    TIM_Cmd(TIM6, ENABLE);	 
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);//ʹ��TIM6�����ж�
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
	  //todo change the led 
		GPIO_ResetBits( GPIOB , GPIO_Pin_9 );
}

void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);    
    tim.TIM_Period = 0xFFFFFFFF;     
    tim.TIM_Prescaler = 168-1;	 //1M ��ʱ��  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_ARRPreloadConfig(TIM4, ENABLE);	//����TIM4_CR1�Ĵ�����ʹ���Զ���װ�ص�Ԥװ�ؼĴ���
    //Ӧ�����õ�TIM2 
    TIM_TimeBaseInit(TIM4, &tim);
    // ʹ��TIM2���ؼĴ���ARR
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
    tim.TIM_Prescaler = 84 - 1;	 //1M ��ʱ��  
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
				TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//����жϱ�־
        TIM_ClearFlag(TIM6, TIM_FLAG_Update);
				Control_Task();         //���̡���̨��������
			 //todo �ð���led�Ƶ�״̬������ת ������ ��䰵
				GPIO_ToggleBits( GPIOB , GPIO_Pin_9 );
    }
}

int32_t ms_count = 0;
uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}

