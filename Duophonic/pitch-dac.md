
# Pitch DAC

Use a very linear, well behaved 16-bit DAC per voice (so, two needed).

Over 10 octaves, at the desired precision, 16 bits is barely sufficient (0.2 cents). More bits would be desirable, to give finer precision and to allow for the loss of precision from using a calibration curve. However, 20bit or 18bit DACs can be expensive, and some are no better than 16bit INL; 24bit DACs are widely available and cheap, but tuned for audio AC performance not DC accuracy and linearity.

_AD5542CRZ_ (SOIC-14, $52.61/1 in stock), same as I used in my previous, mono, MIDI2CV project, provides bipolar ±Vref output (so, ±5V) with one bipolar-powered op-amp in the output loop. Power up to +5V5. INL ±0.5LSB (typ) ±1.0LSB (max, = 15ppm) for best (C) grade. DNL is ±1 LSB max = 15ppm. SPI needs 16bit transfer.

**AD5781ARUZ** (TSSOP-20, $40.96/1 no stock til April 2023) 18bit. INL ±1 LSB (max, 5V vref, = 3.6ppm) for B grade, ±4 LSB (max, = 15ppm) for A grade. _Mouser does not stock the best (B) grade. A grade has more resolution, but same INL, as the 16bit AD5542CRZ. Digikey has A grade at €30.15, B grade at €42.50_    DNL however is ±1 LSB max = 3.6ppm for _both_ grades. Bipolar ±Vref output, power up to ±16.5V. SPI needs 24bit transfer. Needs Schottky diode for power rail syncronization, see datasheet fig. 50. A grade is actually less expensive than AD5542CRZ, with better DNL. Check that the layout is going to be reasonable on a 2-layer board.

_AD5791BRUZ_ (TSSOP-20, $133.24/1 no stock) or _AD5791ARUZ_ (TSSOP-20, $84.45/1 no stock) 20bit, 1ppm linearity, bipolar ±Vref output, power up to ±16.5V. Very expensive.

Conclusion: AD5781ARUZ gives 16bit INL with 18bit DNL at slightly lower cost than AD5542CRZ.

![DAC pinout](./img/AD5781-pinout.png)

## Power

AD5781ARUZ Vdd and Vss abs max of 16.5V. Spec sheet assumes VDD = +12.5 V to +16.5 V, VSS = −16.5 V to −12.5 V. Optimal supply for lowest zero-scale and gain errors is ±9.5V (datasheet, figs 20 & 22, _noting results are given for either 5V and for ±10V spans while I need a ±5V span_).

Advantage of ±12V-derived ±9V5 rails, compared to using Eurorack 5V power, is that filtering and smoothing can be applied, reducing switching PSU noise and increasing decoupling from the digital circuitry running on Eurorack 5V.

## Digital interface

With Teensy 3.6, for AD5542CRZ 3V3 Vdd unless level shifter used. But 3V3 has reduced dynamic range and needs a (less good) 2.5V or 3V ref, although avoiding level shifter. Using 5V5 for DAC power allows higher dynamic range (lower noise) and allows better, 5V ref. Needs unidirectional level shifter for CS, SCLK, MOSI/Dout but those are inexpensive. For AD5781ARUZ 2V7 to 5V5 can be used.

**74AHCT125** Quad Level-Shifter (PDIP, SOIC, SSOP)  good for SPI, fast enough. 2 CS, SCLK, MOSI so two pitch DACs.
Vdd abs max -0.5V to +7V so good for 5V5. Base unit needs 2, second supplies 4 more CS; 3 used for global pich and 2 more channels (on first expander, to 4-voice). A third, fitted if needed, supplies 4 more CS for 4 more channels, only needed for 6-voice and 8-voice builds.

Alternatively, use 4-channel digital isolator to prevent coupling of high speed signals, such as TI **ISO7240C** (SOIC-16, $6.03) which can use 3V3 input and 5V5 output with separate grounds. In that case use one per board on the DAC board, so need 3 for 4-voice and 2 more for 8-voice. Connect with 6-way cable (3V3, DGND, SCLK, MOSI, CS1, CS2). Does this really help with digital feedthrough? Not needed for AD5781ARUZ, simplifying design.

## Initial accuracy

### AD5781ARUZ

1LSB is 10V / 2^18 = 38μV. At 1V/Oct, 12 tones per octave, 100 cents per tone, 1 Cent is 833μV so 1 LSB is about 1/20 cent.

INL is ±2 (typ) ±4 (max) LSB for worst (A) grade = 15ppm.

DNL at 5V vref is ±0.5 (typ) ±1.0 (max) LSB = 3.6ppm.

Linearity Error Long-Term Stability is 0.03 LSB after 1k hours at 100C.

Gain error (away from output voltage extremes) is 0.4ppm (typ) 20ppm FSR (max) = 2μV (typ) 100μV (max) with a gain error TC of ±0.04ppm FSR/C.

