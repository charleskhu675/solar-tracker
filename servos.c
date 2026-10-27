#include "servos.h"
#include "PIC18F46K22-Config.h"

// Pin Assignments:
// Azimuth (360 deg continuous) = RC2, Elevation (180 deg positional) = RC1

static volatile uint16_t az_us = AZ_STOP_US;
static volatile uint16_t el_us = 1500U;

void __interrupt() isr(void) {
    static uint8_t state = 0;
    uint16_t t;

    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;

        if (state == 0) {
            // start azimuth pulse
            AZ_PIN = 1;
            t = 65536UL - az_us;
            state = 1;
        }
        else if (state == 1) {
            // end azimuth, start elevation pulse
            AZ_PIN = 0;
            EL_PIN = 1;
            t = 65536UL - el_us;
            state = 2;
        }
        else {
            // end elevation, wait for rest of 20 ms period
            EL_PIN = 0;
            t = 65536UL - (20000 - az_us - el_us);
            state = 0;
        }
        
        // Reload Timer1
        TMR1H = t >> 8;
        TMR1L = t & 0xFF;
    }
}

void servo_init(void) {

    // Set Internal Oscillator to 16 MHz
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
    
    // Configure RC2/RC1 as digital outputs
    TRISCbits.TRISC2 = 0;           // Azimuth servo
    TRISCbits.TRISC1 = 0;           // Elevation servo
    ANSELC = 0x00;
    LATC = 0x00;

    // Timer1 -> Fosc/4 source, 1:4 prescale (1 us tick), 16-bit read/write, timer off
    // Timer1: {TMR1CS<1:0>. T1CKPS<1:0>, T1SOSCEN, ~T1SYNC, T1RD16, TMR1ON} = 0b00100010 = 0x22 
    T1CON  = 0x22;
    TMR1H = 0xFF;
    TMR1L = 0x00;

    PIR1bits.TMR1IF = 0;        // clear old flags
    
    PIE1bits.TMR1IE = 1;        // enable interrupts
    INTCONbits.PEIE = 1;        // enable peripheral interrupts
    INTCONbits.GIE  = 1;        // enable global interrupts
    
    T1CONbits.TMR1ON = 1;
}

// continuous-rotation servo -> pulse width sets speed/direction
void servo_set_azimuth_speed(int8_t percent) {
    if (percent < -100)  percent = -100;
    if (percent > 100) percent = 100;

    uint16_t us = (uint16_t)((int16_t)AZ_STOP_US +
                             ((int16_t)percent * (int16_t)AZ_SPAN_US) / 100);

    INTCONbits.GIE = 0;             // ensures interrupts do not fire mid-write
    az_us = us;
    INTCONbits.GIE = 1;
}

// positional servo -> 0-180 deg maps to EL_MIN_US-EL_MAX_US
void servo_set_elevation_angle(uint8_t angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    uint16_t us = EL_MIN_US +
                  (uint16_t)(((long)angle * (EL_MAX_US - EL_MIN_US)) / 180);

    INTCONbits.GIE = 0;
    el_us = us;
    INTCONbits.GIE = 1;
}
// Example Demo
/* void main(void)
{
    servo_init();

    servo_set_azimuth_speed(0);
    servo_set_elevation_angle(90);
    __delay_ms(1000);

    while (1) {
        uint8_t el_angle;

        // Sweep forward: azimuth rotates CW slowly, elevation 0 -> 180
        servo_set_azimuth_speed(60);
        __delay_ms(750);
        servo_set_azimuth_speed(0);
        __delay_ms(50);
        
        for (el_angle = 90; el_angle > 0; el_angle -= 5) {
            servo_set_elevation_angle(el_angle);
            __delay_ms(50);
        }
        
        servo_set_azimuth_speed(-60);
        __delay_ms(500);
        servo_set_azimuth_speed(0);
        __delay_ms(50);
        
        for (el_angle = 0; el_angle < 90; el_angle += 5) {
            servo_set_elevation_angle(el_angle);
            __delay_ms(50);
        }
        
    }
}
*/
