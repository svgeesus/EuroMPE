# Duophonic MPE MIDI - Main MPU board.

## Power

Driven by Eurorack 5V power, generates logic 3V3 Vdd (from Teensy onboard, <250mA).

## MCU choice

MPU needs USB host and hardware floating point, so Teensy 3.6 (about to be discontinued) or **Teensy 4.1**. Both are 3V3-only.

T4.1 ($26.85) is somewhat overpowered but has 480Mbit/s on both USB and Host so potentially lower latency (T3.6 is 480 on Host only). T4.1 has 100mA power consumption at full clock speed though.

T3.6 ($29.25) has less memory, has 2 DAC outs (not needed here), power draw unspecified (@@measure it) but less than 4.1. Unobtainable due to NXP parts shortage, likely to be discontinued along with 3.5, 3.2 and LC!

The two are _somewhat_ pin-compatible. Where possible, pins below are chosen to be shared between the two MCUs. Lots of uncertainties though, test on breadboard before rolling a PCB and maybe stick with one MCU rather than trying to be compatible. Downside, running out of room on T3.6 means making a second board.

## MIDI

[DIN MIDI](https://www.pjrc.com/teensy/td_libs_MIDI.html) in needs serial input and 6N138 optoisolator with [1k pulldown](https://forum.pjrc.com/threads/54891-MIDI-serial-(DIN)-to-usbMIDI-issues-got-weird-MIDI-messages?p=287082#post287082)
and 4.7-10 kΩ base-emitter resistor. NO use 6N137, with transistor for level shift and inversion.

MIDI hardware spec states <5mA to turn on, < 2μs rise and fall times. Also pin 2 and shield go from not connected, to connected by 100nF caps for RF purposes.

Apparently 6N138 and or H11L1 are too slow per MIDI spec and 6N137 is better. These are not drop-in replacements, the support circuitry differs. H11L1 is slower, 0.1μs rise/fall while 6N137 is 23ns rise, 7ns fall.

> _(6N137 with)_ An NPN transistor with the collector pulled up to 3.3V. This will also give you a second inversion. The diagram below shows my circuit. The difference is you would have R41 pull up to 3.3V (not isolated 5V as in mine). I would probably make it 10K. C2 is a bypass cap, the datasheet recommends it. Put it as close to pins 8 and 5 as you can. [source](https://forum.pjrc.com/threads/66877-Teensy-4-0-serial-optocoupler-6n137s?p=276314&viewfull=1#post276314)

- [Opto-couple confusion – 6N137 or 6N138?](https://gr33nonline.wordpress.com/2019/05/15/opto-couple-confusion-6n137-or-6n138/)
- [Teensy 4.0 serial optocoupler 6n137s](https://forum.pjrc.com/threads/66877-Teensy-4-0-serial-optocoupler-6n137s)
- [MIDI Input with 6N137, 6N138, or 6N139](https://www.kieranreck.co.uk/blog/midi-input-with-6n137-6n138-or-6n139)

[Vishay 6N137](https://www.mouser.com/ProductDetail/Vishay-Semiconductors/6N137?qs=xCMk%252BIHWTZMrQz4FyDXhMg%3D%3D) PDIP8, $1.76/1.

- 00 RX1 (4.1, not 3.6), or
- 07 RX2/RX3 (same on 4.1 and 3.6)
_

[USB MIDI](https://www.pjrc.com/teensy/td_midi.html) & [USB Host MIDI](https://github.com/PaulStoffregen/USBHost_t36).

## SPI

Two [SPI](https://www.pjrc.com/teensy/td_libs_SPI.html) outputs for DACs (use the faster FIFO one for the  pitch DAC, because MPE pitchbend messages send a lot of data; use the second one for the octal performance DAC and octal CC DAC) with associated chip selects.

Note that second SPI channel (MOSI1 MISO1 SCLK1) in different place on T3.6 & T4.1! Pin numbers are 4.1/3.6 (if different).

- 11 MOSI
- _12 MISO not needed_
- 13 SCLK 
- ?? CS-Pitch (any convenient one, like 10 or 12)
- 26/00 MOSI1
- _39/01 MISO1 not needed_
- 27/32 SCLK1
- ?? CS-Perf (any convenient, 28)
- ?? CS-CC  (any convenient, 29)

Use pull-up resistors on chip selects, per 
[Better SPI Bus Design in 3 Steps](https://www.pjrc.com/better-spi-bus-design-in-3-steps/)


## UI: Display & Controls

### Display

Display: LC/3.2 for display?  if so, I2C (or CAN bus) for communication with LC/3.2.
Or use main MCU, and I2C for display. Slow, but high speed not needed. Remember the external pull-up resistors.

- 18 SDA = A4
- 19 SCL = A5

OLED display [Adafruit 128x32 I2C OLED](https://www.adafruit.com/product/4440) might be too small, but the bigger displays are SPI only. 20mA.35mm by 20mm.
Or [Adafruit 128x64 I2C OLED](https://www.adafruit.com/product/938) 40mA.

> I found that with both u8g2 and Adafruit SSD1306, I was getting extremely slow frame updates. 37ms with u8g2 and 25ms with Adafruit SSD1306. By changing the bus clock speed (setBusClock(1000000) for u8g2, constructor argument for Adafruit), my update time went to 8us and 2us respectively. The frame is doing a clear, drawing a single line of text (which moves over time), and then updates.
[I2C OLED performance on Teensy 4.0 with different clock speeds](https://forum.pjrc.com/threads/61060-I2C-OLED-performance-on-Teensy-4-0-with-different-clock-speeds)

### Encoder, buttons

Buttons and [encoder](https://www.pjrc.com/teensy/td_libs_Encoder.html) for option selection and menu navigation. T3.6 & T4.1 can use any pins for encoders.

- ?? Encoder-A
- ?? Encoder-B
- ?? Encoder-switch
- ?? Button-1
- ?? Button-2
- ?? Button-3
- ?? Button-4

Use encoder switch as OK, needs anther button for "back".
Also needs "Calibrate" "Tune" and "Setup" ? Single button for common functions.

### Mode switch(es)

Unclear if MPE/non-MPE is needed.

Duophonic/Unison switch at minimum, perhaps Duophonic/Unison/Harmonic or use another switch (plus the encoder) for Harmonic. Double press to reset.

### Non-MPE channel selector

Could hide this in a menu; main usage is MPE. Also hide MPE behaviour on split (unlikely) in a setup menu.

## Frequency measurement

Frequency measuring circuitry for auto calibration. How to select channel? Should there be a 440Hz reference output?

> FreqCount: best for 1 kHz to 8 MHz (up to 65 MHz with Teensy 3.0 & 3.1)
> FreqMeasure: best for 0.1 Hz to 1 kHz

Test how low FreqCount can go, or connect both circuits and choose between inputs depending on result.

- 9 [FreqCount](https://www.pjrc.com/teensy/td_libs_FreqCount.html)
- 22 [FreqMeasure](https://www.pjrc.com/teensy/td_libs_FreqMeasure.html)

## Gate outs

Gate logic outputs. PWM for two gate LEDs. Check current draw is okay. Might still be better to use an external 6+ channel PWM chip; then needs I2C connection though.

- 02 Gate1
- 03 Gate 2
- 04 LED1 PWM R
- 05 LED1 PWM G
- 06 LED1 PWM B
- 09  **conflict with T4.1 FreqCount**
- 36 LED1 PWM R
- 37 LED1 PWM G
- 29 LED1 PWM B


**No** Alternatively, use chainable [2mm I2C DotStar 2020 RGB LEDs](https://www.adafruit.com/product/3341) or, larger, [5mm DotStar 5050](https://www.adafruit.com/product/2343) which do their own PWM. 5V though, needs 74AHCT125  or similar level shifter.

- 18 SDA
- 19 SCL
- 20 Gate1
- 21 Gate2

## Digital outs

Two pedal logic outputs, if desired.

- ?? Pedal 1
- ?? Pedal 2

## Case & Osc temp monitoring (?)

Could need analog ins, or I2C, depending on hardware. Not at all a must-have.

## Development & Testing plan

1. T4.1 on breadboard, power from +5V, measure current consumption
2. Test I2C display, see if update speed okay and feasible for menus
3. Test PWM of two RGB LEDS. Test 10V gate output. Fabricate [Gate-LED](./Gate-LED.md) board.
4. Test FreqCount & FreqMeasure
5. Test DIN MIDI input
6. Sketch front panel to get PCB dimensions.
7. Fabricate & test octal perfDAC/CCDAC board (same board, different CS)
8. Fabricate pitch CV board, test
9. Write higher-level, MPE-capable MIDI library
10. Fabricate MCU and display carrier board
11. Front panel
