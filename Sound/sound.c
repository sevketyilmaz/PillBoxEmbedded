#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "sound.h"

void Sound_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//uint16_t  _250Hz = 1279, _500Hz = 639, _1kHz = 319, _2kHz = 159, _4kHz = 79, _8kHz = 39;
	
	//uint16_t CCR1_Val = 35000;
	uint16_t PrescalerValue = 0;
	
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/* GPIOB clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
		/* TIM10 clock enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);

		
		/* GPIOF Configuration: TIM10 CH1 (PB8) */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 

		/* Connect TIM pins to AF3 */
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);
	
	/* ---------------------------------------------------------------------------
    TIM10 Configuration: generate 1 PWM signal:
    
    In this example TIM10 input clock (TIM10CLK) is set to 2 * APB2 clock (PCLK2), 
    since APB2 prescaler is different from 1.   
      TIM10CLK = 2 * PCLK2  
      PCLK2 = HCLK / 2 
      => TIM10CLK = HCLK = SystemCoreClock
          
    To get TIM10 counter clock at 21 MHz, the prescaler is computed as follows:
       Prescaler = (TIM10CLK / TIM10 counter clock) - 1
       Prescaler = (SystemCoreClock /21 MHz) - 1
                                              
    To get TIM10 output clock at 31.530 KHz, the period (TIM10_ARR) is computed as follows:
       ARR = (TIM10 counter clock / TIM10 output clock) - 1
           = 665
                  
    TIM10 Channel1 duty cycle = (TIM10_CCR1/ TIM10_ARR)* 100 = 50%

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  --------------------------------------------------------------------------- */   


  /* Compute the prescaler value *///(320kHz)
  PrescalerValue = (uint16_t) (SystemCoreClock / 45000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535;//ARR Value
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
  TIM_OCInitStructure.TIM_Pulse = 0;
  
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM10, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM10, ENABLE);

  /* TIM10 enable counter */
  TIM_Cmd(TIM10, ENABLE);
}
