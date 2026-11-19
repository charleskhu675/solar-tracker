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

#define DEADZONE 50
#define AZ_SPEED 25
#define SOLAR_PANEL_ADC_CHANNEL 11
#define ADC_COUNTS 1023
#define ADC_VREF_VOLTS 5.0

uint16_t photoresistor_read(uint8_t channel);

void display_solar_values(uint8_t adc_channel)
{
    uint16_t adc_reading = photoresistor_read(adc_channel);

    if (adc_reading > ADC_COUNTS) {
        adc_reading = ADC_COUNTS;
    }

    float voltage = ((float)adc_reading * ADC_VREF_VOLTS) / ADC_COUNTS;
    uint8_t sun_percent = (uint8_t)(((uint32_t)adc_reading * 100) / ADC_COUNTS);

    // First row: voltage
    LCD_cursor_set(1, 1);
    LCD_write_string("V:");
    LCD_write_float(voltage, 0, 2);
    LCD_write_string("V       ");

    // Second row: sunlight percentage
    LCD_cursor_set(2, 1);
    LCD_write_string("Sunlight:");
    LCD_write_variable(sun_percent, 0);
    LCD_write_string("%      ");
}

void photoresistor_init(void);
void ldr_error(int16_t *errorX, int16_t *errorY);
void horizontal_control(int16_t errorX);
void vertical_control(int16_t errorY);

void main(void)
{
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
        // read photoresistor
        ldr_error(&horizontalError, &verticalError);

        // servo control
        horizontal_control(horizontalError);
        vertical_control(verticalError);

        display_solar_values(SOLAR_PANEL_ADC_CHANNEL);

        __delay_ms(20);
    }
}
