/*
Pneumatic water level sensor ("bubbler")

This sketch drives a pneumatic water level sensor. See the associated Fritzing file for the circuit design. The basic theory of operation is discussed in the "Rain water tank level" article of the Arduino playground[1].

For this sketch we are using an SPI connected sensor from the Honeywell TruStability range, model HSCDANN005PGSA5. This sensor has a nominal range of 0-5 PSI, reads "gauge pressure" (i.e.: it compares the pressure being read with ambient air pressure), is temperature compensated and has a rated accuracy of 0.25% across its operating range (which is 10%–90% of its nominal range). This component is expensive though: there are other similar devices available which cost a third the price.

How readings are taken:

1. Start off with a pressure reading.

2. Blow some air into the tube and stop the pump.

3. If the pressure in the tube doesn't change significantly (i.e.: by more than the margin of error of the sensor), consider the tube evacuated, and the air pressure to be at equilibrium with the water pressure.

4. If the pressure does change, go back to step 1.

Stopping the pump is necessary due to the noise generated in the sensor tube by the bursts of air from the diaphragm pump. A further exercise might be to find ways of taking meaningful samples while the pump is running.

Major Components:
 - MAP-1704 diaphragm pump (listed as "60kPa" capable)
 - HSCDANN005PGSA5 pressure senor
 - SparkFun DeadOn RTC (based on DS3234 SPI-connected clock with two alarms)

For details, see the associated Fritzing project, "Bubbler Electronics.fzz".

[1] http://playground.arduino.cc/Main/Waterlevel
*/

#include "pins.h"
#include <SPI.h>
#include <DSRTC.h>
#include <HSCDANN005PGSA5.h>
#include <DS3234.h>
#include <Sleep.h>

const int MIN_RAW = 1638;
const int MAX_RAW = 14746;
const int BUFF_MAX = 80;
const int MARGIN = (MAX_RAW - MIN_RAW) / 400; // 0.25%
const int HIGH_CUTOUT = MAX_RAW * 0.9;
const int PERIOD_MINUTES = 6;
volatile bool alarm = false;

HSCDANN005PGSA5 pressure_sensor(PRESSURE);
DS3234 rtc(RTC_SS_PIN, DS323X_INTCN || DS323X_EOSC);

inline void disablePin( int pin ) {
	pinMode( pin, INPUT );
	digitalWrite( pin, HIGH );
	}

inline void setupPins() {
	// Now configure the pins we're actually using
	pinMode(MOTOR, OUTPUT);
	pinMode(XBEE_ENABLE, OUTPUT);
	digitalWrite(XBEE_ENABLE, LOW);
	pinMode(WARNLED, OUTPUT);
	digitalWrite(PRESSURE, HIGH);
	pinMode(PRESSURE, OUTPUT);
	digitalWrite(RTC_SS_PIN, HIGH);
	pinMode(RTC_SS_PIN, OUTPUT);
	}

void wakeXBee() {
	digitalWrite(XBEE_ENABLE, HIGH);
	delay(100); // Allow XBee to finish waking up
	}

void sleepXBee() {
	delay(100); // Allow time for XBee to do housekeeping (even though it's supposed to finish transmitting before going to sleep after we assert SLEEP)
	digitalWrite(XBEE_ENABLE, LOW);
	}

inline void printDatestamp() {
	char buff[BUFF_MAX];
	dsrtc_calendar_t t;
	rtc.read(t);

	// Current time
	snprintf(buff, BUFF_MAX, "%d/%02d/%02dT%02d:%02d:%02d", t.Year, t.Month, t.Day, t.Hour, t.Minute, t.Second);
	Serial.print(buff);
	}

inline void printValues() {
	char buff[BUFF_MAX];
	tpElements_t rtc_temp;
	int raw_press = pressure_sensor.rawPressure();
	int raw_temp = pressure_sensor.rawTemperature();
	rtc.readTemperature(rtc_temp);
	snprintf(buff, BUFF_MAX, ", %d, %d, %d\.%d", raw_press, raw_temp, rtc_temp.Temp, rtc_temp.Decimal);
	Serial.println(buff);
	}

void printReading() {
	printDatestamp();
	printValues();
	Serial.flush();
	}

inline void getPressureReading() {
	boolean charging = 1;
	boolean highpressure = 0;
	int old_pressure; // pressures are raw readings ("counts") from HSC sensor
	int new_pressure;
	const int pump_duration = 250;
	const int pump_delay = 500;

	pressure_sensor.update();
	new_pressure = pressure_sensor.rawPressure();

	while (charging) {
		old_pressure = new_pressure;
		highpressure = (new_pressure > HIGH_CUTOUT);
		if (highpressure) {
			charging = 0;
			digitalWrite(WARNLED, HIGH);
			break;
			}
		else {
			digitalWrite(WARNLED, LOW);
			}
		if (charging) {
			digitalWrite(MOTOR, HIGH);
			delay(pump_duration);
			digitalWrite(MOTOR, LOW);
			delay(pump_delay); // allow for pump motor inertia, and escape of some bubbles
			}
		pressure_sensor.update();
		new_pressure = pressure_sensor.rawPressure();
		charging = (abs(new_pressure - old_pressure) > MARGIN);
		}
	pressure_sensor.stop();
	}

void INT0_ISR() {
	detachInterrupt(0);
	alarm = true;
	}

#if defined( __AVR_ATtinyX41__ )
ISR (PCINT0_vect) {
	GIMSK &= ~B00010000; // Disable PCI group 0
	alarm = true;
	}
#endif

void enableInterrupt() {
#if defined( __AVR_ATtinyX41__ )
	PCMSK0 |= B00000100; // Enable PCINT2, Arduino pin 8, ATTiny physical pin 11 "A2"
	GIMSK |= B00010000; // Enable PCI group 0
#else
	attachInterrupt(0, INT0_ISR, LOW);
#endif
	}

inline void setupRTC() {
	dsrtc_calendar_t time_buf;

	rtc.clearAlarmFlag(3);
	alarm = false;
	rtc.read(time_buf);
	time_buf.Minute += PERIOD_MINUTES;
	time_buf.Minute %= 60;
	rtc.writeAlarm(2, alarmModeMinutesMatch, time_buf);
	rtc.setSQIMode(sqiModeAlarm2);

	enableInterrupt();
	}

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println("Initialising.");
	setupPins();
	setupRTC();
	pressure_sensor.update();
	printDatestamp();
	Serial.println();
	wakeXBee();
	delay(10000); // Wait long enough for XBee to register on network
	getPressureReading();
	printReading();
}

void loop() {
	// put your main code here, to run repeatedly:
	if (alarm) {
		setupRTC();
		getPressureReading();
		wakeXBee();
		printReading();
	}
	sleepXBee();
	powerDown();
}
