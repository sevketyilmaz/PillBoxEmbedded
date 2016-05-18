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
PC0
PA2
PE4
PE6

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
unsigned int sifirKonumu=400;
unsigned int pwm=0;
	
void button_init(void);
int main(void) {
	
	motor_pin_init();
	stopMotor();
	
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	
	delay_init();
	Init_USART1();
	Init_I2C();
	rtc_init();
	rtc_alarm_init();
	Sound_Init();

	ServoPin_Delay_Config();
	GPIO_ResetBits(GPIOE, GPIO_Pin_5);
	motor_switch_pin_init();
	Sound_Init();
	Init_ADCVbat();
	ADC_SoftwareStartConv(ADC1);
	button_init();
  box_pins_init();
	//bluetooth_status_pins_init();
	
  //How to write and read data from EEPROM
	I2C_writeBytes(0x50<<1, 0, 1, &write_byte);		
	I2C_readBytes(0x50<<1, 0, 1, &read_byte);	
	if(read_byte == 255)
		STM_EVAL_LEDOn(LED4);
	
	
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
	
	
	//printf("Initilazation finish....\n");
	//printf("All Alarm Disable....\n");
	//RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	
	//Servo Correction
	for(k=0; k<63; k++){ //middle
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		delay_us(1500);
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		delay_us(18500);
	}
	
	/*
	for(k=0; k<50; k++){ //right
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		delay_us(500);
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		delay_us(19500);
	}
	
	
	for(k=0; k<155; k++){ //left
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		delay_us(2500);
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		delay_us(17500);
	}
	*/
	Direction = 1;
	//First Initilazation
	while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
		stepper();
		delay_us(3000);
	}
	stopMotor();

	for(k=0;k<21;k++){
		readAlarmtoEEPROM(k);
	}
	
	for(k=0; k<21; k++){
		status = boxes.pillbox[k].alarm_ok;//
		alrmtime = boxes.pillbox[k].alarmTime;
		currentime = get_current_time_ms();
		remaining_alarmtime = difftime(alrmtime, currentime);			//alarmtime - currenttime
		printf("Remaining alarm time%d: %d\n",k, remaining_alarmtime);
	  printf("%s \n", ctime(&currentime));
		if(alrmtime == 0)
			break;
		if(status == 0 & remaining_alarmtime>0){
			STM_EVAL_LEDToggle(LED3);
			which_alarm_created = k; 
			create_one_alarm_in_ms(boxes.pillbox[k].alarmTime);
			set_Box_State_Flag(k,CLOSE_FULL);
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
	
	/*Box Gidis Testi*/
	/*
	delay_ms(2000);
	
	open_box();
	delay_ms(2000);

	go_to_box(0,1);
	open_box();
	delay_ms(2000);
	
	go_to_box(1,2);
	open_box();
	delay_ms(2000);
	
	go_to_box(2,3);
	open_box();
	delay_ms(2000);
	
	go_to_box(3,4);
	open_box();
	delay_ms(2000);
	
	go_to_box(4,5);
	open_box();
	delay_ms(2000);
	
	go_to_box(5,6);
	open_box();
	delay_ms(2000);
	
	go_to_box(6,7);
	open_box();
	delay_ms(2000);
	
	go_to_box(7,8);
	open_box();
	delay_ms(2000);
	
	go_to_box(8,9);
	open_box();
	delay_ms(2000);
	
	go_to_box(9,10);
	open_box();
	delay_ms(2000);
	
	go_to_box(10,11);
	open_box();
	delay_ms(2000);
	
	go_to_box(11,12);
	open_box();
	delay_ms(2000);
	
	go_to_box(12,13);
	open_box();
	delay_ms(2000);
	
	go_to_box(13,14);
	open_box();
	delay_ms(2000);
	
	go_to_box(14,15);
	open_box();
	delay_ms(2000);
	
	go_to_box(15,16);
	open_box();
	delay_ms(2000);
	
	go_to_box(16,17);
	open_box();
	delay_ms(2000);
	
	go_to_box(17,18);
	open_box();
		delay_ms(2000);

	go_to_box(18,19);
	open_box();
	
	delay_ms(2000);

	go_to_box(19,20);
	open_box();
	
	delay_ms(2000);
	*/

	while (1) {
		
		/*Card Test*/
		//STM_EVAL_LEDToggle(LED3);
		//STM_EVAL_LEDToggle(LED4);
		//delay_ms(100);
		
		
		/*MOTOR CONTROL*/
		/*
		if(Motor_Status == 1) {
			stepper();
			delay_us(2500);//3300
		}	
		else{	
			stopMotor();
		}
		*/
		/*BOX SWITCHES CONTROL*/
		/*
		if(GPIO_ReadInputDataBit(Box15_Port, Box15) == 1){			
			STM_EVAL_LEDOn(LED3);
		}
		else{		
			STM_EVAL_LEDOff(LED3);
		}		
		if(GPIO_ReadInputDataBit(Box14_Port, Box14) == 1){			
			STM_EVAL_LEDOn(LED4);
		}
		else{			
			STM_EVAL_LEDOff(LED4);
		}	
		if(GPIO_ReadInputDataBit(Box13_Port, Box13) == 1){			
			STM_EVAL_LEDOn(LED5);
		}
		else{		
			STM_EVAL_LEDOff(LED5);
		}	
		*/

			/*MOTOR SWITCHES CONTROL*/
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1){			
//			STM_EVAL_LEDOn(LED5);
//		}
//		else{		
//			STM_EVAL_LEDOff(LED5);
//		}		
//		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 1){			
//			STM_EVAL_LEDOn(LED6);
//		}
//		else{			
//			STM_EVAL_LEDOff(LED6);
//		}	
//-------------------------------------------------------------------------------------------------------------

		/*Step Count*/
		/*
		if(Motor_Status == 1) {
			stepper();
			delay_us(3300);
			adim++;
		}	
		*/
				
		if(read_boxes == 1){
			read_boxes_state(); 
		  read_boxes = 0;
			readEEPROMtosend();
		}
		
		/*User Button*/
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0)	{
			sendTime();
		}
		
		/*Flags*/
		if(flag_usart == 1){
			usart_interrup_main();
		}		
		
		
		if(rtc_alarm_irqhandler_flag == 1){
		
			rtc_alarm_irqhandler_flag = 0;
			
			open_box();

			boxes.pillbox[which_alarm_created].alarm_ok = 1;
			set_Alarm_Ok_Flag(which_alarm_created,1);
			
			current_box = boxes.pillbox[which_alarm_created].box_number;
			
			alarmok = 1;
			alarmtimecounter = 10;
			playMusic();
			
			set_Alarm_Ok_Flag(which_alarm_created, 1);
			
			which_alarm_created++;
			
			next_box = boxes.pillbox[which_alarm_created].box_number;

			
			if(which_alarm_created<21){
				if(boxes.pillbox[which_alarm_created].alarmTime != 0) {			
					create_one_alarm_in_ms(boxes.pillbox[which_alarm_created].alarmTime);
					STM_EVAL_LEDOn(LED3);
					set_Box_State_Flag(which_alarm_created,CLOSE_FULL);					
					go_to_box(current_box,next_box);
				}	
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
