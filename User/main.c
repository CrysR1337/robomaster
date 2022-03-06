#include "main.h"
uint32_t Upload_Speed = 1;   //数据上传速度  单位 Hz
#define upload_time (1000000/Upload_Speed)  //计算上传的时间。单位为us
uint16_t  power1=0,power2=0;
uint32_t system_micrsecond;   //系统时间 单位ms
int main(void)
{     			
  //CAN1_Configuration();                    
	//USART1_Configuration(100000);
  Led_Configuration();	
	//USART3_Configuration();     			
	while(1)
	{   
		delay_ms(500);
		//GPIO_ResetBits( GPIOB , GPIO_Pin_9 );
		GPIO_ToggleBits( GPIOB , GPIO_Pin_9 );
//	  char temp[50];
//		sprintf(temp,"%d",32);	
//   	UART3_PutStr (temp);//发送字符串
  }
	
	Led_Configuration();	
	TIM6_Configuration();
	TIM6_Start();
	while(1)
	{
		TIM6_DAC_IRQHandler();//???
	}
	
	u8 i=1;
	u16 led0pwmval;//CCR值
	Led_Configuration();
    PWM_Configuration();
    while(1)
    {    
        delay_ms(10);            
        if(i)led0pwmval++;
            else led0pwmval--;
        
        if(led0pwmval==0)i=1;
        if(led0pwmval>100)i=0;
        
        TIM_SetCompare2(TIM3,led0pwmval);        //调节占空比
        
    }
}
