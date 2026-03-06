/*
 * lcd.h
 *
 *  Created on: Aug 2, 2020
 *      Author: drewmt
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f1xx_hal.h"
#include "string.h"


#endif /* INC_LCD_H_ */

// високий рівень на лініях шини даних
#define D4_set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define D5_set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define D6_set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define D7_set HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)

// низький рівень на лініях шини даних
#define D4_reset HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define D5_reset HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define D6_reset HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define D7_reset HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)

// установка лінії E в 1
#define E1    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
// установка лінії E в 0
#define E0    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
// установка лінії RS в 1 (режим передачі даних)
#define RS1   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
// установка лінії RS в 0 (режим передачі команд)
#define RS0   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)


 void LCD_init(void);
 void LCD_SendChar(char ch);
 void LCD_String(char* st);
 void LCD_SetPos(uint8_t x, uint8_t y);
 void LCD_Clear(void);

 void LCD_RunningLine(char* line);
