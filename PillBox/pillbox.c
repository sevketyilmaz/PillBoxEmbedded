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

#include "String.h"
#include "Jansson.h"

struct Boxes boxes;


char *token;
char *(buffer[40]);
char copy1[40][40];
int token_size=0;
int x;

int message,led_status,current_time,bN,bS,aT;

json_t *root, *message_obj, *led_obj, *current_time_obj, *bN_obj, *bS_obj, *aT_obj;
json_error_t error;

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
				/*
				if(boxnumbers == 2) {
										
					Boxes.pillbox[1].alarm_time.hour = hour;
					Boxes.pillbox[1].alarm_time.minute = minute;
					Boxes.pillbox[1].alarm_time.dayofweek = dayOfWeek;
					
					Boxes.pillbox[1].alarm_created = 0;
					Boxes.pillbox[1].alarm_ok = 0;
					Boxes.pillbox[1].box_open_close = 1;
					Boxes.pillbox[1].alarm_creating_order = creatingorder;
				}
				
				if(boxnumbers == 3) {
										
					Boxes.pillbox[2].alarm_time.hour = hour;
					Boxes.pillbox[2].alarm_time.minute = minute;
					Boxes.pillbox[2].alarm_time.dayofweek = dayOfWeek;
					
					Boxes.pillbox[2].alarm_created = 0;
					Boxes.pillbox[2].alarm_ok = 0;
					Boxes.pillbox[2].box_open_close = 1;
					Boxes.pillbox[2].alarm_creating_order = creatingorder;
				}
				*/
				return boxnumbers;
};

void writeAlarmtoEEPROM(uint8_t boxnumbers){

		uint8_t data[9], start_address, end_address, length, byte_size=9; 
		
		//data[0] = boxes.pillbox[boxnumbers-1].alarm_date.date;
		//data[1] = boxes.pillbox[boxnumbers-1].alarm_date.mounth;
		//data[2] = boxes.pillbox[boxnumbers-1].alarm_date.year;
			
		//data[3] =	boxes.pillbox[boxnumbers-1].alarm_time.hour;
		//data[4] =	boxes.pillbox[boxnumbers-1].alarm_time.minute;
		//data[5] =	boxes.pillbox[boxnumbers-1].alarm_time.dayofweek;
		 			
		//data[6] = boxes.pillbox[boxnumbers-1].alarm_created;
		//data[7] =	boxes.pillbox[boxnumbers-1].alarm_ok;
		//data[8] = boxes.pillbox[boxnumbers-1].box_open_close;
	
		start_address = 1 + byte_size*(boxnumbers - 1);
		end_address = 9 + byte_size*(boxnumbers - 1);
		length = end_address-start_address + 1;
	
	  eeprom_write_page(0x50<<1, start_address, data, length );
}

void usart_create_alarm(){
	
	int alarm_date, alarm_month, alarm_year, alarm_dayofweek, alarm_hours, alarm_minutes, alarm_seconds;
	int box_number;
	char * e;
	time_t received_alarm;
	struct tm *alarm;
	uint32_t getalarm;
				
	box_number = received_string[1]-48;
				
	memmove(received_string, received_string+3, strlen (received_string+3) + 3); // or just strlen (s)
	received_alarm = (time_t) strtoll(received_string, &e, 10);

	if (*e != 0) { /* error, don't use time! */ }
				
	alarm = localtime(&received_alarm);
	printf("The Alarm is : %s", ctime(&received_alarm));
				
	alarm_date = (alarm->tm_mday);
	alarm_month = (alarm->tm_mon)+1;
	alarm_year = (alarm->tm_year)+1900;
				
	alarm_dayofweek = (alarm->tm_wday);
	if(alarm_dayofweek == 0)
		alarm_dayofweek = 7;
				
	alarm_hours = (alarm->tm_hour);
	alarm_minutes = (alarm->tm_min);
	alarm_seconds = (alarm->tm_sec);
				
	eeprom_write_long(0x50<<1, 28, received_alarm);
	eeprom_read_long(0x50<<1, 28, &getalarm);	
				
	createAlarm(alarm_hours ,alarm_minutes, alarm_dayofweek);
	STM_EVAL_LEDOn(LED5);
}

void usart_set_time(time_t current_time){
	
	int date, month, year, dayofweek, hours, minutes, seconds;				
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
}

void usart_interrup_main(){
	
 	flag_usart = 0;
	token_size = 0;
	memset(copy1,0,sizeof(copy1));
	
	token = strtok(received_string, "}"); 
  while( token != NULL ) {
		token[strlen(token)] = 0x7D;
		//printf( "%d: %s\n",token_size, token);
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
		//printf( "buffer%d: %s\n",x,buffer[x]);
	}	
	
	root = json_loads(buffer[0], 0, &error);
	if(error.line != 0xFFFFFFFF) {
		STM_EVAL_LEDToggle(LED4);
	}
	message_obj = json_object_get(root,"mS");
	message = json_integer_value(message_obj);
	
	switch(message){
	
		case 3:
			led_obj = json_object_get(root,"led_status");
			led_status = json_integer_value(led_obj);
			if(led_status == 0)
				STM_EVAL_LEDOff(LED6);
			else
				STM_EVAL_LEDOn(LED6);
			json_decref(root);
		break;

		case 2:
			current_time_obj = json_object_get(root,"current_time");
			current_time = json_integer_value(current_time_obj);
		  usart_set_time(current_time);
			json_decref(root);
		break;			
		
		case 1:
			current_time_obj = json_object_get(root,"cT");
			current_time = json_integer_value(current_time_obj);
		  usart_set_time(current_time);
			json_decref(root);
			for(x=1; x<token_size-1; x++){
				root = json_loads(buffer[x], 0, &error);
				if(error.line != 0xFFFFFFFF) {
					STM_EVAL_LEDToggle(LED4);
				}
				bN_obj = json_object_get(root,"bN");
				bS_obj = json_object_get(root,"bS");
				aT_obj = json_object_get(root,"aT");

			  bN = json_integer_value(bN_obj);
				bS = json_integer_value(bS_obj);
			  aT = json_integer_value(aT_obj);
				
				boxes.pillbox[bN].alarmTime = aT;
				boxes.pillbox[bN].box_state = bS;
				json_decref(root);
			}		
		break;	
		
	}
}
