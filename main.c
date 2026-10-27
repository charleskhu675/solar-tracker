/*
 * File:   main.c
 * Author: 
 *
 * Created on October 13, 2026, 2:33 PM
 */


#include <xc.h>
#include <stdint.h>
#include "servos.h"
#include "photoresistor.h"  // change name to the photoresistor header file
#include "PIC18F46K22-Config.h"
#include "LCD.h"

void display_solar_values(float voltage,
                          float power,
                          uint8_t sun_percent)
#define DEADZONE 50
#define AZ_SPEED 25

{
    // First LCD row: voltage
    LCD_cursor_set(1, 1);
    LCD_write_string("V:");
    LCD_write_float(voltage, 0, 2);
    LCD_write_string("V       ");

    // Second LCD row: power and sunlight percentage
    LCD_cursor_set(2, 1);
    LCD_write_string("P:");
    LCD_write_float(power, 0, 2);
    LCD_write_string("W S:");
    LCD_write_variable(sun_percent, 0);
    LCD_write_string("%  ");
}

void main(void)
{
    // Temporary test values
    float voltage = 12.50f;
    float power = 25.75f;
    uint8_t sun_percent = 80;

    // Initialize the LCD
    LCD_init();

    while (1)
    {
        display_solar_values(voltage, power, sun_percent);

        __delay_ms(500);
    }
}
