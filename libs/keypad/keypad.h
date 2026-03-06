/*
 * keypad.h
 *
 *  Created on: Jul 26, 2020
 *      Author: drewmt
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_



#endif /* INC_KEYPAD_H_ */

#include "stm32f1xx_hal.h"
#include "main.h"

// Оголошуємо порти та виводи для підключення клавіатури
#define R1_PORT GPIOB		//порт рядка R1
#define R1_PIN GPIO_PIN_12	//вивід рядка R1

#define R2_PORT GPIOB
#define R2_PIN GPIO_PIN_13

#define R3_PORT GPIOB
#define R3_PIN GPIO_PIN_14

#define R4_PORT GPIOB
#define R4_PIN GPIO_PIN_15

#define C1_PORT GPIOA		//порт стовпця C1
#define C1_PIN GPIO_PIN_8	// вивід стовпця С1

#define C2_PORT GPIOA
#define C2_PIN GPIO_PIN_9

#define C3_PORT GPIOA
#define C3_PIN GPIO_PIN_10

#define C4_PORT GPIOA
#define C4_PIN GPIO_PIN_11

// прототип функції опитування клавіатури
char read_keypad (void);
