#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_rtc.h"
#include "eeprom.h"

#include "pillbox.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pillbox.h"
#include "usart.h"
#include "delay.h"
#include "i2c.h"
#include "rtc.h"
#include "motor.h"

#include "String.h"
#include "Jansson.h"

char send_message[1024];
int l=0;

struct Boxes boxes;
uint8_t which_alarm_created;

int remaining_alarmtime;

time_t alarmtime;


char *token;
char *(buffer[40]);
char copy1[40][40];
int token_size=0;
int x,c;
uint32_t subsecond;
int message,led_status,current_time,bN,bS,aT,motor_direction,motor_onoff;

json_t *root, *message_obj, *led_obj, *current_time_obj, *bN_obj, *bS_obj, *aT_obj, *motor_obj, *motor_direction_obj;
json_error_t error;

int next_box=0, current_box=0;


unsigned int acix=0;
unsigned int sifirKonumux=1100;
unsigned int pwmx=0;

GPIO_TypeDef* BOX_PORT[21] = {Box1_Port, Box2_Port, Box3_Port, Box4_Port, Box5_Port, Box6_Port, Box7_Port, 
																	 Box8_Port, Box9_Port, Box10_Port, Box11_Port, Box12_Port, Box13_Port, Box14_Port,
																	 Box15_Port, Box16_Port, Box17_Port, Box18_Port, Box19_Port, Box20_Port, Box21_Port}; 
const uint16_t BOX_PIN[21] = {Box1, Box2, Box3, Box4, Box5, Box6, Box7,
															Box8, Box9, Box10, Box11, Box12, Box13, Box14, 
															Box15, Box16, Box17, Box18, Box19, Box20, Box21}; 

