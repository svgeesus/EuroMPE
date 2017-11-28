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

Assume trimmed AD588BQ (CERDIP-16) rather than previous mono MIDI2CV designs with untrimmed AD586M (PDIP-8) or AD780 (PDIP-8).

- [Useful EEBlog article on AD588](https://www.eevblog.com/forum/testgear/ad588-reference-10-volt-0-01-1-5-ppmdeg-simple-rugged-and-affordable/)
- [ad588 ref with 15V derived from 12VAC (Russian)](http://radio-hobby.org/modules/news/article.php?storyid=1226)

### Absolute initial accuracy

Initial accuracy guaranteed (individually tested) is 200ppm: 5V ±1mV. Trimming uses special pins on this Vref, not an op-amp buffer after. Trim range is ±4mV.

Final trim offset depends on uncertainty of meter measurement.

### Temperature

1.5ppm/C (525μV max over 0 to 70C)

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
- [simple resistive heater with transistor for current](https://www.alanzucconi.com/2016/08/02/arduino-heater-2/)

### Current / Load

Load regulation good: ±50μV/mA

As this design drives multiple DACs, the kelvin connections only take us from the oven to the Vref distribution board, which will have a fixed, low load (op-amp input resistances, in parallel, plus a small resistive load for stability).

1mA is 5k load resistor. 10-20k static load could help assure a constant load depending on peak DAC current draws. AD5542CRZ current is code-dependent, varies from 100 to 200μA per DAC (so worst case, 9 DAC all switching together, 900μA to 1800μA).

If reference buffered by diff amp next to each DAC, load is constant. But then need precision matched resistors for the differential amp!

If reference output buffered on a central, Vref distribution board by sets of unity gain differential amp (one per DAC), load is constant. Kelvin connections on each DAC over 4-way jumper wire set to each DAC board. Use the Hinton scheme for balanced output, and DAC provides the kelvin connections.

Assume a second, unipolar buffer drives the less critical DACs. Assume 2V reference derived locally (as part of offset trim) on each Pitch DAC board.


### Long term drift

15ppm/1k hr (75μV), worst on new device and settling over time.

Burn in at ±12V/1week. Check burn-in drift over some weeks, determine optimum balance between waiting and stability.

### Line regulation

Worse than comparable (and cheaper!) references: 200 max μV/V (10.8 to 18V)

Regulated bipolar 11V supply for Vref, with ample PSU buffer caps.

LT1964-BYP LDO negative variable regulator, 340mV dropout, 200mA SOT-23

LT1761 LDO positive reg, 300mV dropout, 100ma SOT-23

These should hold supply voltage to ±11V ±5mV and cope with PSU droop down to ±11.4V, and provide some noise rejection.

## Pitch DAC

AD5542CRZ ($36.39), same as the previous, mono, MIDI2CV project, provides bipolar ±Vref output (so, ±5V).

### Power

Vdd absolute max is -0.3V to +6V. Spec sheet assumes 2.7 ≤ Vdd ≤ 5.5, 2.5 ≤ Vref ≤ Vdd. Check how close can swing to Vdd. When using a 5V ref, may need a 5.5V rail to allow clean swing to 5V output. (Previous project used a 5V rail and 2.5V ref for that reason, an op-amp then gave 2x gain).

### Digital interface

With Teensy 3.6, 3v3 Vdd unless level shifter used.But 3V3 has reduced dynamic range and needs a (less good) 2.5V or 3V ref, although avoiding level shifter. 5V5 allows higher dynamic range (lower noise) but allows better, 5V ref. Needs unidirectional level shifter for CS, SCLK, MOSI/Dout but those are inexpensive.

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

Or, aassuming one of the performance controls is connected to the external input
- rely on digital-domain attenuation on the 14bit per-note outputs (probably sufficient).
- use digital pots? No, lots of pots (4 per note)

## Performance (Control Change) DAC

Needs to be 14-bit capable to fully implement the HR aspect, but precision requirement lower than for pitch; most devices send 7bit data and the ones that are high resolution have ENOB less than 14.

AD5648-2 octal 14-bit DAC ($19.91) NO unsuitable due to zero and gain offsets. Internal VRef of 2.5V gives unipolar 5V output. Better performance from the -2 devices at 5V than the -1 devices at 3V3. Fig. 31 shows 100mV (!!) error when sourcing or sinking 2mA. Internal 2V5 reference with 2x gain, can use external (5V) ref. Most of the graphs in datasheet use an external reference :)
Vref seems to give a couple of mV error in output wrt temperature. Fig.54 shows 4mV error in internal ref wrt temperature.

DAC8168C octal 14-bit DAC ($23.96) much better offsets.

Octal DAC does 2 voices  of attack veocity, lift velocity, pressure, glide. Same board used to provide the global performance controls (mod wheel, sustain, etc). .

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

Simplest to use a second Teensy (LC or possibly 3.2) to drive the display and handle user interaction. That keeps the code and control flow separate, also frees up an SPI bus for controlling the display. Use I2C or Serial to send commands to the main Teensy 3.6, primarily to go into calibration mode.

The 3.2/LC can also handle oven control duties.

## MIDI decoding & DAC driving

Teensy 3.6, because it has two separate SPI busses easily acessible. The speed will also likely help when driving a lot of DACs ( 1 × 16bit on one bus, 5 8 × 14bit on the other). Fast single-precision float allows all frequency calculations to be done in float, then rounded for output to DACs.

Teensy 3.6 uses about 80 mA.

### MIDI connections

Primary interface is likely to be as a USB MIDI device, using the Teensy 3.x USB MIDI implementation. That connection requires a host, such as a computer running a DAW.

Secondary interface could be DIN MIDI input, using the Arduino MIDI library and an optocoupler.

Third one could be USB Host MIDI: new in Teensy 3.6 with a separate USB Host connection port.

All three should register (the same) callbacks to allow input from any of the threee interfaces. All running at once? or switching between then from front panel.

### MPE handling

Teensy MIDI library is low-level. A higher level should be built on top, to provide the following (not specific to MPE):

- handling of 14-bit MIDI CC with paired CCs (HR callbacks)
- handling of RPN (all registered ones)
- handling  NRPN (callback)
- handling of tuning request and tuning dump
- handling of HR velocity prefix
- handling pitchbend range setting, and handling pitchbend taking into account default (MPE) and non-default range settings

and in addition (for MPE):

- zone setting (suggest it allows only a single zone, the latest set)
- handling the extra CCs which become 14-bit in MPE
- capturing allowed info from note channels
- capturing global info from global channel and sending to all notes

These would provide a second level of callbacks; the Euro-MPE implementation then provides funtions for each callback to actually control the DACs. The MIDI wrapper and the MPE wrapper should be re-usable for other Teensy projects; eventually becoming a library.

## Calibration

Several types of calibration:

### Initial setup trimming

Ensure trimmers are on front panel so unit can be trimmed while in the rack, powered and warm. Firstly setting precise 5V output from reference; then setting offset (by adjusting the 2V offset reference) and gain on each pitch DAC. The 14bit outputs are untrimmed, 0-5V within 5-10mV is fine.

Vref needs extensive trimming and measurement (for ageing) outside case anyway. Provide a buffered +5V jack on front panel, offset trimmer near it.

Needs outputs at 0V (before offset, so provide test point? or trim outside case as a first check), -2V (= 0V after offset, for offset trim), -4.8 = -2.8V, +4.8 = +6.8V (to avoid DAC extremes where zero and FS offset complicate gain setting, for gain trim).

Global offset also needs an output, for calibration plus handy to expose for other patching.

### Per-voice oscillator calibration

Avoid non-linear calibration curves. These are fine for pulling poorly-tracking oscillators into tune but then the same errors affect the other modules using pitch (filters etc). Instead go for high linearity, and use with well tracking oscillators.