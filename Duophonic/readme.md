# Eurorack Duophonic MIDI

Inspired by the use of duophony on the ARP 2600 and Odyssey.

Re-utilising designs from the Euro-MPE project,
this is a high-precsion, flexible, performance oriented
duophonic MIDI to Eurockack CV module.
It's MPE compatible so there is expression on each of the two notes.

## MIDI connection

There are three MIDI inputs

1. DIN MIDI in
2. USB device in, for DAW
3. USB Host in, for USB keyboards

## Analog voice CV

There are two voice channels. Like Euro-MPE,
each channel has

- color LED for gate
- Gate
- Oscillator pitch CV, highly linear 18bit, low tempco DAC
    - includes pitchbend (Glide) and any microtuning
    - See [Pitch DAC](../pitch-dac.md)
- Filter offset input
- Filter output, mixed from
    - Osc pitch CV
    - Offset DAC
    - offset input
    Related to offsets circuitry, but somewhat lower precision requirement
    Button to indicate adjusting offset (in cents, from encoder)
- Four MPE-compatible expression outputs (14bit)
    1. Stike (attack velocity)
    2. Lift (release velocity)
    3. Slide (forward-back, CC 74)
    4. Press (aftertouch)

A difference from the Euro-MPE design is that, if osssets are used, there is one offset DAC per voice, rather than one for all voices. But that doubes the number of expensive DACs (from 2 to 4).

This also results in 8 jacks plus an led and an encoder, which is cramped and can't be done in a single vertical line. Making the encoder for offset be shared between channels, doesn't really free up enough space.

Dropping the offset and the input, but adding a second buffered pitch CV output to go to a separate offsets module, gives 7 jacks plus an led which is fine in one vertical row. Also reduces number of DACs. That would be

- color LED for gate
- Gate
- Oscillator pitch CV, highly linear 18bit, low tempco DAC
    - includes pitchbend (Glide) and any microtuning
    - See [Pitch DAC](../pitch-dac.md)
- Second pitch CV output
- Four MPE-compatible expression outputs (14bit)
    1. Stike (attack velocity)
    2. Lift (release velocity)
    3. Slide (forward-back, CC 74)
    4. Press (aftertouch)

@@TODO lay out panel for one voice, to check spacing.

## Analog general CC CV

There are also eight general CV outputs (14bit), re-using the same board design that produces the two channels of performance outputs. See [Performance DAC](../performance-dac.md). Perhaps (considering mainly the Roli Seaboard):

Top row
- Slider1 (CC107)
- Slider2 (CC109)
- Slider3 (CC111)
- XY Pad X (CC114)

Bottom row
- Modulation Wheel (CC01+CC33)
- Breath (CC02+CC34)
- Expression Controller (CC11+CC43)
- XY Pad Y (CC113)

Any per-channel calibration curves are still applied, so in monophonic mode two different oscillators which have been calibrated will play in unison.

## Modes

### Monophonic (unison MPE)

Last received note is output on channel one. Any new note overides the previous note.  Channel two duplicates channel one.
Monophonic is MPE-enabled, just outputting to a single voice.

### Monophonic (unison)

Configurable for MIDI channel, or all channels.

### Duophonic (duo MPE)

Lowest received note is output on channel one; highest received on channel two. Duo MPE is MPE-enabled,allocating MPE channels to voices based on lowest and highest note priority.

### Duophonic (duo)

Configurable for MIDI channel, or all channels.

### Harmonic

Like unison MPE, but channel two osc CV is offset (for exampe, by a fifth)

Not clear that the two non-MPE modes are needed. Better if the module accepts either MPE or non-MPE.

## Tuning

Like the original EuroMPE, there should be a [tuning input](../calibration.md) which displays the current note; an auto-tuning mode can use this to calculate a linearising calibration curve. Because this is setup not playing, it's okay to go to a menu for this.

?? Because the tuner is in the same module as the MIDI interface, it knows when the gates are low and can do auto-tuning for drift control after x minutes of inactivity ??

## Voltage reference

The [LM399AH plus differential distribution](../voltage-ref-LM399.md)
solution used by EuroMPE was very expensive, needed a lot of bur in and support circuitry, and was only worthwhile with 4 or more voices.

For duophonic, perhaps one LTC6655CHMS8-5#PBF ($8.95/1) per pitch DAC, which has 1.25mV initial accuracy and kelvin connection, would be good? Mouser does not stock the better LCC version. No trim though.

Or LT1236ACS8-5#PBF ($9.32/1) has trim (worsens tempco) no kelvin.

Or MAX6126AASA50+ ($7.81/1) no trim, kelvin connections for Vref and GND, 1mV initial accuracy, 0.5ppm/C (typ) 3ppm/C (max). As used in transpositeurs.

## Main MPU board

See [MPU board](MPU-board.md).

## Notes

https://support.roli.com/support/solutions/articles/36000025050-slide-absolute-vs-relative
Slide: Absolute vs. Relative