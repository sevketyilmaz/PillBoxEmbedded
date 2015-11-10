#include "delay.h"
 
static __IO uint32_t uwTimingDelay;
			   
void delay_init(){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	while(SysTick_Config(SystemCoreClock / 1000000) != 0); 
	delay_cmd(DISABLE);
}								    

void delay_cmd(FunctionalState state)
{
	if(state == ENABLE){
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	}
	else{
		SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
	}
}

void delay_us(__IO uint32_t nTime){
	delay_cmd(ENABLE);
  uwTimingDelay = nTime;
  while(uwTimingDelay != 0);
	delay_cmd(DISABLE);
}

void delay_ms(__IO uint32_t nTime){
	delay_cmd(ENABLE);
  uwTimingDelay = nTime*1000;
  while(uwTimingDelay != 0);
	delay_cmd(DISABLE);
}

void TimingDelay_Decrement(void){
	
  if (uwTimingDelay != 0){ 
    uwTimingDelay--;
  }
}
