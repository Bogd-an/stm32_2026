// LCD_CS, LCD_SID, LCD_SCK

// № піна LCD,Назва піна,Куди підключати на STM32,Примітка
// 1,  VSS (GND),GND,         Спільний мінус живлення.
// 2,  VDD (VCC),5V,          Важливо! Для чіткого зображення краще 5В.
// 3,  VO,       Wiper ,      Середня ніжка резистора 10кОм для контрасту.
// 4,  RS (CS),  PA4 (LCD_CS),Chip Select (Вибір пристрою).
// 5,  RW(SID),  PA7(LCD_SID),Serial Data (Дані).
// 6,  E (SCK),  PA5(LCD_SCK),Serial Clock (Тактування).
// 15, PSB,      GND,         для вибору Serial режиму.
// 17, RST,      3.3V 
// 18, VOUT,     -
// 19, BLA,      5V (220 Ом), Анод підсвітки (плюс).
// 20, BLK,      GND,         Катод підсвітки (мінус).

#ifndef ST7920_H_
#define ST7920_H_

#include "main.h"

// Прототипи
void LCD_Init(void);
void LCD_SendCmd(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_String(char* str);
void LCD_SetPos(uint8_t x, uint8_t y);
void LCD_Clear(void);

// Затримка для синхронізації (ST7920 досить повільний)
static void LCD_Delay(void) {
    for(uint32_t i=0; i<100; i++) __NOP(); 
}

// Програмна реалізація передачі байта (Bit-banging SPI)
static void LCD_SendByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, (byte & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LCD_SCK_GPIO_Port, LCD_SCK_Pin, GPIO_PIN_RESET);
        LCD_Delay();
        HAL_GPIO_WritePin(LCD_SCK_GPIO_Port, LCD_SCK_Pin, GPIO_PIN_SET);
        LCD_Delay();
        byte <<= 1;
    }
}

// Передача пакета: Стартова послідовність + Нібли даних
static void LCD_Write(uint8_t type, uint8_t data) {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
    
    // 11111 + ABC (A: RW, B: RS, C: 0)
    // Для запису: RS=0(cmd) або RS=1(data), RW=0
    LCD_SendByte(type == 0 ? 0xF8 : 0xFA); 
    
    LCD_SendByte(data & 0xF0);        // Старші 4 біти
    LCD_SendByte((data << 4) & 0xF0); // Молодші 4 біти
    
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_SendCmd(uint8_t cmd)   { LCD_Write(0, cmd); }
void LCD_SendData(uint8_t data) { LCD_Write(1, data); }

void LCD_Init(void) {
    HAL_Delay(50);
    LCD_SendCmd(0x30); // Basic instruction set
    HAL_Delay(1);
    LCD_SendCmd(0x30); // Повторно для стабілізації
    HAL_Delay(1);
    LCD_SendCmd(0x0C); // Display ON, Cursor OFF
    LCD_Clear();
    LCD_SendCmd(0x06); // Entry mode: Cursor moves right
}

void LCD_Clear(void) {
    LCD_SendCmd(0x01);
    HAL_Delay(2);
}

void LCD_SetPos(uint8_t x, uint8_t y) {
    // Для ST7920 адресація рядків: 0x80, 0x90, 0x88, 0x98
    uint8_t addr;
    switch (y) {
        case 0: addr = 0x80 + x; break;
        case 1: addr = 0x90 + x; break;
        case 2: addr = 0x88 + x; break;
        case 3: addr = 0x98 + x; break;
        default: addr = 0x80;
    }
    LCD_SendCmd(addr);
}

void LCD_Char(char c) {
    LCD_SendData((uint8_t)c);
}

void LCD_String(char* str) {
    while(*str) LCD_Char(*str++);
}

#endif