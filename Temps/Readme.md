# Temps

A multichannel temperature monitoring and datalogging module in 4HP (20mm) or 5HP (25mm). Reads sensors mounted directly on the back of oscillators.

Small, vertical-mounted screen displays currently selected value and stripchart of trend over time.

Pushbuttons select displayed channel. 6 for temps, plus +12V -12V +5V and a reset. Originally thought of RGB LED buttons, to give feedback on status. If non-illuminated then maybe a status RGB LED for green/amber/red temperature stability?

Teensy-LC is fine for the logging, SPI or I2C temp sensors plus display on the other SPI bus; RGB LEDs requires 27 pwm channels, seems like a lot? Depends on availability of RGB LED tact buttons.

Micro-USB on panel for serial datalogging of timestamped measurements.

- Six channels of temperature sensors (4 VCOs, PSU, case for example)
- Voltage monitoring of the three rails
- Relative humidity and atmospheric pressure (not on display, just logged)

## Display

Maybe [Monochrome 128x32 SPI OLED graphic display](https://www.adafruit.com/product/661). Board is 23mm so too wide for 4HP, fits in 5HP. 33mm long, so leaves 77mm for switches and USB. Actual display area 7mm x 25mm, 32x128 pixels, monochrome OLED. SSD1306 driver chip. Uses around 20mA.

## Buttons

Adafruit only has [non-illuminated tact switches](https://www.adafruit.com/category/759) or [huge illuminated switches](https://www.adafruit.com/category/758)

## USB

Maybe [Panel Mount Extension USB Cable - Micro B Male to Micro B Female](https://www.adafruit.com/product/3258) 25.3mm wide. Still not fitting in 4HP or even 5HP. Or [MUSB-K152-30 ](https://eu.mouser.com/ProductDetail/Amphenol-Commercial-Products/MUSB-K152-30?qs=sGAEpiMZZMulM8LPOQ%252bykyVH%2fhPQ59j80dFehTRqUy4%3d) 20mm wide, has 90-degree PCB mount connection.

## Temperature sensors

[TMP36 - Analog Temperature sensor](https://www.adafruit.com/product/165) $1.50

- analog, 10 mV/°C scale factor (20 mV/°C on TMP37)
- ±2°C accuracy over temperature (typ)
- ±0.5°C linearity (typ)

Needs analog input per channel, so 6 of them. Accuracy seems poor. Cheap.

[DS18B20 Digital temperature sensor](https://www.adafruit.com/product/374) $3.95

- digital (onewire)
- -55 to 125°C
- ±0.5°C from -10°C to +85°C
- 9 to 12 bit selectable resolution

Check the Dallas Temp library plus the Oneire library actually fits on a Teensy LC :(

## Relative Humidity

Maybe [Adafruit BME280 I2C or SPI Temperature Humidity Pressure Sensor](https://www.adafruit.com/product/2652) $19.95. Board is 19.0 x 18.0mm. Bosch BME280 sensor (I2C or SPI) has

- humidity with ±3% accuracy
- barometric pressure with ±1 hPa absolute accuracy, 12 Pascal resolution
- temperature with ±1.0°C accuracy (use for ambient case temp)

Or (humidity and temperature, no pressure)  [Adafruit Sensirion SHT31-D - Temperature & Humidity Sensor](https://www.adafruit.com/product/2857) $13.95. Board 12.7mm x 18mm.

- relative humidity ±2% accuracy
- temperature  ±0.3°C accuracy

## Atmospheric pressure

Seems newer than BME280, also cheaper, but no humidity) [Adafruit BMP388 - Precision Barometric Pressure and Altimeter](https://www.adafruit.com/product/3966) $9.95. Board is 21.6mm x 16.6mm. Bosch BMP388 sensor (I2C or SPI) has

- barometric pressure with  ±1 hPa absolute accuracy, 8 Pascal resolution, 300 hPa to 1250 hPa
- temperature with ±0.5°C accuracy,  0-65°C

Looks like SHT31-D for humidity and temp, plus BMP388 for pressure (and check on temp) is best solution.

## Voltage rails

A 10V ref plus some op-amps to shift and scale to a 0 to 3V3 range. Jumpers to switch between rails and test points, for calibration. Software calibration based on known voltages.

Duplicates functionality of an existing module, but may be better for logging.