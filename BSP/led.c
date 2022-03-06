#include "main.h"



void Led_Configuration(void)
{
  GPIO_InitTypeDef gpio;
    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //开启PCLK，外设时钟
	
	gpio.GPIO_Pin = GPIO_Pin_9 ;
	gpio.GPIO_Mode = GPIO_Mode_OUT ;
	gpio.GPIO_OType = GPIO_OType_PP ;//通用推免
	gpio.GPIO_Speed = GPIO_Speed_100MHz ;
	GPIO_Init(GPIOB,&gpio);//配置数据
    
	GPIO_SetBits( GPIOB , GPIO_Pin_9 );

}
