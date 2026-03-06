#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f1xx_hal.h"

// Налаштування пінів (можна змінювати тут)
#define LCD_PORT GPIOB
#define RS_PIN   GPIO_PIN_3
#define E_PIN    GPIO_PIN_4
#define D4_PIN   GPIO_PIN_5
#define D5_PIN   GPIO_PIN_6
#define D6_PIN   GPIO_PIN_7
#define D7_PIN   GPIO_PIN_8

// Прототипи функцій
void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Char(char ch);
void LCD_String(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Clear(void);

// lcd.c BEGIN


// Надійна затримка, яку не видалить компілятор
static void LCD_Delay(uint32_t us) {
    uint32_t count = us * (SystemCoreClock / 8000000); // Приблизно для F103
    for (volatile uint32_t i = 0; i < count; i++) {
        __NOP();
    }
}

// Внутрішня функція для "стробу" (сигнал E)
static void LCD_Pulse(void) {
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_SET);
    LCD_Delay(10); 
    HAL_GPIO_WritePin(LCD_PORT, E_PIN, GPIO_PIN_RESET);
    LCD_Delay(10);
}

// Відправка 4 біт (нібла) прямо на піни
static void LCD_SendInternal(uint8_t data) {
    HAL_GPIO_WritePin(LCD_PORT, D4_PIN, (data >> 0) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D5_PIN, (data >> 1) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D6_PIN, (data >> 2) & 0x01);
    HAL_GPIO_WritePin(LCD_PORT, D7_PIN, (data >> 3) & 0x01);
    LCD_Pulse();
}

void LCD_Command(uint8_t cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET); // Режим команд
    LCD_SendInternal(cmd >> 4); // Старший нібл
    LCD_SendInternal(cmd & 0x0F); // Молодший нібл
    HAL_Delay(2); // Командам потрібен час
}

void LCD_Char(char ch) {
    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_SET); // Режим даних
    LCD_SendInternal((uint8_t)ch >> 4);
    LCD_SendInternal((uint8_t)ch & 0x0F);
    LCD_Delay(50);
}

void LCD_Init(void) {
    HAL_Delay(50); // Чекаємо стабілізації живлення дисплея

    HAL_GPIO_WritePin(LCD_PORT, RS_PIN, GPIO_PIN_RESET);

    // Спеціальна послідовність для переходу в 4-бітний режим
    LCD_SendInternal(0x03);
    HAL_Delay(5);
    LCD_SendInternal(0x03);
    HAL_Delay(1);
    LCD_SendInternal(0x03);
    HAL_Delay(1);
    LCD_SendInternal(0x02); // Встановлюємо 4 біти

    // Тепер можна використовувати стандартну функцію LCD_Command
    LCD_Command(0x28); // 2 рядки, шрифт 5х8
    LCD_Command(0x0C); // Дисплей увімкнено, курсор вимкнено
    LCD_Command(0x06); // Інкремент курсору (пишемо зліва направо)
    LCD_Clear();
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t mask = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Command(mask);
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    HAL_Delay(2);
}

// lcd.c END

#endif /* INC_LCD_H_ */