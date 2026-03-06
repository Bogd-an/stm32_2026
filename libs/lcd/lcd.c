/*
 * lcd.c
 *
 *  Created on: Aug 2, 2020
 *      Author: drewmt
 */
#include "lcd.h"


void delay(void)
{
       uint16_t i;
       for(i=0;i<1000;i++)
       	   {}
}

 void LCD_WriteData(uint8_t data)
 {
        if(((data >> 3)&0x01)==1) {D7_set;} else {D7_reset;}
        if(((data >> 2)&0x01)==1) {D6_set;} else {D6_reset;}
        if(((data >> 1)&0x01)==1) {D5_set;} else {D5_reset;}
        if((data&0x01)==1) {D4_set;} else {D4_reset;}
 }

 void LCD_Command(uint8_t data)
 {
        RS0;
        LCD_WriteData(data >> 4);
        E1;
        delay();
        E0;
        LCD_WriteData(data);
        E1;
        delay();
        E0;
 }

 void LCD_init(void)
  {
	 	HAL_Delay(40);
		RS0;
		LCD_WriteData(3);
		E1;
		delay();
		E0;
		HAL_Delay(5);
		LCD_WriteData(3);
		E1;
		delay();
		E0;
		HAL_Delay(1);
		LCD_WriteData(3);
		E1;
		delay();
		E0;
		HAL_Delay(1);
		LCD_Command(0x2);	// активувати 4-бітнимй режим
		HAL_Delay(1);
		LCD_Command(0x08);	// 2 лінії, символ 5х8
		HAL_Delay(1);
		LCD_Command(0x0F);	// увімкнути дисплей (D=1) та курсор
		HAL_Delay(1);
		LCD_Command(0x01);	// очистити дисплей
		HAL_Delay(2);
		LCD_Command(0x06);	// друк вліво
		HAL_Delay(1);
		LCD_Command(0x02);	// повернути курсор в початкове положення
		HAL_Delay(2);

}

//---------------------------------------------------

 void LCD_Data(uint8_t data)
{
        RS1;
        LCD_WriteData(data>>4);
        E1;
        delay();
        E0;
        LCD_WriteData(data);
        E1;
        delay();
        E0;
}

 void LCD_SendChar(char ch)
{
        LCD_Data((uint8_t) ch);
        delay();
}

 void LCD_String(char* st)
{
	uint8_t i = 0;
	while (st[i] != 0)
	{
		LCD_Data(st[i]);
		delay();
		i++;
	}
}

void LCD_SetPos(uint8_t x, uint8_t y)
{
	switch (y)
	{
	case 0:
		LCD_Command(x | 0x80);
		HAL_Delay(1);
		break;
	case 1:
		LCD_Command((0x40 + x) | 0x80);
		HAL_Delay(1);
		break;
	}
}

 void LCD_Clear(void)
 {
	LCD_Command(0x01);
	HAL_Delay(2);
 }

//-------------------------------------
void LCD_RunningLine(char* line)
{
	uint8_t i = 0;
	for (i = 0; i <= 32; i++) {
		if(17 - i >=0 )
		{
			LCD_SetPos(17 - i, 0);
			LCD_String(line);
		}
		else
		{
			int idxToDel = 0;
			memmove(&line[idxToDel], &line[idxToDel + 1], strlen(line) - idxToDel);
			LCD_SetPos(0, 0);
			LCD_String(line);
		}
		HAL_Delay(500);
		LCD_Clear();
	}
}
