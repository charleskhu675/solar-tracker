/*
 * File:   main.c
 * Author: 
 *
 * Created on October 13, 2026, 2:33 PM
 */


#include <xc.h>
#include <stdint.h>
#include "PIC18F46K22-Config.h"
#include "LCD.h"

#define _XTAL_FREQ 16000000UL   // 16 MHz HFINTOSC

void main(void)
{
    float voltage = 12.50f;
    float power = 25.75f;
    uint8_t sun_percent = 80;

    LCD_init();

    while (1)
    {
        LCD_cursor_set(1, 1);
        LCD_write_string("Voltage: ");
        LCD_write_float(voltage, 0, 2);
        LCD_write_string(" V   ");

        LCD_cursor_set(2, 1);
        LCD_write_string("Power: ");
        LCD_write_float(power, 0, 2);
        LCD_write_string(" W   ");

        LCD_cursor_set(3, 1);
        LCD_write_string("Sun: ");
        LCD_write_variable(sun_percent, 0);
        LCD_write_string("%   ");

        __delay_ms(500);
    }
}
void display_solar_values(float voltage, float power, uint8_t sun_percent)

{
    LCD_cursor_set(1,1);
    LCD_write_string("voltage: ");
    LCD_write_float(voltage,0 ,2);
}
