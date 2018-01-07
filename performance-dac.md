
# Performance (Control Change) DAC

One octal DAC does 2 voices  of attack veocity, lift velocity, pressure, glide. Same board also used to provide the global performance controls (mod wheel, sustain, etc). .

## Chip selection

Needs to be 14-bit capable to fully implement the HR aspect, but precision requirement lower than for pitch; most devices send 7bit data and the ones that are high resolution have ENOB less than 14.

AD5648-2 octal 14-bit DAC ($19.91, got one) NO unsuitable due to zero and gain offsets. Internal VRef of 2.5V gives unipolar 5V output. Better performance from the -2 devices at 5V than the -1 devices at 3V3. Fig. 31 shows 100mV (!!) error when sourcing or sinking 2mA. Internal 2V5 reference with 2x gain, can use external (5V) ref. Most of the graphs in datasheet use an external reference :)
Vref seems to give a couple of mV error in output wrt temperature. Fig.54 shows 4mV error in internal ref wrt temperature.

DAC8168C octal 14-bit DAC (TSSOP-16, $23.96) much better offsets.

## Power

Same clean 5.5V as the pitch DACs use

## Digital interface

Same 74AHCT125 as used for pitch DAC. One does 2 DAC (4 voices), second adds 4 more CS. Base 4-voice needs two because of the global channel, so one CS for that, two for the other four voices, one left spare.

## Fading (all voice 'attenuators')

Four small pots or small sliders, read by four ADC inputs, used to reduce the code value sent to DACs to implement per-performance-output attenuator function across all channels.

## Vref

Internal Vref in DAC8168C is adequate for general non-pitch CV duties

## Output conditioning

3mV offset on TL074 is not significant, though if an inexpensive alternative op-amp is better (1mV or less, does not need to be high precision) use that.

