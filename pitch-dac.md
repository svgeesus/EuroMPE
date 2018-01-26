
# Pitch DAC

AD5542CRZ (SOIC-14, $36.39), same as the previous, mono, MIDI2CV project, provides bipolar ±Vref output (so, ±5V).

## Power

Vdd absolute max is -0.3V to +6V. Spec sheet assumes 2.7 ≤ Vdd ≤ 5.5, 2.5 ≤ Vref ≤ Vdd. Check how close can swing to Vdd. When using a 5V ref, may need a 5.5V rail to allow clean swing to 5V output. (Previous project used a 5V rail and 2.5V ref for that reason, an op-amp then gave 2x gain).

## Digital interface

With Teensy 3.6, 3V3 Vdd unless level shifter used. But 3V3 has reduced dynamic range and needs a (less good) 2.5V or 3V ref, although avoiding level shifter. Using 5V5 for DAC power allows higher dynamic range (lower noise) and allows better, 5V ref. Needs unidirectional level shifter for CS, SCLK, MOSI/Dout but those are inexpensive.

74AHCT125 Quad Level-Shifter (PDIP, SOIC, SSOP)  good for SPI, fast enough. 2 CS, SCLK, MOSI so two pitch DACs.
Vdd abs max -0.5V to +7V so good for 5V5. Base unit needs 2, second supplies 4 more CS; 3 used for global pich and 2 more channels (on first expander, to 4-voice). A third, fitted if needed, supplies 4 more CS for 4 more channels, only needed for 6-voice and 8-voice builds.

Alternatively, use 4-channel digital isolator to prevent coupling of high speed signals, such as TI ISO7240C (SOIC-16, $6.03) which can use 3V3 input and 5V5 output with separate grounds. In that case use one per board on the DAC board, so need 3 for 4-voice and 2 more for 8-voice. Connect with 6-way cable (3V3, DGND, SCLK, MOSI, CS1, CS2).

## Initial accuracy

1LSB is 5V / 2^16 = 76μV. At 1V/Oct, 12 tones per octave, 100 cents per tone, 1 Cent is 833μV so 1 LSB is about 1/10 cent.

INL is ±0.5 (typ) ±1.0 (max) LSB for best (C) grade.

DNL is ±0.5 (typ) ±1.0 (max) LSB

Gain error (away from output voltage extremes) ±0.5 (typ) ±2 max) LSB
with a gain error TC of ±0.1ppm/C.

Bipolar output depends on matching of internal resistor pair, which is typ 0.0015%. Do the error analysis here. But there is offset and gain trim after this.

## Line regulation

±1 LSB for  ΔVdd ±10%, nothing specal needed for Vdd regulation here but ensure supply is low noise.

## Load regulation

DAC output impedance is 6.25k which is irrelevant as bipolar mode requires an op-amp on the output anyway.

## Vref connection

Unlike previous project, use a dual op-amp [OP-A, OP-B] near the VREF to provide the kelvin connections for each DAC. This avoids variable loading effects from each DAC on the Vref, which become a significant source of error once the Vref offest is accurately nulled. Only the sockets used by number of channel expansion cards actually used need to be populated.

"The use of separate force (F) and sense (S) connections (often referred to as a Kelvin connection) at the load removes any errors resulting from voltage drops in the force lead, but, of course, may only be used in systems where there is negative feedback. It is also impossible to use such an arrangement to drive two or more loads with equal accuracy, since feedback may only be taken from one point."

Vref input resistance code-dependent, lowest (around 7.5kΩ) at 0x8555 which is 660 μA at 5V.

Or, use a differental op-amp near the DAC to isolate from loading effects; then a 2V ref can be derived from that too.


## Output conditioning

With a 5V ref and an output buffer [OP-C, no external components] this gives ±5V output (10 octaves) which includes Note-ON voltage, global pitchbend, and per-note pitchbend. Note that this does not cover the full MIDI note range of 128 notes = 10.66 octaves. is that an issue in practice?

In the analog domain this is summed [OP-D] with 2V offset (to make range -3 to +8V), and offset trim (on the 2V divider). This op-amp also provides trimmable gain scaling to ensure an accurate 1V/oct over a 9.8 octave range (avoiding calibrating at the ends for offset errors). Or combine into one opamp, both gain and offset. In that case the DAC should perform the inversion, so re-inverted by the inverting mixer.

2V offset from 20k + 30k resistors [both E24 values] plus trimmer

Summing from 2 * 10k resistors; trim from the third 10k resistor  + 10 ohm? 3250 wirewound trimmer (but needs small compensating resistors, half of the trimmer value, on the input) plus current limiting innie 47R resistor and cap. 3/4 of a quad array.

3250 has 5% tolerance, 10Ω has effective resolution of 1.3% (130 mΩ), tempco ±50ppm/C


Needs error analysis to be sure the error budget from resistor matching is reasonable. Breadboard this with OPA2777PA to measure performance, in particular gain error and offsets. Assume 0.1% resistors plus trimmers, or 0.01% resistor pack. Needs DAC on a SOIC to DIP breakout board.

at 10k 0.01% (100ppm) is 1 ohm; 0.025% (250ppm) is 2.5 ohm
10,001/9999 gain gives a 1mv error on 5V.

Note "This unity-gain difference amplifier (equal resistors) causes the input difference voltage (V2-V1) to be impressed on R5; the resulting current flows to the load. The offset voltage, however, is applied directly to the noninverting input and is amplified by +2 – like a noninverting amplifier (G = 1 + R2/R1). Thus, a 10-mV offset voltage creates 20 mV across R5, producing a 20mA output current offset. A -10-mV offset would create a -20-mA output current (current sinking from the load)."

OP-A could be a precision differential amp with on-chip input resistors.

# Offset DAC

Same DAC, similar circuit in terms of SPI connection, Vref and bipolar output amp. No offset trim. Gain set lower to give ±2V swing (10k and 25k). Unity gain inverting buffer for external (global) input, normalled to 0V (2 10k resistors). Buffer for offset output (2 10k). Mixer to sum offset and external signals to send to pitch dac boards (3 10k).