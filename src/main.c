/*

Sound(Buzzer) -> PA8

*/

/*

SCL -> PB8
SDA -> PB9

*/

/*

PB6 -> TX
PB7 -> RX

*/

/*
MOTOR
PE7
PE8
PE9
PE10

4 pin Stepper Motor

---------------------------------------
STEPS | STEP1 | STEP2 | STEP3 | STEP4 |
---------------------------------------
PE7   |   1   |   0   |   0   |   0   |
---------------------------------------
PE8   |   0   |   0   |   1   |   0   |
---------------------------------------
PE9   |   0   |   1   |   0   |   0   |
---------------------------------------
PE10  |   0   |   0   |   0   |   1   |
---------------------------------------

5 pin Stepper Motor

---------------------------------------
STEPS | STEP1 | STEP2 | STEP3 | STEP4 |
---------------------------------------
PE7   |   0   |   0   |   1   |   1   |
---------------------------------------
PE8   |   1   |   1   |   0   |   0   |
---------------------------------------
PE9   |   1   |   0   |   0   |   1   |
---------------------------------------
PE10  |   0   |   1   |   1   |   0   |
---------------------------------------
*/

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "main.h"

#include <stdio.h>
#include <time.h>

#include "delay.h"
#include "usart.h"
#include "eeprom.h"
#include "i2c.h"
#include "keypad.h"
#include "pillbox.h"
#include "motor.h"
#include "rtc.h"
#include "vbat.h"
#include "sound.h"

#include "String.h"
#include "Jansson.h"

uint8_t read_byte;
uint8_t write_byte=255;

struct tm t;
time_t t_of_day;

int k;

void button_init(void);
int main(void) {
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);
	
	delay_init();
	Init_USART1();
	rtc_init();
	rtc_alarm_init();
	Sound_Init();
	motor_pin_init();
	solenoid_pin_init();
	Init_ADCVbat();
	ADC_SoftwareStartConv(ADC1);
	Init_I2C();
	button_init();
	

  //How to write and read data from EEPROM
	I2C_writeBytes(0x50<<1, 0, 1, &write_byte);		
	I2C_readBytes(0x50<<1, 0, 1, &read_byte);	
	if(read_byte == 255)
		STM_EVAL_LEDOn(LED6);
	
	/*
	//How to get milisecond from time
	t.tm_year = 2015-1900;
  t.tm_mon = 9;           // Month, 0 - jan
  t.tm_mday = 13;          // Day of the month
  t.tm_hour = 12-3;     
  t.tm_min = 01;
  t.tm_sec = 02;
  t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
	t_of_day = mktime(&t);
  printf("seconds since the Epoch: %ld\n", (long) t_of_day);
	*/
	
	printf("Initilazation finish....\n");
	
	STM_EVAL_LEDOn(LED3);

	TIM1->CCR1 = 0;
	
	while (1) {
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))	{
			sendTime();
			for(k=0;k<5;k++){
				readAlarmtoEEPROM(k);
			}
		}
				
		if(flag_usart == 1){
			usart_interrup_main();
		}		
		
	}
}

void button_init() {
	  GPIO_InitTypeDef GPIO_InitStruct_Button;
	
	  GPIO_InitStruct_Button.GPIO_Pin = GPIO_Pin_0 ;
		GPIO_InitStruct_Button.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStruct_Button.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct_Button.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct_Button.GPIO_Speed = GPIO_Speed_100MHz;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	  GPIO_Init(GPIOA, &GPIO_InitStruct_Button);
}
