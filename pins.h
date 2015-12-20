#ifndef PINS
#define PINS

#if defined( __AVR_ATtinyX41__ )
// Pin 0 - XTAL
// Pin 1 - XTAL
// Pin 2
#define XBEE_ENABLE 2
// Pin 3
#define MOTOR 3
// Pin 4 - MOSI
// Pin 5 - MISO
// Pin 6 - SCK
// Pin 7
#define PRESSURE 7
// Pin 8 - RX0
#define RTC_ALARM_PIN 8
// Pin 9 - TX0
// Pin 10
#define RTC_SS_PIN 10
// Pin 11 - RESET

#else
// DIGITAL I/O

// Pin 0 UART RX, PCINT16
// Pin 1 UART TX, PCINT17
// Pin 2 interrupt 0, PCINT18
#define RTC_ALARM_PIN 2
// Pin 3 interrupt 1, PCINT19, OC2B, PWM output

// Pin 4 PCINT20/XCK/T0
#define WARNLED 4
// Pin 5 PCINT21/OC0B/T1, PWM
#define XBEE_ENABLE 5
// Pin 6 PCINT22/OC0A/AIN0, PWM
#define MOTOR 6
// Pin 7 PCINT23/AIN1
#define PRESSURE 7
// Pin 8 PCINT0/CLK0/ICP1
#define RTC_SS_PIN 8
// Pin 9 PCINT1/OC1A, PWM
// Pin 10 default SS
// Pin 11 MOSI
// Pin 12 MISO
// Pin 13 SCK, LED
#endif
#endif

// PWM notes: http://playground.arduino.cc/Main/TimerPWMCheatsheet
