
# Performance (Control Change) DAC

One octal DAC does 2 voices  of attack veocity, lift velocity, pressure, glide. Same board also used to provide the global performance controls (mod wheel, sustain, etc). .

## Chip selection

Needs to be 14-bit capable to fully implement the HR aspect, but precision requirement lower than for pitch; most devices send 7bit data and the ones that are high resolution have ENOB less than 14.

**AD5648-2** octal 14-bit DAC ($19.91, got one) NO unsuitable due to zero and gain offsets. Internal VRef of 2.5V gives unipolar 5V output. Better performance from the -2 devices at 5V than the -1 devices at 3V3. Fig. 31 shows 100mV (!!) error when sourcing or sinking 2mA. Internal 2V5 reference with 2x gain, can use external (5V) ref. Most of the graphs in datasheet use an external reference :)
Vref seems to give a couple of mV error in output wrt temperature. Fig.54 shows 4mV error in internal ref wrt temperature.

**DAC8168C** octal 14-bit DAC (TSSOP-16, $23.96) much better offsets.

## Power

Same clean 5.5V as the pitch DACs use

## Digital interface

Needs level shifter for SPI. Use second SPI channel on Teensy 3.6. One quad shifter handles 2 CS plus SCLK and MOSI - enough for base board 2 voices and global CC board. A second optional shifter gives 4 more CS of which 3 are used for voices 3-4, 5-6 and 7-8.

Same 74AHCT125 as used for pitch DAC. One does 2 DAC (4 voices), second adds 4 more CS. Base 4-voice needs two because of the global channel, so one CS for that, two for the other four voices, one left spare.

## Vref

DAC8168C Internal 2.5V VRef has good temperature stability (especially C grade) and long-term drift. External Vref probably not needed.
Internal Vref in DAC8168C is adequate for general non-pitch CV duties

## Initial accuracy

**DAC8168C** check the figures below are for that chip!

INL ±1 / ±4 LSB

DNL ±0.1 / ±0.5 LSB

Gain error 500μV / 7.5mV

Zero error 	1mV / 4mV

At 0..5V, 1LSB is 306μV. INL implies 13ENOB (612μV). For standard resolution 7bit data, we only need 2^6 = 64 LSB accuracy which is 19.5mV. Especilly due to the huge max zero-scale offset, we are getting 11-12 ENOB with these mV offsets. TL071 with 3mV offset is now significant wrt typical (but not max) offsets.

DAC is not trimmable without external conditioning circuitry.

Long term drift (Fig.7) inside ±100ppm/2k hours

## Line regulation

10μV/V is fine here

## Load

10μV/mA

Internally buffered, but poor load regulation when sinking current; will need external buffer to protect from modular environment (like getting 12V plugged into an output by mistake) and to give current drive.


## Output conditioning

3mV offset on TL074 is not significant, though if an inexpensive alternative op-amp is better (1mV or less, does not need to be high precision) use that.

Consider LT1214 (100μV, $5.85 quad, PDIP-14 or SOIC-14) or OPA4172 (200μV, $3.82 quad, SOIC-14) as non-inverting output buffers. Use innie current limiting resistor. Maybe time to try a SMD board design?


## Fading (all voice 'attenuators')

Four small pots or small sliders, read by four ADC inputs, used to reduce the code value sent to DACs to implement per-performance-output attenuator function across all channels.

[Example responsive analog read](https://forum.pjrc.com/threads/45376-Example-code-for-MIDI-controllers-with-Pots-and-Buttons)