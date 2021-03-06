# Pneumatic Water Level Sensor

This is a playground for developing the SPI side of the Honeywell SSC/HSC series of pressure sensors with digital outputs.

The sketch operates as a periodic pressure reader, based on the "[Rain water tank level](http://playground.arduino.cc/Main/Waterlevel)" project in the Arduino playground. This implementation has two stages of operation: firstly charging the pressure tube, and secondly monitoring the pressure. The output presents pressure and sensor temperature in raw values reported by the sensor. The 5PSI sensor was selected as having a suitable range for measuring the water level in a 2m-high water tank.

The hardware used for developing this sketch:
 - Mitsumi MAP-1704 6VDC membrane pump
 - Honeywell HSCDANN005PGSA5 5psi temperature compensated SPI output pressure sensor
 - [SparkFun DeadOn RTC Breakout - DS3234](https://www.sparkfun.com/products/10160)
 - Arduino Uno SMD
 - 2N2222 PNP transistor
 - 1N4001 diode
 - 220Ω resistor
 - 3 x LED, various colours
 - 3 x 2k2Ω resistors (current limiting for LEDs)

The circuit is presented in the "Bubbler Electronics" Fritzing project.

NB: at this time there is no HSC sensor on the Fritzing project simply because the Fritzing component isn't available yet.

NB: these sensors are relatively expensive, costing about as much as the Arduino Uno itself.

NB: the MAP-1704 is easily capable of producing pressures that will burst the HSCDANN005PGSA5. The sensor's burst pressure is 40PSI, the pump is capable of 60PSI. Please be careful with your expensive sensors. This sketch pulses the motor and checks pressure readings after each short pulse, halting the charging process and lighting the warning LED if the pressure becomes too high.

# Setting up ZigBee radios

Remember to set the SP (Sleep Period) and SN (Number of Sleeps) parameters on the coordinator so that it will wait twice as long as your sensor will be sleeping.

If the sensor is going to sleep for three minutes, make sure (SP * SN * 10ms) > (3*2). This will reduce the amount of time you have to wake the XBee for in order to be able to send messages.
