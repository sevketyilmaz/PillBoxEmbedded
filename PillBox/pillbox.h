#ifndef __PILLBOX_H
#define __PILLBOX_H

#include <stdint.h>
#include <time.h>

typedef struct{
	
	uint8_t hour;
	uint8_t minute;
	uint8_t dayofweek;
	
}alarm_time;

typedef struct{
	
	uint8_t date;
	uint8_t mounth;
	uint8_t year;
	
}alarm_date;

typedef struct{
	int alarmTime;
	uint8_t box_state; 
	uint8_t alarm_creating_order; // If it equal 1 box close, not box open
	uint8_t alarm_created; 				// If it equal 1 box close, not box open
	uint8_t alarm_ok;
	
}BoxSettings;

struct Boxes{					
	
	BoxSettings pillbox[28];
};


extern struct Boxes boxes;

void createAlarm(uint8_t hour, uint8_t minutes, uint8_t dayOfWeek);
uint8_t getVariable(char variable[50]);
void writeAlarmtoEEPROM(uint8_t boxnumbers);

void usart_create_alarm(void);
void usart_set_time(time_t current_time);
void usart_interrup_main(void);

#endif /* __PILLBOX_H */
