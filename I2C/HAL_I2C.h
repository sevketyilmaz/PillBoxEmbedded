/* Define to prevent recursive inclusion*/
#ifndef __HAL_I2C_H
#define __HAL_I2C_H

/* Includes */
#include "stm32f4xx.h"

#define I2C                  I2C1
#define I2C_RCC_Periph       RCC_APB1Periph_I2C1
#define I2C_SCL_Pin          GPIO_Pin_8
#define I2C_SDA_Pin          GPIO_Pin_9
#define I2C_RCC_PortA        RCC_AHB1Periph_GPIOA
#define I2C_RCC_PortC        RCC_AHB1Periph_GPIOC

#define I2C_Speed            50000 // 100kHz standard mode

#endif /* __HAL_I2C_H */
