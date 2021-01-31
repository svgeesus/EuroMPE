# Main MPU board.

## Power

Driven by Eurorack 5V power, generates logic 3V3 Vdd (from Teensy onboard, <250mA).
## MCU choice

MPU needs USB host and hardware floating point, so Teensy 3.6 or Teensy 4.1. Both are 3V3-only.

T4.1 ($26.85) is somewhat overpowered but has 480Mbit/s on both USB and Host so potentially lower latency (T3.6 is 480 on Host only). T4.1 has 100mA power consumption at full clock speed though.

T3.6 ($29.25) has less memory, has 2 DAC outs (not needed here), power draw unspecified but less than 4.1.

The two are somewhat pin-compatible. Where possible, pins below are chosen to be shared between the two MCUs. Lots of uncertainties though, tst on breadboard before rolling a PCB and maybe stick with one MCU rather than trying to be compatible. Downside, running out of room on T3.6 means making a second board.

## MIDI

DIN MIDI in needs serial input. RX1/TX1 hard to use on a dual-function T3.6/T4.1 design.

- 07 RX2/RX3
- 08 TX2/TX3 _not used here_

USB MIDI & USB Host MIDI.

## SPI

Two [SPI](https://www.pjrc.com/teensy/td_libs_SPI.html) outputs for DACs (use the faster FIFO one for the two pitch DACs, because MPE pitchbend messages send a lot of data; use the second one for the octal performance DAC and octal CC DAC) with associated chip selects.

Note that second SPI channel (MOSI1 MISO1 SCLK1) in different place on T3.6 & T4.1! Pin numbers are 4.1/3.6 (if different).

- 11 MOSI
- 12 MISO _not needed_
- 13 SCLK (or on 3.6, 14 for no LED, frees up 13 for FreqCount)
- ?? CS-Pitch1
- ?? CS-Pitch2
- 26/00 MOSI1
- 39/01 MISO1 _not needed_
- 27/32 SCLK1
- ?? CS-Perf
- ?? CS-CC

## Display

Display: LC/3.2 for display?  if so, I2C (or CAN bus) for communication with LC/3.2.
Or use main MCU, and I2C for display. Slow, but high speed not needed. Remember the external pull-up resistors.

- 18 SDA
- 19 SCL

OLED display [Adafruit 128x32 I2C OLED](https://www.adafruit.com/product/4440) might be too small, but the bigger displays are SPI only. 20mA.
Or [Adafruit 128x64 I2C OLED](https://www.adafruit.com/product/938) 40mA.

> I found that with both u8g2 and Adafruit SSD1306, I was getting extremely slow frame updates. 37ms with u8g2 and 25ms with Adafruit SSD1306. By changing the bus clock speed (setBusClock(1000000) for u8g2, constructor argument for Adafruit), my update time went to 8us and 2us respectively. The frame is doing a clear, drawing a single line of text (which moves over time), and then updates.
[I2C OLED performance on Teensy 4.0 with different clock speeds](https://forum.pjrc.com/threads/61060-I2C-OLED-performance-on-Teensy-4-0-with-different-clock-speeds)

Buttons and [encoder](https://www.pjrc.com/teensy/td_libs_Encoder.html) for option selection and menu navigation. T3.6 & T4.1 can use any pins for encoders.

?? Encoder-A
?? Encoder-B
?? Encoder-switch
?? Button-1
?? Button-2
?? Button-3
?? Button-4

## Frequency measurement

Frequency measuring circuitry for auto calibration.

> FreqCount: best for 1 kHz to 8 MHz (up to 65 MHz with Teensy 3.0 & 3.1)
> FreqMeasure: best for 0.1 Hz to 1 kHz

Test how low FreqCount can go, or connect both circuits and choose between inputs depending on result.

- 9/13 [FreqCount](https://www.pjrc.com/teensy/td_libs_FreqCount.html)
- 3/22 [FreqMeasure](https://www.pjrc.com/teensy/td_libs_FreqMeasure.html)

## Gate outs

Gate logic outputs. PWM for two gate LEDs. Choose pins carefully so PWM-capable on both 4.1 and 3.6. Check current draw is okay. Might still be better to use an external 6+ channel PWM chip; then needs I2C connection though.

- 02 Gate1
- 03  **conflict with T4.1 FreqMeasure**
- 04 LED1 PWM R
- 05 LED1 PWM G
- 06 LED1 PWM B
- 09  **conflict with T4.1 FreqCount**
- 36 LED1 PWM R
- 37 LED1 PWM G
- 29 LED1 PWM B
- 30 Gate2

## Digital outs

Two pedal logic outputs, if desired.

- ?? Pedal 1
- ?? Pedal 2

## Case & Osc temp monitoring (?)

Could need analog ins, or I2C, depending on hardware. Not at all a must-have.

## Development & Testing plan

1. T3.6 on breadboard, power from +5V, measure current consumption
2. Test I2C display, see if update speed okay and feasible for menus
3. Test PWM of two RGB LEDS. Test 10V gate output. Fabricate board.
4. Test FreqCount & FreqMeasure
5. Test DIN MIDI input
6. Fabricate & test octal perfDAC/CCDAC board (same board, different CS)
7. Fabricate pitch CV board, test
8. Write higher-level, MPE-capable MIDI library
9. Fabricate MCU carrier board
