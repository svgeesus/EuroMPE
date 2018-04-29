# Euro-MPE notes

Goal is a Multidimensional Polyphonic Expression (MPE) implementation for Eurorack, with a focus on
high precision CV generation (especially for pitch) and complete MIDI 1.0 & MPE implementation.

Secondary aim is one-control-per-function, avoiding menu diving for normal operation.

Per-note, that means:

- 16bit pitch CV (for oscillators) from NoteOn, accurate to better than 1 cent over 10 octaves. -3V to +8V
- secondary pitch output (for filters) with both global offset and per-note CV input
- gate and LED from NoteOn/NoteOff
- 4 x 14bit CV for Attack Velocity, Release Velocity, Aftertouch and Glide

Initial design gives four-note polyphony; expander adds another 4 notes to max of 8. (Spec limit is 15, but that seems way high for Eurorack). Plan to build as 4-voice.

## Pitch accuracy and error budget

MPE 14bit glide with 48 semitone range is 4800/16384 = 0.29 cent resolution (244μV).

At ±5V, 16bit, 1LSB = 152μV. 833μV is 1 cent at 1V/oct (5.4 LSB).
    Aim is better than 2LSB (300 μV, 60 ppm, 0.4 cent) overall system performance. Idealy 1 LSB so better than 0.29 cent, but that seems hard.

## Voltage ref

See [Voltage ref](voltage-ref-LM399.md) precise 5.000 0V ref, stable over time and temperature better than 25ppm (125 μV).

## DAC driving

Use one SPI channel for the pitch DACs and a second channel for the performance DACs. There is a third SPI channel, but it is inconvenient to use because it connects via the inner Teensy pads, not the outer pins.

## Pitch DAC

See [Pitch DAC](pitch-dac.md) 16-bit high linearity DAC per voice, with 10 octave (10V) range.


## Secondary pitch output

Global pitch shift (via another AD5542CRZ board) is combined [OP-glob-A, Vishay or LT .01% or .025% quad resistor array] with the main pitch output and a buffered [OP-glob-B, half a quad resistor array] per-note external input to give a secondary pitch output for driving filters.

Pitch shift selected by a rotary encoder with LED and pushbutton. Left to decrease, right to increase in 1 semitone steps and push to reset to zero. I2C is fine as this is low speed, manual use and not rapid use.
[Rotary Encoder - Illuminated (RGB)](https://www.sparkfun.com/products/10982)
[SparkFun Rotary Encoder Breakout](https://www.sparkfun.com/products/11722)
[Teensy Encoder Library](https://www.pjrc.com/teensy/td_libs_Encoder.html)
and an option for hardware quadrature decode
[Hardware Quadrature Code for Teensy 3.x](https://forum.pjrc.com/threads/26803-Hardware-Quadrature-Code-for-Teensy-3-x)


Ideally the external input would have an attenuator, except that needs to be the same value for all 8 voice channels. This can have a slightly more relaxed error budget. Options:

- include a per-note VCA, drive with the Teensy DAC out?
- use digital pots? No, only 5V operation
- rely on an external 2,4,6,8 channel VCA with a single DAC providing the CV to all VCAs. And external multichannel CV mixers.

Or, assuming one of the performance controls is connected to the external input, attenuate those instead:

- rely on digital-domain attenuation on the 14bit per-note outputs (probably sufficient).
- use digital pots? No, lots of pots (4 per note)

## Gate output

Requires 8-10V drive to a gate output; also switch to allow +5V to flow through an LED as indicator (both high side swiching, npn driving pnp). Driven from 3V3 logic high, transition should be 1.5 to 2V. 3-terminal variable regulator to give 6 to 10V from +12V rail.

- [Hinton two-transistor gate, no LED](https://www.muffwiggler.com/forum/viewtopic.php?p=2720659#2720659)
- [PNP high-side switching](http://www.w9xt.com/page_microdesign_pt12_hv_pnp_switching.html)
- [gate buffers](https://synthnerd.wordpress.com/2016/03/17/synth-diy-gate-buffer/)

Pirhana diffuse 5mm rgb, through hole, 7.6mm sq, 20mA max,  $9.95 for 10
experiment, likely good at half power. 3 pins per led.
fixed resistors better than pwm in terms of interference.
8 colors for the 8 voices, easier to set up in software to allow for 4-voice or 8-voice builds.
10mA is 320R red, 200R green & blue. Adjust for desired color.
Or use an external 16-channel PWM chip driven over I2C to offload PWMing the LEDs.

## Performance (Control Change) DAC

See [Performance DAC](performance-dac.md). Provides four performance controls per voice (attack velocity, release velocity, pressure, glide) with 0 to 5V at 12-13ENOB.


## Display, UI

Aim for direct manipulation style. Normal operation requires no menu diving.

Display used for tuning, calibration, and setting of uncommon options.

Simplest to use a second Teensy (LC, or possibly 3.2) to drive the display and handle user interaction. That keeps the code and control flow separate, also frees up an SPI bus for controlling the display. Use I2C or Serial to send commands to the main Teensy 3.6, primarily to go into calibration mode.

The 3.2/LC can also handle oven control duties if those are needed.


## MIDI

Full MIDI 1.0 (including tuning control) and MPE implementation at 14bit accuracy.
See [MIDI notes](midi-notes.md)

## Calibration

Initial setup (trimming), oscillator tuning. See [Calibration details](calibration.md).