void box_pins_init(){
	
	GPIO_InitTypeDef GPIO_InitStruct;
	  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 |GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//---------------
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);		
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_1 |GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void bluetooth_status_pins_init(){
	GPIO_InitTypeDef GPIO_InitStruct;
	  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void read_boxes_state(){
	int i = 0;
	for(i=0; i<21; i++){
		
		if(GPIO_ReadInputDataBit(BOX_PORT[boxes.pillbox[i].box_number], BOX_PIN[boxes.pillbox[i].box_number]) == 1){
			set_Box_State_Flag(i, OPEN_FULL);
		}
		else{
			set_Box_State_Flag(i, CLOSE_DONE);
		}
	}
}

void createAlarm(uint8_t hour, uint8_t minutes, uint8_t dayOfWeek) {
	
				RTC_AlarmTypeDef RTC_AlarmStructure;
				RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
				
			  RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;
				RTC_AlarmStructure.RTC_AlarmDateWeekDay = dayOfWeek;
				RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	
				RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_HourFormat_24;
				RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = hour;
				RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = minutes;
				RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0;
				RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
				
				RTC_ITConfig(RTC_IT_ALRA, ENABLE);
				RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
};
/*
uint8_t getVariable(char variable[50]){
			  
	      uint8_t hour, minute, dayOfWeek, creatingorder, boxnumbers;
	
				boxnumbers = (variable[1]-48);
        hour = (variable[3]-48)*10 + (variable[4]-48);
				minute = (variable[6]-48)*10 + (variable[7]-48);
				dayOfWeek = (variable[9]-48);
				creatingorder = (variable[9]-48);
	
				//if(boxnumbers == 1) {
										
					//boxes.pillbox[boxnumbers-1].alarm_time.hour = hour;
					//boxes.pillbox[boxnumbers-1].alarm_time.minute = minute;
					//boxes.pillbox[boxnumbers].alarm_time.dayofweek = dayOfWeek;
					
					//boxes.pillbox[boxnumbers-1].alarm_created = 0;
					//boxes.pillbox[boxnumbers-1].alarm_ok = 0;
					//boxes.pillbox[boxnumbers-1].box_open_close = 1;
					//boxes.pillbox[boxnumbers-1].alarm_creating_order = creatingorder;
				//}
				return boxnumbers;
};
*/
void writeAlarmtoEEPROM(uint8_t boxnumbers){
		uint8_t byte_size = 8; // Number of byte for each boxes
		uint8_t start_address;
		//uint8_t end_address, length; 
		
		start_address = 1 + byte_size * boxnumbers;
		//end_address =  byte_size * (boxnumbers + 1);
		//length = end_address-start_address + 1;
	
	  eeprom_write_long(0x50<<1, start_address,   boxes.pillbox[boxnumbers].alarmTime);
		eeprom_write_byte(0x50<<1, start_address+4, boxes.pillbox[boxnumbers].box_number);		
		eeprom_write_byte(0x50<<1, start_address+5, boxes.pillbox[boxnumbers].box_state);		
		eeprom_write_byte(0x50<<1, start_address+6,	boxes.pillbox[boxnumbers].alarm_created);		
		eeprom_write_byte(0x50<<1, start_address+7,	boxes.pillbox[boxnumbers].alarm_ok);		
}

void set_Alarm_Ok_Flag(uint8_t boxnumbers, uint8_t alarm_ok){
		uint8_t byte_size = 8; // Number of byte for each boxes
		uint8_t start_address;
		//uint8_t end_address, length; 
		
		start_address = 1 + byte_size * boxnumbers;
		//end_address =  byte_size * (boxnumbers + 1);
		//length = end_address-start_address + 1;
		boxes.pillbox[boxnumbers].alarm_ok = alarm_ok;	
		eeprom_write_byte(0x50<<1, start_address+7,	alarm_ok);		
}

uint8_t read_Alarm_Ok_Flag(uint8_t boxnumbers){
		uint8_t byte_size = 8; // Number of byte for each boxes
		uint8_t start_address;
		//uint8_t end_address, length; 
		uint8_t flag;
		start_address = 1 + byte_size * boxnumbers;
		//end_address =  byte_size * (boxnumbers + 1);
		//length = end_address-start_address + 1;
		
		flag = eeprom_read_byte(0x50<<1, start_address+7);
		return flag;
}

void set_Box_State_Flag(uint8_t boxnumbers, uint8_t box_state){
		uint8_t byte_size = 8; // Number of byte for each boxes
		uint8_t start_address;
		//uint8_t end_address, length; 
		if(boxes.pillbox[boxnumbers].box_number != 255) {
			start_address = 1 + byte_size * boxnumbers;
			boxes.pillbox[boxnumbers].box_state = box_state;	
			eeprom_write_byte(0x50<<1, start_address+5,	box_state);		
		}
}

uint8_t read_Box_State_Flag(uint8_t boxnumbers){
		uint8_t byte_size = 8; // Number of byte for each boxes
		uint8_t start_address;
		uint8_t flag;
	
		start_address = 1 + byte_size * boxnumbers;
		flag = eeprom_read_byte(0x50<<1, start_address+5);
		return flag;
}

void readAlarmtoEEPROM(uint8_t boxnumbers){
		
		int read_data;

		uint8_t byte_size = 8;
		uint8_t start_address;
  	uint8_t flag;
	
		start_address = 1 + byte_size * boxnumbers;
	
		flag = eeprom_read_byte(0x50<<1, start_address+4);//box_numbers	
		//if(flag != 255) {
			
			printf("  | %d | ", flag);
			boxes.pillbox[boxnumbers].box_number  = flag;
	
			eeprom_read_long(0x50<<1, start_address, &read_data);
			boxes.pillbox[boxnumbers].alarmTime = read_data;
			alarmtime = read_data;
			printf("Read Alarm %d: %s ", boxnumbers, ctime(&alarmtime));
	

			flag = eeprom_read_byte(0x50<<1, start_address+5);//box_state
			printf("  | %d | ", flag);
			boxes.pillbox[boxnumbers].box_state  = flag;

			flag = eeprom_read_byte(0x50<<1, start_address+6);	
			printf("  | %d | ", flag);
			boxes.pillbox[boxnumbers].alarm_created = flag;

			flag = eeprom_read_byte(0x50<<1, start_address+7);//alarm_ok	
			printf("  | %d |\n", flag);
			boxes.pillbox[boxnumbers].alarm_ok = flag;
		//}	
}

void readEEPROMtosend(){
		uint8_t esn;// Eeprom State Number
		uint8_t byte_size = 8;
		uint8_t start_address;
  	uint8_t box_number, box_state;
		
		sprintf(send_message, "[{\"mS\":%d, \"cT\":%d},", 1,2);
		USART_puts(USART6, send_message);
		
	for(esn=0; esn<21; esn++){
			start_address = 1 + byte_size * esn;
			
			box_number = eeprom_read_byte(0x50<<1, start_address+4);//box_number
			box_state = eeprom_read_byte(0x50<<1, start_address+5);//box_state
		
			if(esn == 20) {
				sprintf(send_message, "{\"bN\":%d, \"bs\":%d}]\n",  box_number, box_state);
				USART_puts(USART6, send_message);
			}
			else{
				sprintf(send_message, "{\"bN\":%d, \"bs\":%d},", box_number, box_state);
				USART_puts(USART6, send_message);
			}
	}		
}

void create_one_alarm(time_t alarm_time){
	//int alarm_date, alarm_month, alarm_year, alarm_seconds;
	int	alarm_hours, alarm_minutes, alarm_dayofweek;
	struct tm *alarm;
			
	alarm = localtime(&alarm_time);
				
	//alarm_date = (alarm->tm_mday);
	//alarm_month = (alarm->tm_mon)+1;
	//alarm_year = (alarm->tm_year)+1900;
				
	alarm_dayofweek = (alarm->tm_wday);
	if(alarm_dayofweek == 0)
		alarm_dayofweek = 7;
				
	alarm_hours = (alarm->tm_hour);
	alarm_minutes = (alarm->tm_min);
	//alarm_seconds = (alarm->tm_sec);
				
	createAlarm(alarm_hours ,alarm_minutes, alarm_dayofweek);
	
	//RTC_AlarmSubSecondConfig(RTC_Alarm_a, subsecond, RTC_AlarmSubSecondMask_None);
	//subsecond = RTC_GetAlarmSubSecond(RTC_Alarm_A);
	//printf("Sub Second: %d\n", subsecond);

	STM_EVAL_LEDOn(LED5);
}

void create_one_alarm_in_ms(time_t alarm_time){
	
	time_t currentime;
	currentime = get_current_time_ms();
	remaining_alarmtime = difftime(alarm_time, currentime);			//alarmtime - currenttime
	STM_EVAL_LEDOn(LED5);
}

void usart_set_time(time_t current_time){
	
	int date, month, year, dayofweek, hours, minutes, seconds, now;				
	struct tm *time_date;
			
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;	
				
	time_date = localtime(&current_time);
	printf("The Date is : %s", ctime(&current_time));
				
	date = (time_date->tm_mday);
	month = (time_date->tm_mon)+1;
	year = (time_date->tm_year)+1900;
				
	dayofweek = (time_date->tm_wday);
	if(dayofweek == 0)
		dayofweek = 7;
				
	hours = (time_date->tm_hour);
	minutes = (time_date->tm_min);
	seconds = (time_date->tm_sec);
				
	RTC_DateStructure.RTC_Year = year-2000;
	RTC_DateStructure.RTC_Month = month;
	RTC_DateStructure.RTC_Date = date;
	RTC_DateStructure.RTC_WeekDay = dayofweek;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);	
				
	RTC_TimeStructure.RTC_H12     = RTC_HourFormat_24;
	RTC_TimeStructure.RTC_Hours   = hours;
	RTC_TimeStructure.RTC_Minutes = minutes;
	RTC_TimeStructure.RTC_Seconds = seconds; 
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);  
	
	now = get_current_time_ms();
	printf("My calculation of current time is : %d", now);

}

