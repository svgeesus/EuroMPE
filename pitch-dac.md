
# Pitch DAC

AD5542CRZ (SOIC-14, $36.39), same as the previous, mono, MIDI2CV project, provides bipolar ±Vref output (so, ±5V).

## Power

Vdd absolute max is -0.3V to +6V. Spec sheet assumes 2.7 ≤ Vdd ≤ 5.5, 2.5 ≤ Vref ≤ Vdd. Check how close can swing to Vdd. When using a 5V ref, may need a 5.5V rail to allow clean swing to 5V output. (Previous project used a 5V rail and AD780N 2.5V ref for that reason, an op-amp then gave 2x gain).

## Digital interface

With Teensy 3.6, 3V3 Vdd unless level shifter used. But 3V3 has reduced dynamic range and needs a (less good) 2.5V or 3V ref, although avoiding level shifter. Using 5V5 for DAC power allows higher dynamic range (lower noise) and allows better, 5V ref. Needs unidirectional level shifter for CS, SCLK, MOSI/Dout but those are inexpensive.

74AHCT125 Quad Level-Shifter (PDIP, SOIC, SSOP)  good for SPI, fast enough. 2 CS, SCLK, MOSI so two pitch DACs.
Vdd abs max -0.5V to +7V so good for 5V5. Base unit needs 2, second supplies 4 more CS; 3 used for global pich and 2 more channels (on first expander, to 4-voice). A third, fitted if needed, supplies 4 more CS for 4 more channels, only needed for 6-voice and 8-voice builds.

Alternatively, use 4-channel digital isolator to prevent coupling of high speed signals, such as TI ISO7240C (SOIC-16, $6.03) which can use 3V3 input and 5V5 output with separate grounds. In that case use one per board on the DAC board, so need 3 for 4-voice and 2 more for 8-voice. Connect with 6-way cable (3V3, DGND, SCLK, MOSI, CS1, CS2).

## Initial accuracy

1LSB is 10V / 2^16 = 152μV. At 1V/Oct, 12 tones per octave, 100 cents per tone, 1 Cent is 833μV so 1 LSB is about 1/5 cent.

INL is ±0.5 (typ) ±1.0 (max) LSB for best (C) grade.

DNL is ±0.5 (typ) ±1.0 (max) LSB

Gain error (away from output voltage extremes) ±0.5 (typ) ±2 (max) LSB
with a gain error TC of ±0.1ppm/C.

Bipolar output depends on matching of internal resistor pair, which is typ 0.0015%. Do the error analysis here. But there is offset and gain trim after this; and matching of internal pair is better than best available LT5400 0.01% pack.

## Line regulation

±1 LSB for  ΔVdd ±10%, nothing specal needed for Vdd regulation here but ensure supply is low noise.

## Load regulation

DAC output impedance is 6.25k which is irrelevant as bipolar mode requires an op-amp on the output anyway.

## Vref connection

Unlike previous project, use a dual op-amp [noninv OP-A, OP-B check common mode range] near the VREF to provide the kelvin connections for each DAC. This avoids variable loading effects from each DAC on the Vref, which become a significant source of error once the Vref offest is accurately nulled. Only the positions used by number of channel expansion cards actually used need to be populated.

"The use of separate force (F) and sense (S) connections (often referred to as a Kelvin connection) at the load removes any errors resulting from voltage drops in the force lead, but, of course, may only be used in systems where there is negative feedback. It is also impossible to use such an arrangement to drive two or more loads with equal accuracy, since feedback may only be taken from one point."

DAC input resistance is highly code-dependent, lowest (around 7.5kΩ) at 0x8555 which is 660 μA at 5V.


## Output conditioning

With a 5V ref and an output buffer [unity OP-C, no external components] this gives ±5V output (10 octaves) which includes Note-ON voltage, global pitchbend, and per-note pitchbend. Note that this does not cover the full MIDI note range of 128 notes = 10.66 octaves. Is that an issue in practice?

