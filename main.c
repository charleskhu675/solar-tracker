/*
 * File:   main.c
 * Author: 
 *
 * Created on October 13, 2026, 2:33 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "servos.h"
#include "PIC18F46K22-Config.h"
#include "LCD.h"


#define DEADZONE 200
#define AZ_SPEED 25
#define SOLAR_PANEL_ADC_CHANNEL 11
#define ADC_COUNTS 1023
#define ADC_VREF_VOLTS 5.0

void photoresistor_init(void);
uint16_t photoresistor_read(uint8_t channel);
void horizontal_control(int16_t errorX);
void vertical_control(int16_t errorY);

void UART_Init(void) {
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;

    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;

    TXSTA1bits.SYNC = 0;
    TXSTA1bits.BRGH = 1;
    BAUDCON1bits.BRG16 = 1;

    SPBRGH1 = 0;
    SPBRG1 = 103;

    RCSTA1bits.SPEN = 1;
    TXSTA1bits.TXEN = 1;
}

void UART_Write(char data) {
    while (!PIR1bits.TX1IF);
    TXREG1 = data;
}

void UART_Text(const char *text) {
    while (*text) {
        UART_Write(*text++);
    }
}

void UART_PrintLDR_X(uint16_t TL, uint16_t TR, int16_t errorX) {
    char buffer[50];

    sprintf(buffer, "TL:%u TR:%u ErrorX:%d\r\n",
            TL, TR, errorX);

    UART_Text(buffer);
}

void UART_PrintLDR_Y(uint16_t BL, uint16_t BR, int16_t errorY) {
    char buffer[50];

    sprintf(buffer, "TL:%u TR:%u ErrorX:%d\r\n",
            BL, BR, errorY);

    UART_Text(buffer);
}

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

void main(void)
{
    // Initial servo 
    servo_set_azimuth_speed(0);
    servo_set_elevation_angle(0);
  
    // Initialize
    LCD_init();
    photoresistor_init();
    servo_init();
    UART_Init();
    
    uint16_t TL, TR, BL, BR;
    int16_t errorX, errorY;
    
    while (1)
    {
        
        TL = photoresistor_read(1);
        TR = photoresistor_read(2);
        BL = photoresistor_read(3);
        BR = photoresistor_read(4);

        errorX = ((int16_t)TL + BL - TR - BR) / 2;
        errorY = ((int16_t)TL + TR - BL - BR) / 2;
        
        UART_PrintLDR_X(TL, TR, errorX);
        UART_PrintLDR_Y(BL, BR, errorY);
        
        horizontal_control(errorX);
        vertical_control(errorY);
        __delay_ms(50);
        
        display_solar_values(SOLAR_PANEL_ADC_CHANNEL);
        
        __delay_ms(20);
    }
}

