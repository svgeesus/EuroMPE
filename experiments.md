# Experiments

Breaking down to small, testable, breadboardable sub-tasks.

- build & test LM399 burn-in board *struggling to see, got microscope, waiting on parts*
- pitch DAC on breadboard [done](code/)
- control pitch DAC in 0.5V steps
- read encoder from Teensy LC/3.6 [done](code/EncoderTest/EncoderTest.ino)
- RGB LED, resistors and Teensy PWM
- RGB LED with offloaded 16-channel PWM chip from Adafruit *in progress*
    - needed different level shifter for I2C
    - I2C ok, PWM not ok, check output with scope
- octal DAC on breakout board
- DIN MIDI input, 3V3
- 8-10V gate out options: *5V not enough for Intellijel ADSR*
    - 74AHCT125 (5V only, so no)
    - **this** two-transistor high-side switch
    - LM324 comparator
    - [discussion of gate levels](https://www.muffwiggler.com/forum/viewtopic.php?p=468009&sid=1596d8bf763b7e4b2635537f46b4323f)
- kelvin ref driving with ground sense
- freqcount/freqmeasure input circuit
    needs 3V3 circuit, 74AHCT14 is 4.5 to 5.5V, check transistor bias resistor values.
- ISO7240CDWR optical SPI isolator on SOIC-16 breakout board
    - maybe isolator for USB device in, to isolate from laptop?
- I2C master/slave communication between Teensy LC and Teensy 3.6
- LCD menu system with LC/3.2
- test DipTrace.
   -  quick schematic for vref
    - and for pitch dac
    - try a dual offsets module (through-hole, existing components) with pcb.
        - put vref on daughter board with kelvin, so can have that option.
        - no need for precise 10V to 5V trimmed divider, stability more important than abs value here
    - pcb to osh park
- slew rate limiter test
    - prototype one-channel perf dac board on breadboard, measure.
    - lay out 8-channel perfdac board
- AD22103KTZ temp sensor to Teensy ADC



