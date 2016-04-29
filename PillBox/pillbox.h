#ifndef __PILLBOX_H
#define __PILLBOX_H

#include <stdint.h>
#include <time.h>
#include "stm32f4xx.h"

#define CLOSE_EMPTY 				0
#define CLOSE_FULL					1
#define CLOSE_DONE 					2
#define OPEN_FULL						3

#define Box1 				GPIO_Pin_13
#define Box1_Port 	GPIOE

#define Box2 				GPIO_Pin_12
#define Box2_Port 	GPIOE

#define Box3 				GPIO_Pin_11
#define Box3_Port 	GPIOE

#define Box4 				GPIO_Pin_10
#define Box4_Port 	GPIOE

#define Box5 				GPIO_Pin_9
#define Box5_Port 	GPIOE

#define Box6 				GPIO_Pin_8
#define Box6_Port 	GPIOE

#define Box7 				GPIO_Pin_7
#define Box7_Port 	GPIOE

#define Box8 				GPIO_Pin_1
#define Box8_Port 	GPIOB

#define Box9 				GPIO_Pin_0
#define Box9_Port 	GPIOB

#define Box10 			GPIO_Pin_5
#define Box10_Port	GPIOC



#define Box11 			GPIO_Pin_4
#define Box11_Port 	GPIOC

#define Box12 			GPIO_Pin_11
#define Box12_Port 	GPIOD

#define Box13 			GPIO_Pin_10
#define Box13_Port 	GPIOD

#define Box14 			GPIO_Pin_9
#define Box14_Port 	GPIOD

#define Box15 			GPIO_Pin_8
#define Box15_Port 	GPIOD

#define Box16 			GPIO_Pin_15
#define Box16_Port 	GPIOB

#define Box17 			GPIO_Pin_14
#define Box17_Port 	GPIOB

#define Box18 			GPIO_Pin_13
#define Box18_Port 	GPIOB

#define Box19 			GPIO_Pin_12
#define Box19_Port 	GPIOB

#define Box20 			GPIO_Pin_15
#define Box20_Port 	GPIOE

#define Box21 			GPIO_Pin_14
#define Box21_Port 	GPIOE



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

void set_Box_State_Flag(uint8_t boxnumbers, uint8_t box_state);
uint8_t read_Box_State_Flag(uint8_t boxnumbers);

void create_one_alarm_in_ms(time_t alarm_time);
void open_box(void);

void bluetooth_status_pins_init(void);

void readEEPROMtosend(void);

#endif /* __PILLBOX_H */

