# Euro-MPE notes

Goal is a Multidimensional Polyphonic Expression (MPE) implementation for Eurorack, with a focus on
high precision CV generation (especially for pitch) and complete implementation.

Per-note, that means:

- 16bit pitch CV (for oscillators) from NoteOn, accurate to better than 1 cent over 10 octaves. -3V to +8V
- secondary pitch output (for filters) with both global offset and per-note CV input
- gate and LED from NoteOn/NoteOff
- 4 x 14bit CV for Attack Velocity, Release Velocity, Aftertouch and Glide

Initial design gives two-note polyphony; expanders add another 2 notes each to max of 8. (Spec limit is 15, but that seems way high for Eurorack). Plan to build as 4-voice.

## Voltage ref

See [Voltage ref](voltage-ref.md)


## Pitch DAC

See [Pitch DAC](pitch-dac.md)


## Secondary pitch output

Global pitch shift (via another AD5542CRZ board ) is combined [OP-D, Vishay .01% quad resistor array] with the main pitch output and a buffered [OP-E] per-note external input to give a secondary pitch output for driving filters.

Pitch shift selected by a rotary encoder with LED and pushbutton. Left to decrease, right to increase in 1 semitone steps and push to reset to zero. I2C is fine as this is low speed, manual use and not rapid use.
[Rotary Encoder - Illuminated (RGB)](https://www.sparkfun.com/products/10982)
[SparkFun Rotary Encoder Breakout](https://www.sparkfun.com/products/11722)
[Teensy Encoder Library](https://www.pjrc.com/teensy/td_libs_Encoder.html)
and an option for hardware quadrature decode
[Hardware Quadrature Code for Teensy 3.x](https://forum.pjrc.com/threads/26803-Hardware-Quadrature-Code-for-Teensy-3-x)


Ideally the external input would have an attenuator, except that needs to be the same value for all 8 voice channels. This can have a slightly more relaxed error budget:

- include a per-note VCA, drive with the Teensy DAC out?
- use digital pots? No, only 5V operation
- rely on an external 2,4,6,8 channel VCA with a single DAC providing the CV to all VCAs. And external multichannel CV mixers.

Or, aassuming one of the performance controls is connected to the external input
- rely on digital-domain attenuation on the 14bit per-note outputs (probably sufficient).
- use digital pots? No, lots of pots (4 per note)

Use 4 pots with rail-to-rail buffer amps connected to 4 adc inputs. Use to digitally scale the 14bit performance values (on all voices).

- [Alpha 9mm T18 shaft pot, 10k](https://www.thonk.co.uk/shop/alpha-9mm-pots-vertical-t18/)
- [T18 micro knobs](https://www.thonk.co.uk/shop/micro-knobs/)

## Gate output

- [Hinton two-transistor gate, no LED](https://www.muffwiggler.com/forum/viewtopic.php?p=2720659#2720659)

## Performance (Control Change) DAC

See [Performance DAC](performance-dac.md)

### Power

Handles 5.5V so can use same supply as pitc DACs.

### Digital connection

Needs level shifter for SPI. Use second SPI channel on Teensy 3.6. One quad shifter handles 2 CS plus SCLK and MOSI - enough for base board 2 voices and global CC board. A second optional shifter gives 4 more CS of which 3 are used for voices 3-4, 5-6 and 7-8.

### Initial accuracy

INL ±1 / ±4 LSB

DNL ±0.1 / ±0.5 LSB

Gain error 500μV / 7.5mV

Zero error 	1mV / 4mV

At 0..5V, 1LSB is 306μV. INL implies 13ENOB (612μV). For standard resolution 7bit data, we only need 2^6 = 64 LSB accuracy which is 19.5mV. Especilly due to the huge max zero-scale offset, we are getting 11-12 ENOB with these mV offsets. TL071 with 3mV offset is now significant wrt typical (but not max) offsets.

DAC is not trimmable without external conditioning circuitry.

Long term drift (Fig.7) inside ±100ppm/2k hours

### Line regulation

10μV/V is fine here

### Load

10μV/mA

Internally buffered, but poor load regulation when sinking current; will need external buffer to protect from modular environment (like getting 12V plugged into an output by mistake) and to give current drive.

Consider LT1214 (100μV, $5.85 quad, PDIP-14 or SOIC-14) or OPA4172 (200μV, $3.82 quad, SOIC-14) as non-inverting output buffers. Use innie current limiting resistor. Maybe time to try a SMD board design?

### Vref

Internal 2.5V VRef has good remperature stability (especially C grade) and long-term drift. External Vref probably not needed.



## Display, UI

Simplest to use a second Teensy (LC, or possibly 3.2) to drive the display and handle user interaction. That keeps the code and control flow separate, also frees up an SPI bus for controlling the display. Use I2C or Serial to send commands to the main Teensy 3.6, primarily to go into calibration mode.

The 3.2/LC can also handle oven control duties.


## MIDI

See [MIDI notes](midi-notes.md)

## Calibration

Several types of calibration:

### Initial setup trimming

Ensure trimmers are on front panel so unit can be trimmed while in the rack, powered and warm. Firstly setting precise 5V output from reference; then setting offset (by adjusting the 2V offset reference) and gain on each pitch DAC. The 14bit outputs are untrimmed, 0-5V within 5-10mV is fine.

Vref needs extensive trimming and measurement (for ageing) outside case anyway. Provide a buffered +5V jack on front panel, offset trimmer near it.

Needs outputs at 0V (before offset, so provide test point? or trim outside case as a first check), -2V (= 0V after offset, for offset trim), -4.8 = -2.8V, +4.8 = +6.8V (to avoid DAC extremes where zero and FS offset complicate gain setting, for gain trim).

Global offset also needs an output, for calibration plus handy to expose for other patching.

### Per-voice oscillator calibration

Avoid non-linear calibration curves. These are fine for pulling poorly-tracking oscillators into tune but then the same errors affect the other modules using pitch (filters etc). Instead go for high linearity, and use with well tracking oscillators.

Output the same frequency for 440Hz on all pitch channels. Use FreqMeasure library to provide a frequency input, showing the frequency on the LCD screen. This allows each oscilator to be tuned the same. These pins support measuring frequency:

#define FTM0_CH0_PIN 22
#define FTM0_CH1_PIN 23
#define FTM0_CH2_PIN  9
#define FTM0_CH3_PIN 10
#define FTM0_CH4_PIN  6
#define FTM0_CH5_PIN 20
#define FTM0_CH6_PIN 21
#define FTM0_CH7_PIN  5