void clear_structure(){
	int i=0;
	for(i=0;i<21;i++) {
		boxes.pillbox[i].box_number = 255;
		boxes.pillbox[i].alarmTime = 0;
				
		boxes.pillbox[i].box_state = 0;
		boxes.pillbox[i].alarm_created = 0;
		boxes.pillbox[i].alarm_ok = 0;
	}	
}

void open_box(){
			stopMotor();
			for(c=0; c<9; c++){ //right
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				delay_us(500);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				delay_us(19500);
			}
			for(c=0; c<40; c++){ //middle
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				delay_us(1500);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				delay_us(18500);
			}
}

void usart_interrup_main(){
	
	printf( "Received String: %s\n",received_string);

 	flag_usart = 0;
	token_size = 0;
	memset(copy1,0,sizeof(copy1));
	
	token = strtok(received_string, "}"); 
  while( token != NULL ) {
		token[strlen(token)] = 0x7D;
		printf( "%d: %s\n",token_size, token);
		buffer[token_size] = token;
    token = strtok(NULL, "}");
		token_size++;
  }		
	for(x=0; x<token_size-1; x++){
		if(x+1 != token_size) {
			strncpy(copy1[x],buffer[x],strlen(buffer[x]) - strlen(buffer[x+1]) + 1);
			buffer[x] = copy1[x];
		}
		else{
			strcpy(copy1[x],buffer[x]);
			buffer[x] = copy1[x];
		}
		
		if(token_size>1) {
			buffer[x]++;
			buffer[x][strlen(buffer[x])-1] = 0;
		}	
		printf( "buffer%d: %s\n",x,buffer[x]);
	}	
	
	root = json_loads(buffer[0], 0, &error);
	if(error.line != 0xFFFFFFFF) {
		STM_EVAL_LEDToggle(LED3);
	}
	message_obj = json_object_get(root,"mS");
	message = json_integer_value(message_obj);
	
	switch(message){
	
		case 6:
			
			stopMotor();
			for(c=0; c<10; c++){ //right
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				delay_us(500);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				delay_us(19500);
			}
			for(c=0; c<150; c++){ //middle
				GPIO_SetBits(GPIOE, GPIO_Pin_5);
				delay_us(1500);
				GPIO_ResetBits(GPIOE, GPIO_Pin_5);
				delay_us(18500);
			}
	
			json_decref(root);
		break;
		
		case 5:
			motor_direction_obj = json_object_get(root,"motor_direction");
			motor_direction = json_integer_value(motor_direction_obj);
			Direction = motor_direction;
			json_decref(root);
		break;
		
		case 4:
			motor_obj = json_object_get(root,"motor_onoff");
			motor_onoff = json_integer_value(motor_obj);
			Motor_Status = motor_onoff;
			json_decref(root);
		break;
		
		case 3:
			led_obj = json_object_get(root,"led_status");
			led_status = json_integer_value(led_obj);
			if(led_status == 0)
				STM_EVAL_LEDOff(LED3);
			else
				STM_EVAL_LEDOn(LED3);
			json_decref(root);
		break;

		case 2:
			current_time_obj = json_object_get(root,"cT");
			current_time = json_integer_value(current_time_obj);
			usart_set_time(current_time);
			json_decref(root);
			//read_boxes_state();
			readEEPROMtosend();

		break;			
		
		case 1:
			
			clear_eeprom();
			clear_structure();
			//First Initilazation
			Direction = 1;
			while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
				stepper();
				delay_us(3000);
			}
			current_box = 0;
			next_box = 0;
		
			current_time_obj = json_object_get(root,"cT");
			current_time = json_integer_value(current_time_obj);
		  usart_set_time(current_time);
			json_decref(root);
		
			for(x=1; x<token_size-1; x++){
				root = json_loads(buffer[x], 0, &error);
				if(error.line != 0xFFFFFFFF) {
					STM_EVAL_LEDToggle(LED3);
				}
				bN_obj = json_object_get(root,"bN");
				bS_obj = json_object_get(root,"bS");
				aT_obj = json_object_get(root,"aT");

			  bN = json_integer_value(bN_obj);
				bS = json_integer_value(bS_obj);
			  aT = json_integer_value(aT_obj);
				alarmtime = aT;
				
				printf("Alarm %d: %s", x-1, ctime(&alarmtime));
				
				boxes.pillbox[x-1].box_number = bN;
				boxes.pillbox[x-1].alarmTime = aT;
				
				boxes.pillbox[x-1].box_state = bS;
				boxes.pillbox[x-1].alarm_created = 0;
				boxes.pillbox[x-1].alarm_ok = 0;
				
				json_decref(root);
				
				writeAlarmtoEEPROM(x-1);
				
				if(x==1) {
					
					boxes.pillbox[x-1].alarm_created = 1;
					next_box = bN;
					
					//create_one_alarm(aT);
					create_one_alarm_in_ms(aT);
					STM_EVAL_LEDOn(LED3);
					which_alarm_created = 0;
					
					set_Box_State_Flag(which_alarm_created,CLOSE_FULL);
				}	
			}
			
			go_to_box(current_box,next_box);
		
		break;	
	}
}
