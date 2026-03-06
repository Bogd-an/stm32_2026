/*
 * keypad.c
 *
 *  Created on: Jul 26, 2020
 *      Author: drewmt
 */

#include "keypad.h"


// Допоміжні структури для зручності
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

// char read_keypad (void)
// {
// 	/* Встановити НИЗЬКИЙ рівень на рядку R1 та ВИСОКИЙ на всіх інших рядках */
// 	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);

// 	// якщо на стовпці С1 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))
// 	{
// 		// очікуємо поки кнопка натиснута
// 		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));
// 		return '1';
// 	}

// 	// якщо на стовпці С2 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));
// 		return '2';
// 	}

// 	// якщо на стовпці С3 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));
// 		return '3';
// 	}

// 	// якщо на стовпці С4 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));
// 		return 'A';
// 	}

// 	/* Встановити НИЗЬКИЙ рівень на рядку R2 та ВИСОКИЙ на всіх інших рядках */
// 	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);

// 	// якщо на стовпці С1 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));
// 		return '4';
// 	}

// 	// якщо на стовпці С2 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));
// 		return '5';
// 	}

// 	// якщо на стовпці С3 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));
// 		return '6';
// 	}

// 	// якщо на стовпці С4 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));
// 		return 'B';
// 	}


// 	/* Встановити НИЗЬКИЙ рівень на рядку R3 та ВИСОКИЙ на всіх інших рядках */
// 	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_SET);

// 	// якщо на стовпці С1 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));
// 		return '7';
// 	}

// 	// якщо на стовпці С2 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));
// 		return '8';
// 	}

// 	// якщо на стовпці С3 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));
// 		return '9';
// 	}
// 	// якщо на стовпці С4 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));
// 		return 'C';
// 	}


// 	/* Встановити НИЗЬКИЙ рівень на рядку R4 та ВИСОКИЙ на всіх інших рядках */
// 	HAL_GPIO_WritePin (R1_PORT, R1_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R2_PORT, R2_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R3_PORT, R3_PIN, GPIO_PIN_SET);
// 	HAL_GPIO_WritePin (R4_PORT, R4_PIN, GPIO_PIN_RESET);

// 	// якщо на стовпці С1 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C1_PORT, C1_PIN)));
// 		return '*';
// 	}

// 	// якщо на стовпці С2 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C2_PORT, C2_PIN)));
// 		return '0';
// 	}

// 	// якщо на стовпці С3 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C3_PORT, C3_PIN)));
// 		return '#';
// 	}

// 	// якщо на стовпці С4 низький рівень
// 	if (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)))
// 	{
// 		while (!(HAL_GPIO_ReadPin (C4_PORT, C4_PIN)));
// 		return 'D';
// 	}

// 	return 0; 	// якщо жодна кнопка не натиснута
// }
