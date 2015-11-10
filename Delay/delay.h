#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

void delay_init(void);
void delay_cmd(FunctionalState state);

void TimingDelay_Decrement(void);

void delay_ms(__IO uint32_t nTime);
void delay_us(__IO uint32_t nTime);

#endif /* __DELAY_H */
