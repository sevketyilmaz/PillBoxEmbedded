//PA8

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "sound.h"

void Sound_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint16_t Period = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//RCC Configuration
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

	//GPIO Configuration
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	//Connect TIM pins to AF
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM13);
	
	//TIM1 Configuration: Generate 1 PWM signal:

	Period = 1000000 / 200; // 20 KHz for 1MHz prescaled
	
  TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // Get clock to 1 MHz
  TIM_TimeBaseStructure.TIM_Period = Period-1;//ARR Value
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Period/10;		//%50
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM13, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);

  //TIM_ARRPreloadConfig(TIM1, ENABLE);
	
  TIM_CtrlPWMOutputs(TIM13, ENABLE);
  TIM_Cmd(TIM13, ENABLE);
}
