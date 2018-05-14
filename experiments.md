# Experiments

Breaking down to small, testable, breadboardable sub-tasks.

- build & test LM399 burn-in board *struggling to see, got microscope, waiting on parts*
- pitch DAC on breadboard [done](code/)
- control pitch DAC in 0.5V steps
- read encoder from Teensy LC/3.6 [done](code/EncoderTest/EncoderTest.ino)
- RGB LED, resistors and Teensy PWM
- RGB LED with offloaded 16-channel PWM chip from Adafruit
- octal DAC on breakout board
- DIN MIDI input, 3V3
- 8-10V gate out options:
    - 74AHCT125 (5V only, so no)
    - [this] two-transistor high-side
    - LM324 comparator
    - [discussion of gate levels](https://www.muffwiggler.com/forum/viewtopic.php?p=468009&sid=1596d8bf763b7e4b2635537f46b4323f)
- kelvin ref driving with ground sense
- freqcount/freqmeasure input circuit
- ISO7240CDWR optical SPI isolator on SOIC-16 breakout board
- I2C master/slave communication between Teensy LC and Teensy 3.6
- test DipTrace.
    quick schematic for vref
    and for pitch dac
    try and offsets module (through-hole, existing components) with pcb put vref on daughter board with kelvin so can have that option.
    pcb to osh park


