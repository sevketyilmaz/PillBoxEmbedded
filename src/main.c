/*Motor Switch

SW1 -> PD12
SW2 -> PD13
*/

/*
PA0 -> Wake-Up Button
NRST
*/

/*Switches 
PA3 --- PA7 5
PC4-PC5 2
PB0-PB1 2
PE7 ----  PE15 9
PB10 ---- PB15 6
PD8 ---- PD11 4
*/

/*Sound(Buzzer)
PA8
*/

/*Servo
PC6 (PC9*****)
*/

/*LED
LED1,2,3,4 -> PC8,PC7,PC6,PD15
*/

/*LIPO ADC 
PE6
*/

/*I2C
SCL -> PB8
SDA -> PB9
*/

/*Usart
PB6 -> TX
PB7 -> RX
PB5 -> RESET
*/

/*MOTOR
PE7 -> PA9 -> ULN2003
PE8 -> PA10
PE9 -> PA11
PE10 -> PA12

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
#include "servo.h"

#include "String.h"
#include "Jansson.h"

uint8_t read_byte;
uint8_t write_byte=255;
uint8_t status=0;
int start = 0;
int adim = 0;
unsigned int enable_servo=0;

struct tm t;
time_t t_of_day, alrmtime,currentime;
	
int k;

unsigned int aci=0;
unsigned int sifirKonumu=1000;
unsigned int pwm=0;
	
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
	ServoPin_PWM_Output_Config();
	motor_switch_pin_init();
	box_switch_pins_init();
  box_pins_init();
	
  //How to write and read data from EEPROM
	I2C_writeBytes(0x50<<1, 0, 1, &write_byte);		
	I2C_readBytes(0x50<<1, 0, 1, &read_byte);	
	if(read_byte == 255)
		//STM_EVAL_LEDOn(LED6);
	
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
	
	printf("All Alarm Disable....\n");
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	TIM1->CCR1 = 0;
	
	aci = 0;
	pwm=(aci*20)+sifirKonumu; //pwm 500-550 iken 0 derece konumunda bekler
	TIM_SetCompare1(TIM3,pwm); //pwm'i guncelle
	delay_ms(20); //acinin her degismesinde 10ms bekler
	TIM_SetCompare1(TIM3,0); //pwm'i guncelle
	aci=10;
  pwm=(aci*20)+sifirKonumu;
  TIM_SetCompare1(TIM3,pwm);
  delay_ms(2000);
  sifirKonumu = 400;
	pwm=(aci*20)+sifirKonumu; //pwm 500-550 iken 0 derece konumunda bekler
	TIM_SetCompare1(TIM3,pwm); //pwm'i guncelle
	delay_ms(20); //acinin her degismesinde 10ms bekler
	TIM_SetCompare1(TIM3,0); //pwm'i guncelle	
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)){
		stepper();
		delay_us(3300);
	}
	
	for(k=0;k<5;k++){
		readAlarmtoEEPROM(k);
	}
	
	for(k=0; k<5; k++){
		//YAnlislik var!!!!!!!!!!!!!!!!!!!!!!!!!!!***************************
		status = boxes.pillbox[k].alarm_ok;//
		//double difftime(time_t time1, time_t time0);//time1 - time0
		alrmtime = boxes.pillbox[k].alarmTime;
		currentime = get_current_time_ms();
		remaining_alarmtime = difftime(alrmtime, currentime);			//alarmtime - currenttime
		printf("Remaining alarm time%d: %d\n",k, remaining_alarmtime);
	  printf("%s \n", ctime(&currentime));


		if(status == 0 & remaining_alarmtime>0){
			which_alarm_created = k; 
			//create_one_alarm(boxes.pillbox[k].alarmTime);
			create_one_alarm_in_ms(boxes.pillbox[k].alarmTime);

			next_box = boxes.pillbox[k].box_number;
			go_to_box(current_box,next_box);
			break;
		}
		else{
			
			if(status == 0){
				set_Alarm_Ok_Flag(k, 1);
			
				if(k>0)
					current_box = next_box;
			
				next_box = boxes.pillbox[k].box_number;
				go_to_box(current_box,next_box);
				open_box();
			}	
		}
	}
	
  STM_EVAL_LEDOn(LED3);

	/*Box Gidis Testi*/
	/*
	delay_ms(2000);
	
	open_box();

	go_to_box(0,1);
	open_box();
	
	go_to_box(1,2);
	open_box();
	
	go_to_box(2,3);
	open_box();
	
	go_to_box(3,4);
	open_box();
	
	go_to_box(4,5);
	open_box();
	
	go_to_box(5,6);
	open_box();
	
	go_to_box(6,7);
	open_box();
	
	go_to_box(7,8);
	open_box();
	
	delay_ms(2000);
	*/

	while (1) {
		
		if(read_boxes == 1){
			read_boxes_state();
			read_boxes = 0;
		}
		/*Motor Switches*/
		/*
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0){
			STM_EVAL_LEDOn(LED4);
			Direction = 1;//right
		}
		else{
			STM_EVAL_LEDOff(LED4);
		}
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){
			STM_EVAL_LEDOn(LED5);
			Direction = 0;//left//sonu
			adim = 0;
		}
		else{
			STM_EVAL_LEDOff(LED5);
		}
		if(Motor_Status == 1) {
			stepper();
			delay_us(3300);
			adim++;
		}	
		*/
		
		/*
		//SWITCHES//
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 1){			
			STM_EVAL_LEDOn(LED3);
		}
		else{		
			STM_EVAL_LEDOff(LED3);
		}		
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == 1){			
			STM_EVAL_LEDOn(LED4);
		}
		else{			
			STM_EVAL_LEDOff(LED4);
		}	
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4) == 1){			
			STM_EVAL_LEDOn(LED5);
		}
		else{		
			STM_EVAL_LEDOff(LED5);
		}
		
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 1){			
			STM_EVAL_LEDOn(LED6);
		}
		else{		
			STM_EVAL_LEDOff(LED6);
		}
		*/
		
		
		/*User Button*/
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))	{
			sendTime();
			for(k=0;k<5;k++){
				readAlarmtoEEPROM(k);
			}
		}
		
		/*Flags*/
		if(flag_usart == 1){
			usart_interrup_main();
		}		
		
		if(rtc_alarm_irqhandler_flag == 1){
		
			rtc_alarm_irqhandler_flag = 0;
			
			STM_EVAL_LEDOff(LED5);
			open_box();

			boxes.pillbox[which_alarm_created].alarm_ok = 1;
			set_Alarm_Ok_Flag(which_alarm_created,1);
			
			current_box = boxes.pillbox[which_alarm_created].box_number;
			
			alarmok = 1;
			alarmtimecounter = 60;
			TIM1->CCR1 = 20;
			
			set_Alarm_Ok_Flag(which_alarm_created, 1);
			
			which_alarm_created++;
			
			next_box = boxes.pillbox[which_alarm_created].box_number;

			
			if(which_alarm_created<5){
				//create_one_alarm(boxes.pillbox[which_alarm_created].alarmTime);
				create_one_alarm_in_ms(boxes.pillbox[which_alarm_created].alarmTime);
				go_to_box(current_box,next_box);
			}	
		}
		
		if(which_alarm_created == 5) {
			
			current_box=0;
			next_box=0;
			Direction = 1;
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)){
				stepper();
				delay_us(3300);
			}
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
