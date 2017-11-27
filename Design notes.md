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

[Useful EEBlog article on AD588](https://www.eevblog.com/forum/testgear/ad588-reference-10-volt-0-01-1-5-ppmdeg-simple-rugged-and-affordable/)
[ad588 ref with 15V derived from 12VAC (Russian)](http://radio-hobby.org/modules/news/article.php?storyid=1226)

### Absolute initial accuracy

Assume trimmed AD588BQ (CERDIP-16) rather than previous mono MIDI2CV design with untrimmed AD586M (PDIP-8). Vref is 5V ±1mV so allow for ±2mV adjustment to completely cover error sources.

if 5V is unity gain, 5.002 is 1.0004 gain and 4.998 is 0.9996 gain. However, trimming seems to use special pins on this Vref, not an op-amp buffer after. Trim range is ±4mV.

Final trim offset depends on uncertainty of meter measurement.

### Temperature

Once trimmed, temperature term dominates. Need an oven, 50 or 60CC ±0.1C instead of 20 to 50C range. If running at 60 or over, maybe better going for the Industrial range (-25 to 85C) rather than Commercial (0 to 70C) for longer reference life. Oven control circuitry drives panel LED with red - orange - green for temp stability.

Temp measurement by DS18B20 thermally epoxied to the Vref chip. Use direct power, not parasitic. Accuracy is 0.5C.

- [Maxim DS18B20 product page](https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18B20.html)
- [DS18B20 (9 bit) vs. DS18S20 (12 bit)](https://www.maximintegrated.com/en/app-notes/index.mvp/id/4377)
- [Maxim DS18B20](http://vwlowen.co.uk/arduino/ds18b20/ds18b20.htm)
- [PJRC OneWire library docs](https://www.pjrc.com/teensy/td_libs_OneWire.html)
- [Onewire tutorial](https://playground.arduino.cc/Learning/OneWire)
- [original Dallas Library](https://www.milesburton.com/Dallas_Temperature_Control_Library)
- [Arduino Library for Maxim Temperature ICs](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- [Example, Arduino DS18B20](https://create.arduino.cc/projecthub/everth-villamil-ruiz/temperature-sensor-ds18b20-3decfc)
- [Example2, same](https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806)

Heater by variable current (or PWM??) through a resistor?

- [Arduino PID playground](http://playground.arduino.cc/Code/PIDLibrary)
- [Arduino PID library on GitHub](https://github.com/br3ttb/Arduino-PID-Library/)
- [PID Tutorial](http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/)
- [Google group](https://groups.google.com/forum/#!forum/diy-pid-control)
- [Autotune](https://playground.arduino.cc/Code/PIDAutotuneLibrary)
- [Example: PID control for instrument "oven"](https://groups.google.com/forum/#!topic/diy-pid-control/4EY679OWbQw)

### Current / Load

Consider total load of 8+1 = 9 16bit DACs with kelvin connections (NO, can only drive one load). Assume a second, unipolar buffer drives the less critical DACs and another, the 2V reference.

1mA is 5k load resistor. 10k static load could help assure a constant load depending on peak DAC current draws. AD5542CRZ current is code-dependent varies from 100 to 200μA per DAC (so worst case, 9 DAC all switching together, 900μA to 1800μA).

If reference buffered by diff amp next to each DAC, load is constant. But then need precision matched resistors for the differential amp!

Reference output buffered by sets of unity gain differential amp (one per DAC) driving the kelvin connections to each DAC over 4-way jumper wire set to each DAC board. Use the Hinton scheme for balanced output, and DAC provides the kelvin connections.

### Long term

Burn in at ±12V/1week. Check burn-in drift over some weeks, determine optimum balance between waiting and stability.

### Line regulation

Regulated bipolar 11V supply for Vref. Otherwise, just ample PSU buffer caps.
LT1964-BYP LDO negative variable regulator, 340mV dropout, 200mA SOT-23
LT1761 LDO positive reg, 300mV dropout, 100ma SOT-23
These should hold supply voltage to ±11V ±5mV and cope with PSU droop down to ±11.4V, and provide some noise rejection.

## Pitch DAC

AD5542CRZ, same as the previous, mono, MIDI2CV project.

### Power

Vdd absolute max is -0.3V to +6V. Spec sheet assumes 2.7 ≤ Vdd ≤ 5.5, 2.5 ≤ Vref ≤ Vdd. Check how close can swing to Vdd. When using a 5V ref, may need a 5.5V rail to allow clean swing to 5V output. (Previous project used a 5V rail and 2.5V ref for that reason, op-amp then gave 2x gain).

### Digital interface

Uh, driving with Teensy 3.6, 3v3 Vdd unless level shifter used. Whoops! 3V3 has reduced dynamic range and needs a (less good) 2.5V or 3V ref, but needs no level shifter. 5V5 allows higher dynamic range (lower noise) but allows better, 5V ref. Needs unidirectional level shifter for CS, SCLK, MOSI/Dout.

74AHCT125 Quad Level-Shifter (PDIP, SOIC, SSOP)  good for SPI, fast enough. 2 CS, SCLK, MOSI so two pitch DACs.
Vdd abs max -0.5V to +7V so good for 5V5. Base unit needs 2, second supplies 4 more CS; 3 used for global pich and 2 more channels (on first expander, to 4-voice). A third, fitted if needed, supplies 4 more CS for 4 more channels, only needed for 6-voice and 8-voice builds.

### Initial accuracy

INL is ±0.5 (typ) ±1.0 (max) LSB for best (C) grade.
DNL is ±0.5 (typ) ±1.0 (max) LSB
Gain error (away from output voltage extremes) ±0.5 (typ) ±2 max) LSB
with a gain error TC of ±0.1ppm/C.

Bipolar output depends on matching of internal resistor pair, which is typ 0.0015%

### Line regulation

±1 LSB for  ΔVdd ±10%, nothing specal needed for Vdd regulation here.

### Load regulation

DAC output impedance is 6.25 which is irrelevant as bipolar mode requires an op-amp on the output anyway.

### Vref connection

Unlike previous project, use a [OP-A] differential op-amp near the VREF to provide the kelvin connections for each DAC. This avoids variable loading effects from each DAC on the Vref, which become a significant source of error once the Vref offest is accurately nulled. Only the sockets used by number of channel expansion cards actually used need to be populated.

"The use of separate force (F) and sense (S) connections (often referred to as a Kelvin connection) at the load removes any errors resulting from voltage drops in the force lead, but, of course, may only be used in systems where there is negative feedback. It is also impossible to use such an arrangement to drive two or more loads with equal accuracy, since feedback may only be taken from one point."

Vref input resistance code-dependent, lowest (around 7.5kΩ) at 0x8555

### Output conditioning

With a 5V ref and an output buffer [OP-B] this gives ±5V output (10 octaves) which includes Note-ON voltage, global pitchbend, and per-note pitchbend. In the analog domain this is summed [OP-C] with 2V offset (to make range -3 to +8V), and offset trim (on the 2V divider). A second? op-amp provides trimmable gain scaling to ensure an accurate 1V/oct over a 9 octave range (avoiding the ends for offset errors). Or combine into one opamp.

Needs error analysis to be sure the error budget from resistor matching is reasonable.

Note "This unity-gain difference amplifier (equal resistors) causes the input difference voltage (V2-V1) to be impressed on R5; the resulting current flows to the load. The offset voltage, however, is applied directly to the noninverting input and is amplified by +2 – like a noninverting amplifier (G = 1 + R2/R1). Thus, a 10-mV offset voltage creates 20 mV across R5, producing a 20mA output current offset. A -10-mV offset would create a -20-mA output current (current sinking from the load)."

OP-A could be a precision differential amp with on-chip input resistors.

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

(assuming one of the performance controls is connected to the external input)
- rely on digital-domain attenuation on the 14bit per-note outputs (probably sufficient).
- use digital pots? No, lots of pots (4 per note)


## Display, UI

Simplest to use a second Teensy (LC or possibly 3.2) to drive the display and handle user interaction. That keeps the code and control flow separate, also frees up an SPI bus for controlling the display. Use I2C or Serial to send commands to the main Teensy 3.6, primarily to go into calibration mode.

The 3.2/LC can also handle oven control duties.

## Calibration

Several types of calibration:

### Initial setup trimming

Ensure trimmers are on front panel so unit can be trimmed while in the rack, powered and warm. Firstly setting precise 5V output from reference; then setting offset (by adjusting the 2V offset reference) and gain on each pitch DAC. The 14bit outputs are untrimmed, 0-5V within 5-10mV is fine.

Vref needs extensive trimming and measurement (for ageing) outside case anyway. Provide a buffered +5V jack on front panel, offset trimmer near it.

Needs outputs at 0V (before offset, so provide test point? or trim outside case as a first check), -2V (= 0V after offset, for offset trim), -4.8 = -2.8V, +4.8 = +6.8V (to avoid DAC extremes where zero and FS offset complicate gain setting, for gain trim).

Global offset also needs an output, for calibration plus handy to expose for other patching.

### Per-voice oscillator calibration

Avoid non-linear calibration curves. These are fine for pulling poorly-tracking oscillators into tune but then the same errors affect the other modules using pitch (filters etc). Instead go for high linearity, and use with well tracking oscillators.