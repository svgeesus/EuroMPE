# Gate and LED

Provides gate outputs and LED indication of gate status.

RGB LEDs give feedback on what note is playing.

## LEDs

[Diffused 'Piranha' Super-flux RGB LED](https://www.adafruit.com/product/1451) (got 10). 5mm height, 7.6mm square, common anode.

Red: 1.8-2.2V Forward Voltage, at 20mA current, Green: 3.0-3.3V, Blue: 3.0-3.3V

### LED interfacing

Two RGB LEDs need 6 channels. If LED on rotary encoder, needs 1 or 3 more.

PWM: native on Teensy, or use a PWM LED driver chip like:

- [PCA9685](https://www.adafruit.com/product/815) (16 channels, I2C)
- [TLC59711](https://www.adafruit.com/product/1455) (12 channels, SPI, 15mA, 5 to 17V.)

Mouser out of stock bare TLC59711 but carries the Adafruit boards which are 22.75mm x 28.38mm so minimum 6HP (30mm) for that part.

Voltage translation: SN74AHCT125 (up to 5V5) or LED driver does that too.
