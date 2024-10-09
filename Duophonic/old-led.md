# Older musings on RGB LED options

## Introduction

PWM: native on Teensy, or use a PWM LED driver chip like:

- [PCA9685](https://www.adafruit.com/product/815) (16 channels, I2C)
- [TLC59711](https://www.adafruit.com/product/1455) (12 channels, SPI, 15mA, 5 to 17V.)
- [NCP5623](https://www.mouser.com/ProductDetail/onsemi/NCP5623DTBR2G?qs=atIEnC%2F2K4UXlMf9SxIM5g%3D%3D) (3 channels, 5bit, I2C, fixed address)

Mouser out of stock bare TLC59711 but carries the Adafruit boards which are 22.75mm x 28.38mm so minimum 6HP (30mm) for that part.

Voltage translation: SN74AHCT125 (up to 5V5) or 74HCT595, or LED driver does that too.

Notice [this thread](https://forum.pjrc.com/threads/28460-teensy3-1-tlc59711-16-bit-led-driver-timing-issue-and-flicker?highlight=TLC59711) on using the Adafruit TLC59711 library. [Alternate library](https://github.com/ulrichstern/Tlc59711) with [wiki notes](https://github.com/ulrichstern/Tlc59711/wiki#electronics-notes) uses transactions, does not use old SPI clock multiplier.

Also [this thread](https://forum.pjrc.com/threads/24599-IntervalTimer-and-LED-fading?highlight=TLC59711) on why Teensy PWM is better than a PWM chip; uses an op-amp integrator to smooth the PWM signal and to provide the current drive rather than getting it from the Teensy. LMV358 SOIC-8 dual op-amp is [cheap](https://www.mouser.com/ProductDetail/Diodes-Incorporated/LMV358SG-13) at $0.414/10, need 3 for 2 RGB LEDs.

## Pirhana with PWM

[Diffused 'Piranha' Super-flux RGB LED](https://www.adafruit.com/product/1451) (got 10). 5mm height (2.5mm shoulder plus 2.5mm dome; but panel is 2mm so at least gate jacks will need spacer washers), 7.6mm square, common anode.

Red: 1.8-2.2V Forward Voltage, at 20mA current, Green: 3.0-3.3V, Blue: 3.0-3.3V

[Constant current LED with op-amp](https://www.allaboutcircuits.com/technical-articles/the-basics-behind-constant-current-led-drive-circuitry/)

- [How to Hook up (common anode) RGB LEDs](https://forum.pjrc.com/threads/73390-How-to-Hook-up-RGB-LEDs-with-a-Proprietary-Switch-Circuit)

The [straightforward solution](https://www.pjrc.com/teensy/tutorial2.html) works fine with no visible flicker; just modified to common anode so the common pin goes to 3V3 not ground, and the PWM gives brightest light at zero not 255. But better to increase the PWM resolution to 10 bits.
Can still be a problem with PWM not fully going to zero. Workaround:

```C
if( PWM == 0){ digitalWriteFast( PIN , LOW ); }
```

Scope shows PWM duty cycle varies between 0.67% and 99.82% at 146.48kHz

> Red: 800 mcd typical brightness, Green: 1500 mcd, Blue: 700 mcd (3000 mcd total)

So scale resistors appropriately, 270R is too bright for green.

**decided to use through-hole 5mm NeoPixels instead** - one pin instead of six