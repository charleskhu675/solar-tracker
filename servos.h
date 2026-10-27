#ifndef SERVOS_H
#define SERVOS_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 16000000UL

// Pin Assignments:
// Azimuth (360 deg continuous) = RC2, Elevation (180 deg positional) = RC1
#define AZ_PIN   LATCbits.LATC2
#define EL_PIN   LATCbits.LATC1

// Calibration (us)
#define FRAME_US     20000U
#define AZ_STOP_US   1500U     // pulse width where 360 servo is still
#define AZ_SPAN_US   300U      // change in pulse width at 100% speed
#define EL_MIN_US    600U      // pulse width for 0 deg
#define EL_MAX_US    2400U     // pulse width for 180 deg

void servo_init(void);
void servo_set_azimuth_speed(int8_t percent);   // -100 (CCW) .. 0 (stop) .. +100 (CW)
void servo_set_elevation_angle(uint8_t angle);  // 0..180 degrees

#endif
