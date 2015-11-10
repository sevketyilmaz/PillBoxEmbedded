#include "stm32f4xx.h"
#include "keypad.h"


void keypad_init() {
		GPIO_InitTypeDef GPIO_InitStruct_Keypad_Column;
		GPIO_InitTypeDef GPIO_InitStruct_Keypad_Row;

	  GPIO_InitStruct_Keypad_Column.GPIO_Pin = COLUMN_1_PIN | COLUMN_2_PIN | COLUMN_3_PIN | COLUMN_4_PIN;
		GPIO_InitStruct_Keypad_Column.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct_Keypad_Column.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct_Keypad_Column.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct_Keypad_Column.GPIO_Speed = GPIO_Speed_100MHz;
	 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		
	  GPIO_Init(GPIOD, &GPIO_InitStruct_Keypad_Column);
	
		GPIO_InitStruct_Keypad_Row.GPIO_Pin = ROW_1_PIN | ROW_2_PIN | ROW_3_PIN | ROW_4_PIN;
		GPIO_InitStruct_Keypad_Row.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct_Keypad_Row.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct_Keypad_Row.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct_Keypad_Row.GPIO_Speed = GPIO_Speed_100MHz;
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		
	  GPIO_Init(GPIOC, &GPIO_InitStruct_Keypad_Row);
}
