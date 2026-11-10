#include <xc.h>
#include <stdint.h>
#include "servos.h"
#include "PIC18F46K22-Config.h"
#include "LCD.h"

#define _XTAL_FREQ 16000000UL
#define DEADZONE 20

void photoresistor_init() {
    ANSELA |= 0x0F;
    TRISA |= 0x0F;

    ADCON0 = 0x01;
    ADCON1 = 0x00;
    ADCON2 = 0b10101110;
}

uint16_t photoresistor_read(uint8_t channel) {
    ADCON0bits.CHS = channel;
    ADCON0bits.GO = 1;

    while (ADCON0bits.GO);

    return ((uint16_t)ADRESH << 8) | ADRESL;
}

void ldr_error(int16_t *errorX, int16_t *errorY) {
    uint16_t TL = photoresistor_read(0);
    uint16_t TR = photoresistor_read(1);
    uint16_t BL = photoresistor_read(2);
    uint16_t BR = photoresistor_read(3);

    *errorX = ((int16_t)TL + BL - TR - BR) / 2;
    *errorY = ((int16_t)TL + TR - BL - BR) / 2;
}

void horizontal_control(int16_t errorX) {
      if (errorX > DEADZONE) {
        servo_set_azimuth_speed(-30);
    }
    else if (errorX < -DEADZONE) {
        servo_set_azimuth_speed(30);
    }
    else {
        servo_set_azimuth_speed(0);
    }
}

void vertical_control(int16_t errorY) {
    static uint8_t angle = 90;

    if (errorY > DEADZONE) {
        if (angle < 180) {
            angle++;
        }
    }
    else if (errorY < -DEADZONE) {
        if (angle > 0) {
            angle--;
        }
    }
    servo_set_elevation_angle(angle);
}