## Line regulation

±1 LSB for  ΔVdd ±10%, nothing specal needed for Vdd regulation here but ensure supply is low noise.

## Load regulation

DAC output impedance is 3.4k for AD5781ARUZ which is irrelevant as bipolar mode requires an op-amp on the output anyway.

## Vref connection

A pair of op-amps (one non inverting for VrefP, one inverting for VrefN) per pitch DAC for (required) kelvin connections. AD5781 datasheet uses AD8676 ($6.89/1) dual op-amp for Vref buffers, which is 12 uV Vos low noise rail-to-rail. R/R does not seem to be needed for a 5V output on ±12 or ±9.5 rails.

DAC input resistance is highly code-dependent. For AD5781ARUZ, lowest (around 5kΩ) which is 1mA at 5V.


## Output conditioning

With a 5V ref and an output buffer [unity OP-C, no external components] this gives ±5V output (10 octaves) which includes Note-ON voltage, global pitchbend, and per-note pitchbend. Note that this does not cover the full MIDI note range of 128 notes = 10.66 octaves. Is that an issue in practice?

Op-amps here need a max Vos of 100μV (1LSB), preferably better. Are chopper amps suitable here? LT1150 operates on 12V bipolar supples, has 10μV max offset. Mouser $9.04 each, $8.28/10, $5.99/25 so go for 25 = 141.25 be damn sure they work first! **OPA4192D** (quad, SOIC-14 which performs better than the TSSOP) 8μV (typ) 50μV (max) $3.67/10 seems good, probably adequate (common-mode input to within 100mV of each rail) and cheaper. Note however Fig. offset voltage vs. common-mode voltage, bad results between 3V and 1.5V from positive rail (i.e. 9 to 10.5V on 12V rails) - not suitable as 10V distribution amp?

**No** In the analog domain this is summed [inv OP-D] with -2V offset (to make range -3 to +8V), and offset trim (on the 2V divider). This op-amp also provides trimmable gain scaling to ensure an accurate 1V/oct over a 9.8 octave range (avoiding calibrating at the ends for offset errors). Thus the DAC should perform the inversion, so re-inverted by the inverting mixer.

2V offset from 20k + 30k resistors [both E24 values] plus trimmer (for precision 2V, and also trimming DAC offset), buffered and inverted with [inv OP-E] 2 × 10k resistors. Watch TC on those 20k, 30k resistive divider.

**Yes** just do the offset trim in the digital domain, avoiding errors from a 2V offset and the more complex Vref connection. Output remains simply -5 to +5V. Gain trimming still needed, so OP-D still required sadly, unless it can be combined with the output op-amp somehow.

Summing from 2 × 10k resistors; gain trim from the third 10k resistor  + 10 ohm? 3250 wirewound trimmer (but needs small compensating resistors, half of the trimmer value, on the input) plus current limiting innie 47R resistor and cap. 3/4 of a quad array. In total, 1.5 quad arrays per channel (3 per pair of channels). **No, need to use a complete array to get benefit of within-package matching:**

LT5400 A-grade resistor pack. 0.01% _within-package_ matching, 0.2ppm/°C within-package drift and 8ppm/°C absolute drift.

"Note 6: (∆R/R)CMRR (Matching for CMRR) is a metric for the contribution of error from the LT5400 when used in a difference configuration using the specific resistor pairs of R1/R2 and R4/R3"

"The LT5400 specifies matching in the most conservative possible way. In each device, the ratio error of the largest of the four resistors to the smallest of the four resistors meets the specified matching level."

3250 has 5% tolerance, 10Ω has effective resolution of 1.3% (130 mΩ), tempco ±50ppm/C
Maybe better to use a non-wirewound on such a small resistance. Or use a high value, in parallel with one resistor of the divider to lower the value slightly. Jumper selects which one to lower.


Needs error analysis to be sure the error budget from resistor matching is reasonable. Breadboard this with OPA2777PA to measure performance, in particular gain error and offsets. Assume  0.01% quad resistor pack, possibly with trimmers too. Needs DAC on a SOIC to DIP breakout board [GOT].

at 10k, 0.01% (100ppm) is 1 ohm; 0.025% (250ppm) is 2.5 ohm
10,001/9999 gain gives a 1mv error on +5V, far too big. Overall gain will need trimming.

Note "This unity-gain difference amplifier (equal resistors) causes the input difference voltage (V2-V1) to be impressed on R5; the resulting current flows to the load. The offset voltage, however, is applied directly to the noninverting input and is amplified by +2 – like a noninverting amplifier (G = 1 + R2/R1). Thus, a 10-mV offset voltage creates 20 mV across R5, producing a 20mA output current offset. A -10-mV offset would create a -20-mA output current (current sinking from the load)."

3 op-amps per channel = 12 for 4 voice, plus 2-op-amps for DAC vref driving per voice = 8 for 4 voice.
