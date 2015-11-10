#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "stm32f4xx.h"
#include <stdlib.h>
#include <stdint.h>

/* Columns */
/* Column 1 default */
#define COLUMN_1_PORT       GPIOD
#define COLUMN_1_PIN        GPIO_Pin_1
/* Column 2 default */
#define COLUMN_2_PORT       GPIOD
#define COLUMN_2_PIN        GPIO_Pin_2
/* Column 3 default */
#define COLUMN_3_PORT       GPIOD
#define COLUMN_3_PIN        GPIO_Pin_3
/* Column 4 default */
#define COLUMN_4_PORT       GPIOD
#define COLUMN_4_PIN        GPIO_Pin_4
 
/* Rows */
/* Row 1 default */
#define ROW_1_PORT           GPIOC
#define ROW_1_PIN            GPIO_Pin_1
/* Row 2 default */
#define ROW_2_PORT           GPIOC
#define ROW_2_PIN            GPIO_Pin_2
/* Row 3 default */
#define ROW_3_PORT           GPIOC
#define ROW_3_PIN            GPIO_Pin_3
/* Row 4 default */
#define ROW_4_PORT           GPIOC
#define ROW_4_PIN            GPIO_Pin_5

void keypad_init(void);

#endif /* __KEYPAD_H */
