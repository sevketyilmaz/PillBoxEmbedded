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
	uint8_t box_number; 
	uint8_t box_state; 
	uint8_t alarm_created; 				// If it equal 1 box close, not box open
	uint8_t alarm_ok;
	
}BoxSettings;

struct Boxes{					
	
	BoxSettings pillbox[28];
};


extern struct Boxes boxes;
extern uint8_t which_alarm_created;
extern int remaining_alarmtime;
extern int next_box, current_box;

void box_switch_pins_init(void);
void box_pins_init(void);
void read_boxes_state(void);

void createAlarm(uint8_t hour, uint8_t minutes, uint8_t dayOfWeek);
uint8_t getVariable(char variable[50]);
void writeAlarmtoEEPROM(uint8_t boxnumbers);
void readAlarmtoEEPROM(uint8_t boxnumbers);

void create_one_alarm(time_t alarm_time);
void usart_set_time(time_t current_time);
void usart_interrup_main(void);

void set_Alarm_Ok_Flag(uint8_t boxnumbers, uint8_t alarm_ok);
uint8_t read_Alarm_Ok_Flag(uint8_t boxnumbers);

void create_one_alarm_in_ms(time_t alarm_time);
void open_box(void);

#endif /* __PILLBOX_H */

