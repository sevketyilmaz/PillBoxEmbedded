#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_usart.h"

#define MAX_STRLEN 2048 

extern uint8_t flag_usart ;
extern char received_string[MAX_STRLEN]; 

void Init_USART1(void);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);

#endif /* __USART_H */
