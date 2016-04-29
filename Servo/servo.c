#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

/* For 50 Hz PWM signal Prescaler and Period Values */
uint16_t PrescalerValue = 84;
uint32_t PeriodValue = 20000;

/*Timer 3 PE5*/
//void ServoPin_PWM_Output_Config() {
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//  TIM_OCInitTypeDef  TIM_OCInitStructure;

//	/* TIM3 clock enable */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

//	/* GPIOC clock enable */
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//  GPIO_Init(GPIOE, &GPIO_InitStructure);

//  /* Connect TIM3 pins to AF2 */
//  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9);

//  /******************************************************************/

//  /* Time base configuration */
//  TIM_TimeBaseStructure.TIM_Period = PeriodValue - 1;
//  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue - 1;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure);

//  /* PWM1 Mode configuration */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

//  /* PWM1 Mode configuration: Channel1 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 0;

//  TIM_OC1Init(TIM9, &TIM_OCInitStructure);
//  TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);


//  TIM_ARRPreloadConfig(TIM9, ENABLE);

//  /* TIM3 enable counter */
//  TIM_Cmd(TIM9, ENABLE);
//}
void ServoPin_Delay_Config() {

	GPIO_InitTypeDef GPIO_InitStruct_Servo;
	  
	GPIO_InitStruct_Servo.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct_Servo.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct_Servo.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct_Servo.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct_Servo.GPIO_Speed = GPIO_Speed_100MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		
	GPIO_Init(GPIOE, &GPIO_InitStruct_Servo);
}
