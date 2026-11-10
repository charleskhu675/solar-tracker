/*
 * File:   main.c
 * Author: 
 *
 * Created on October 13, 2026, 2:33 PM
 */


#include <xc.h>
#include <stdint.h>
#include "servos.h"
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

void photoresistor_init(void);
uint16_t photoresistor_read(uint8_t channel);
void ldr_error(int16_t *errorX, int16_t *errorY);
void horizontal_control(int16_t errorX);
void vertical_control(int16_t errorY);

void main(void)
{
    // Temporary test values
    float voltage = 12.50f;
    float power = 25.75f;
    uint8_t sun_percent = 80;

    // Light differences
    int16_t horizontalError;
    int16_t verticalError;

  // Initial servo 
     servo_set_azimuth_speed(0);
    servo_set_elevation_angle(90);
  
    // Initialize
    LCD_init();
    photoresistor_init();
    servo_init();

    while (1)
    {
        //read photoresistor
        ldr_error(&horizontalError, &verticalError);

        // servo control
        horizontal_control(horizontalError);
        vertical_control(verticalError);

        display_solar_values(voltage, power, sun_percent);

        __delay_ms(20);
    }
}
