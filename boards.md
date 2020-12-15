# Board layout

## Main digital board

Eurorack 5V power, 3V3 Vdd (from T3.6 onboard). T3.6, analog input conditioning (602 rail-to-rail op-amps), frequency measuring circuitry, LC/3.2 for display, OLED display, analog input for temp measurement. Encoder and LED for pitch offset. SPI outputs for DACs. Gate logic outputs. Pedal logic outputs. I2C for communication with LC/3.2 and to control LED PWM. Buttons and encoder for option selection and menu navigation. One for entire module.

## Gate and LED boards

5V power for LEDs, derived 8-10V power for gate, LEDs, transistors. One board does 4 gates. 2 boards for 8 voices. I2C logic level bidirectional converter.

## Analog power board

Eurorack +12V -12V for op-amps. Creates clean 5V5 from +12V for DACs. Powers pitch DACs, offset DAC, global DACs and performance DACs. One for entire module. Run across width of first 4 voice boards.

## Vref generator board

Small, insulated against drafts. Connections for +12/-12/Analog 0V; temperature sensor feed; bipolar power for heating; kelvin 10V and (multiple) 0V output connections. Test point for unbuffered 10V outputs.  One for entire module. Screened, insulated, off the back of the module somewhere.

## Vref distribution board

Trimmed divider from 10.0x00 V to 5V precision Vref.  Leads for front-panel trimmer for 5V. Provides 5 (9) onward-forwarding kelvin connections for the 5 (9) 16-bit DAC Vrefs. Also buffered output for front-panel 5V output. Also across the back of the first four voices (with connections for the other 4)? One for entire module.

## Pitch DAC boards

Two 18-bit pitch DACs; level shifter or opto-isolator for SPI connection; two Vref inputs; offset and gain pots per-DAC; two secondary pitch output mixers and input buffers. 5V5 from analog power board, +12/-12V for op-amps. Two for 4-voice, 4 for 8-voice. Front board for panel jacks with 90-degree soldered connection for DAC board.

## Offset DAC board

Similar circuitry to (part of) pitch DAC board. One DAC, different scaling, analog input buffer, mixing op-amp. 5V5 from analog power board, +12/-12V for op-amps. Maybe add the frequency (tuning) input and DAC (tuning) output jacks and buffers here too?  One for entire module.

## Performance DAC boards

One octal 14-bit DAC and octal buffer for performance DAC (one board does 2 voices; 2 boards for 4 voice,4 for 8 voice). Needs level shifters from 3V3 to 5V5 but less worries about digital feedthrough. Same board also used for global CV outputs. 5V5 from analog power board, +12/-12V for op-amps. Front board for panel jacks with 90-degree soldered connection for DAC board.

## Pedal output board (?)

Four on-off panel-mount 5V outputs for pedal CV; like gate board (but without LEDs?). Goes above global DAC board.

## Global DAC board

Try to re-use the perf DAC board entirely. 8 outputs.

## Off boards (panel mount)

- DIN MIDI input
- USB device input
- USB Host input
- Kelvin 4-pole output for 5V measurement? (Mini 4-pole XLR?)

Where do the input selection pushbuttons and leds go? What about the LCD?


