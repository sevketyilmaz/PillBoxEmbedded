/*
MOTOR
PE7
PE8
PE9
PE10

4 pin Stepper Motor

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

STEPS | STEP1 | STEP2 | STEP3 | STEP4 |
---------------------------------------
PE7   |   0   |   0   |   1   |   1   |
---------------------------------------
PE8   |   1   |   1   |   0   |   0   |
---------------------------------------
PE9   |   0   |   1   |   1   |   0   |
---------------------------------------
PE10  |   1   |   0   |   0   |   1   |
---------------------------------------
      |   5   |   6   |   10  |   9   |
---------------------------------------
*/

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

#include "motor.h"
#include "delay.h"

int Direction = 1, Steps = 0, Motor_Status = 0;
BitAction LOW = Bit_RESET, HIGH = Bit_SET;
int step_count;
int m=0;

void go_to_box(uint8_t current_boxnumber, uint8_t next_boxnumber){
			
	int a = 5750;
	int b = 12450; //hesap 15400
	int n = 0;
	int step_count = 0;

	n = next_boxnumber - current_boxnumber;
	
	if(n>0) {
		Direction = 0;
	}	
	else {
		Direction = 1;
		n = -n;
		current_boxnumber = next_boxnumber;
		next_boxnumber = current_boxnumber +n;
	}
	
	step_count = a*n + (b-a)*((next_boxnumber/3) - (current_boxnumber/3));
	
	for(m=0;m<step_count;m++) {		
		stepper();
		delay_us(3300);
	}
	
	stopMotor();
}

void motor_switch_pin_init(){
	GPIO_InitTypeDef GPIO_InitStruct;
	  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void motor_pin_init(void) {
	
	  GPIO_InitTypeDef GPIO_InitStruct_Motor;
	  
	  GPIO_InitStruct_Motor.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 ;
	  GPIO_InitStruct_Motor.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct_Motor.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct_Motor.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct_Motor.GPIO_Speed = GPIO_Speed_100MHz;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);		
	  GPIO_Init(GPIOE, &GPIO_InitStruct_Motor);
}

void solenoid_pin_init(void) {
	
	  GPIO_InitTypeDef GPIO_InitStruct_Motor;
	  GPIO_InitStruct_Motor.GPIO_Pin = GPIO_Pin_7;
		GPIO_InitStruct_Motor.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStruct_Motor.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct_Motor.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStruct_Motor.GPIO_Speed = GPIO_Speed_100MHz;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);		
	  GPIO_Init(GPIOC, &GPIO_InitStruct_Motor);
}

void stepper(void){
	
		switch(Steps){
			case 0:
			GPIO_WriteBit(GPIOE,GPIO_Pin_7, HIGH); 
			GPIO_WriteBit(GPIOE,GPIO_Pin_8, HIGH);
			GPIO_WriteBit(GPIOE,GPIO_Pin_9, LOW);
			GPIO_WriteBit(GPIOE,GPIO_Pin_10, LOW);
			break; 
			case 1:
			GPIO_WriteBit(GPIOE,GPIO_Pin_7, LOW); 
			GPIO_WriteBit(GPIOE,GPIO_Pin_8, HIGH);
			GPIO_WriteBit(GPIOE,GPIO_Pin_9, HIGH);
			GPIO_WriteBit(GPIOE,GPIO_Pin_10, LOW);
			break; 
			case 2:
			GPIO_WriteBit(GPIOE,GPIO_Pin_7, LOW); 
			GPIO_WriteBit(GPIOE,GPIO_Pin_8, LOW);
			GPIO_WriteBit(GPIOE,GPIO_Pin_9, HIGH);
			GPIO_WriteBit(GPIOE,GPIO_Pin_10, HIGH);
			break; 
			case 3:
			GPIO_WriteBit(GPIOE,GPIO_Pin_7, HIGH); 
			GPIO_WriteBit(GPIOE,GPIO_Pin_8, LOW);
			GPIO_WriteBit(GPIOE,GPIO_Pin_9, LOW);
			GPIO_WriteBit(GPIOE,GPIO_Pin_10, HIGH);
			break; 
			default:
			GPIO_WriteBit(GPIOE,GPIO_Pin_7, LOW); 
			GPIO_WriteBit(GPIOE,GPIO_Pin_8, LOW);
			GPIO_WriteBit(GPIOE,GPIO_Pin_9, LOW);
			GPIO_WriteBit(GPIOE,GPIO_Pin_10, LOW);
			break; 
		}
		SetDirection();
} 

void stopMotor(void) {
	
	GPIO_WriteBit(GPIOE,GPIO_Pin_7, LOW); 
	GPIO_WriteBit(GPIOE,GPIO_Pin_8, LOW);
	GPIO_WriteBit(GPIOE,GPIO_Pin_9, LOW);
	GPIO_WriteBit(GPIOE,GPIO_Pin_10, LOW);
}

void SetDirection(void){
	if(Direction==1){ Steps++;}
	if(Direction==0){ Steps--;}
	if(Steps>3){Steps=0;}
	if(Steps<0){Steps=3;}
}