Op-amps here need a max Vos of 100μV (1LSB), preferably better. Are chopper amps suitable here? LT1150 operates on 12V bipolar supples, has 10μV max offset. Mouser $9.04 each, $8.28/10, $5.99/25 so go for 25 = 141.25 be damn sure they work first! **OPA4192D** (quad, SOIC-14 which performs better than the TSSOP) 8μV (typ) 50μV (max) $3.67/10 seems good, probably adequate (common-mode input to within 100mV of each rail) and cheaper. Note however Fig. offset voltage vs. common-mode voltage, bad results between 3V and 1.5V from positive rail (i.e. 9 to 10.5V on 12V rails) - not suitable as 10V distribution amp?

In the analog domain this is summed [inv OP-D] with -2V offset (to make range -3 to +8V), and offset trim (on the 2V divider). This op-amp also provides trimmable gain scaling to ensure an accurate 1V/oct over a 9.8 octave range (avoiding calibrating at the ends for offset errors). Thus the DAC should perform the inversion, so re-inverted by the inverting mixer.

2V offset from 20k + 30k resistors [both E24 values] plus trimmer (for precision 2V, and also trimming DAC offset), buffered and inverted with [inv OP-E] 2 × 10k resistors. Watch TC on those 20k, 30k resistive divider.

Summing from 2 × 10k resistors; gain trim from the third 10k resistor  + 10 ohm? 3250 wirewound trimmer (but needs small compensating resistors, half of the trimmer value, on the input) plus current limiting innie 47R resistor and cap. 3/4 of a quad array. In total, 1.5 quad arrays per channel (3 per pair of channels).

3250 has 5% tolerance, 10Ω has effective resolution of 1.3% (130 mΩ), tempco ±50ppm/C
Maybe better to use a non-wirewound on such a small resistance. Or use a high value, in parallel with one resistor of the divider to lower the value slightly. Jumper selects which one to lower.


Needs error analysis to be sure the error budget from resistor matching is reasonable. Breadboard this with OPA2777PA to measure performance, in particular gain error and offsets. Assume  0.01% quad resistor pack, possibly with trimmers too. Needs DAC on a SOIC to DIP breakout board [GOT].

at 10k, 0.01% (100ppm) is 1 ohm; 0.025% (250ppm) is 2.5 ohm
10,001/9999 gain gives a 1mv error on +5V, far too big. Overall gain will need trimming.

Note "This unity-gain difference amplifier (equal resistors) causes the input difference voltage (V2-V1) to be impressed on R5; the resulting current flows to the load. The offset voltage, however, is applied directly to the noninverting input and is amplified by +2 – like a noninverting amplifier (G = 1 + R2/R1). Thus, a 10-mV offset voltage creates 20 mV across R5, producing a 20mA output current offset. A -10-mV offset would create a -20-mA output current (current sinking from the load)."

3 op-amps per channel = 12 for 4 voice, plus 2-op-amps for DAC vref driving per voice = 8 for 4 voice.

# Offset for secondary pitch output

One global offset for all voices.

## Offset DAC

Same DAC, similar circuit in terms of SPI connection, Vref and bipolar output amp. Gain trim (or do digitally?) but no offset trim. Several options:

A) Gain set lower to give ±3V swing (10k and 20k).

B) Unity gain, with ±3V swing? In which case even an AD780N 3V ref and unity-gain op-amp buffer?

Unity gain inverting buffer for external (global) input, normalled to 0V (2 × 10k 0.025% resistors).

Technically a 14bit DAC would be fine here. Or the lower grade of the 16bit. In practice it is easier to use known-working DAC and code for this one too. So:

DAC plus output buffer [unity off-OP-A].

## Offset external Input

Non-inverting buffer for input. Does this need to be as precise? [unity off-OP-B] Check adequate common mode range

## Offset summing

Inverting mixer to sum offset and external signals to send to pitch dac boards (3 × 10k). [inv off-OP-C]

Inverting buffer for offset output jack (2 × 10k) [inv off-OP-D]

Needs 2 quad resistor packs for offset. The less expensive 0.025% ones likely fine here.

One quad low-offset op-amp. OPA4192ID is good here.

## Secondary pitch output (per voice)

Inverting buffer for pitch DAC inversion prior to mixing [inv sec-OP-A]

Inverting mixer for pitch DAC and offset [inv sec-OP-B]

OPA4192ID for two channels of secondary pitch output; two for 4-voice.

