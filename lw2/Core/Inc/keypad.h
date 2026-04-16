#include "main.h"
#include "stm32f1xx_hal.h"
GPIO_TypeDef* row_ports[] = {R1_PORT, R2_PORT, R3_PORT, R4_PORT};
uint16_t row_pins[] = {R1_PIN, R2_PIN, R3_PIN, R4_PIN};

GPIO_TypeDef* col_ports[] = {C1_PORT, C2_PORT, C3_PORT, C4_PORT};
uint16_t col_pins[] = {C1_PIN, C2_PIN, C3_PIN, C4_PIN};

char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char read_keypad(void){
    for (int r = 0; r < 4; r++){
        // 1. Встановлюємо НИЗЬКИЙ рівень на поточному рядку, на інших — ВИСОКИЙ
        for (int i = 0; i < 4; i++)
        HAL_GPIO_WritePin(row_ports[i], row_pins[i], (i == r) ? GPIO_PIN_RESET : GPIO_PIN_SET);

        // 2. Перевіряємо кожен стовпець
        for (int c = 0; c < 4; c++)
		if (HAL_GPIO_ReadPin(col_ports[c], col_pins[c]) == GPIO_PIN_RESET)
		{
			// Очікуємо відпускання кнопки (Debounce/Wait)
			while (HAL_GPIO_ReadPin(col_ports[c], col_pins[c]) == GPIO_PIN_RESET);
			
			return keys[r][c];
		}
    }

    return 0; // Якщо нічого не натиснуто
}