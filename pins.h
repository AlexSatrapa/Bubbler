#ifndef PINS
#define PINS

// DIGITAL I/O

// Pin 0 UART RX, PCINT16
// Pin 1 UART TX, PCINT17
// Pin 2 interrupt 0, PCINT18
#define RTC_ALARM_PIN 2
// Pin 3 interrupt 1, PCINT19, OC2B, PWM output
#define MOTOR 3

// Pin 4 PCINT20/XCK/T0
#define WARNLED 4
// Pin 5 PCINT21/OC0B/T1, PWM
#define XBEE_ENABLE 5
// Pin 6 PCINT22/OC0A/AIN0, PWM
// Pin 7 PCINT23/AIN1
#define PRESSURE 7
// Pin 8 PCINT0/CLK0/ICP1
#define RTC_SS_PIN 8
// Pin 9 PCINT1/OC1A, PWM

inline void disablePin( int pin ) {
	pinMode( pin, INPUT );
	digitalWrite( pin, HIGH );
	}

inline void setupPins() {
	// Now configure the pins we're actually using
	disablePin( 1 );
	disablePin( 2 );
	pinMode(MOTOR, OUTPUT);
	pinMode(XBEE_ENABLE, OUTPUT);
	digitalWrite(XBEE_ENABLE, LOW);
	pinMode(WARNLED, OUTPUT);
	digitalWrite(PRESSURE, HIGH);
	pinMode(PRESSURE, OUTPUT);
	digitalWrite(RTC_SS_PIN, HIGH);
	pinMode(RTC_SS_PIN, OUTPUT);
	disablePin( 9 );
	}

#endif

// PWM notes: http://playground.arduino.cc/Main/TimerPWMCheatsheet
