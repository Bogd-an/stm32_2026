#ifndef LCD_H_
#define LCD_H_

#include "main.h"

// Команди дисплея

// Прототипи функцій
void LCD_Init(void);
void LCD_Char(char ch);
void LCD_String(char* str);
void LCD_SetPos(uint8_t x, uint8_t y);
void LCD_Clear(void);

/* //////////////////// lcd.c BEGIN //////////////////// */

#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// Внутрішня функція для формування стробуючого імпульсу на лінії Enable
static void LCD_PulseEnable(void) {
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin,  GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_E_GPIO_Port, LCD_E_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

// Передача 4 біт (нібла) на дисплей
uint16_t pins[] = {LCD_D4_Pin, LCD_D5_Pin, LCD_D6_Pin, LCD_D7_Pin};
GPIO_TypeDef* ports[] = {LCD_D4_GPIO_Port, LCD_D5_GPIO_Port, LCD_D6_GPIO_Port, LCD_D7_GPIO_Port};
static void LCD_WriteNibble(uint8_t nibble) {
    for(uint8_t i = 0; i < 4; i++)
        HAL_GPIO_WritePin(ports[i], pins[i], (GPIO_PinState)((nibble >> i) & 0x01));
    LCD_PulseEnable();
}

// Передача байта (команди або даних)
static void LCD_Send(uint8_t value, uint8_t mode) {
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, (GPIO_PinState)mode); // 0 - команда, 1 - дані
    LCD_WriteNibble(value >> 4);   // Старші 4 біти
    LCD_WriteNibble(value & 0x0F); // Молодші 4 біти
}

/* //////////////////// PUBLIC //////////////////// */

void LCD_Init(void) {
    HAL_Delay(50); // Очікування стабілізації живлення
    
    // Ініціалізація у 4-бітному режимі
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    LCD_WriteNibble(0x03);
    HAL_Delay(5);
    LCD_WriteNibble(0x03);
    HAL_Delay(1);
    LCD_WriteNibble(0x03);
    LCD_WriteNibble(0x02); // Встановлення 4-бітного режиму
    
    // Налаштування дисплея
    LCD_Send(LCD_FUNCTIONSET | 0x08, 0); // 2 рядки, шрифт 5x8
    LCD_Send(LCD_DISPLAYCONTROL | 0x04, 0); // Дисплей увімкнено, курсор вимкнено
    LCD_Clear();
    LCD_Send(LCD_ENTRYMODESET | 0x02, 0); // Автоінкремент курсора
}

void LCD_Clear(void) {
    LCD_Send(LCD_CLEARDISPLAY, 0);
    HAL_Delay(2);
}

void LCD_SetPos(uint8_t x, uint8_t y) {
    LCD_Send(((y == 0) ? (0x80 + x) : (0xC0 + x)), 0);
}

void LCD_Char(char ch) {
    LCD_Send((uint8_t)ch, 1);
}

void LCD_String(char* str) {
    while(*str) LCD_Char(*str++);
}



/* //////////////////// lcd.c END //////////////////// */

#endif /* LCD_H_ */