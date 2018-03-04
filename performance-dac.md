
# Performance (Control Change) DAC

One octal DAC does 2 voices  of attack veocity, lift velocity, pressure, glide. Same board also used to provide the global performance controls (mod wheel, sustain, etc). .

## Chip selection

Needs to be 14-bit capable to fully implement the HR aspect, but precision requirement lower than for pitch; most devices send 7bit data and the ones that are high resolution have ENOB less than 14. 1 LSB at 14bit is 300μV but 1 LSB at 7bit is a huge 39mV. Aim for around 10ENOB.

**AD5648-2** octal 14-bit DAC ($19.91, got one) NO unsuitable due to zero and gain offsets. Internal VRef of 2.5V gives unipolar 5V output. Better performance from the -2 devices at 5V than the -1 devices at 3V3. Fig. 31 shows **100mV** (!!) error when sourcing or sinking 2mA. Internal 2V5 reference with 2x gain, can use external (5V) ref. Notice most of the graphs in datasheet use an external reference :)
Vref seems to give a couple of mV error in output wrt temperature. Fig.54 shows 4mV error in internal ref wrt temperature.

**DAC8168C** octal 14-bit DAC (TSSOP-16, $23.96) much better offsets.

## Power

Same clean 5.5V as the pitch DACs use

## Digital interface

Needs level shifter for SPI. Use second SPI channel on Teensy 3.6. One quad shifter handles 2 CS plus SCLK and MOSI - enough for base board 2 voices and global CC board. A second shifter gives 4 more CS of which 3 are used for voices 3-4, 5-6 and 7-8.

74AHCT125. One does 2 DAC (4 voices), second adds 4 more CS. Base 4-voice needs two because of the global channel, so one CS for that, two for the other four voices, one left spare. Or would each DAC need its own SCLK and MOSI? In that case one per 2 voices, plus one for global (3 for 4 voice, 5 for 8 voice).


## Vref

DAC8168C Internal 2.5V VRef has initial accuracy of 100μV (0.004%), good temperature stability (especially 2x gain C grade) and long-term drift. External Vref probably not needed.
Internal Vref in DAC8168C is adequate for general non-pitch CV duties. Output is 0 to 5V.

## Initial accuracy

From **DAC8168C** datasheet, assuming internal Vref:

INL 300μV / 1.2mV (±1 / ±4 LSB)

DNL 30μV / 150μV (±0.1 / ±0.5 LSB)

Gain error 1mV / 4mV (±0.01% / ±0.15% of FSR) with  5μV (±1 ppm of FSR)/°C drift

Offset error  ±1 / ±4 mV with ±0.5 μV/°C drift

Zero error 	1mV / 4mV with  ±2 μV/°C drift

At 0..5V, 1LSB is 306μV. TL071 with 3mV offset is now significant wrt typical (but not max) offsets.

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

Use 4 pots with rail-to-rail input and output buffer amps connected to 4 adc inputs. Then use these values to digitally scale the 14bit performance values (on all voices), providing per-performance-output attenuator function across all channels.

- [Alpha 9mm T18 shaft pot, 10k](https://www.thonk.co.uk/shop/alpha-9mm-pots-vertical-t18/)
- [T18 micro knobs](https://www.thonk.co.uk/shop/micro-knobs/)

or (takes more space, but easier to see the value)

- [Alpha B50K Slider](https://www.thonk.co.uk/shop/music-thing-modular-sliders-for-voltages/)

 Use Responsive Analog Read:

- [Writing a better noise-reduction algorithm](http://damienclarke.me/code/posts/writing-a-better-noise-reducing-analogread)
- [RAR on GitHub](https://github.com/dxinteractive/ResponsiveAnalogRead)
- [Example responsive analog read](https://forum.pjrc.com/threads/45376-Example-code-for-MIDI-controllers-with-Pots-and-Buttons)

