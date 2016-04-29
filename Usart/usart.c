/*

PB6 -> TX
PB7 -> RX

*/

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include <stdio.h>
#include <string.h>

#include "usart.h"
#include "pillbox.h"
#include "eeprom.h"
#include "delay.h"

#include <stdlib.h>
#include <time.h>

uint8_t flag_usart;
char received_string[MAX_STRLEN]; 

int cnt = 0;
int i=0;

char dies[1] = "#";
char star[1] = "*";

void Init_USART1(void){
	
	GPIO_InitTypeDef GPIO_InitStruct; 
	USART_InitTypeDef USART_InitStruct; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) , 7 (RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			
	GPIO_Init(GPIOC, &GPIO_InitStruct);					
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	USART_InitStruct.USART_BaudRate = 9600;				
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;	
	USART_InitStruct.USART_Parity = USART_Parity_No;		
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_Init(USART6, &USART_InitStruct);					
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;		 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 
	NVIC_Init(&NVIC_InitStructure);						

	USART_Cmd(USART6, ENABLE);
}

void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) ); 
		USART_SendData(USARTx, *s);
		*s++;
	}
}

void USART6_IRQHandler(void){
	
	if( USART_GetITStatus(USART6, USART_IT_RXNE) ){
		
		char t = USART6->DR; 
		
		if(cnt == 0) {			
			memset(&received_string, 0, sizeof(received_string));
		}
		
		if( (t != '\n') && (cnt < MAX_STRLEN)){ 			
			
			received_string[cnt] = t;
			cnt++;
		}
		
		else { 
			
			cnt = 0;
			flag_usart=1;			
		}
	}
}
