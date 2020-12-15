# Eurorack Duophonic MIDI

Re-utilising designs from the Euro-MPE project,
this is a high-precsion, flexible, performance oriented
duophonic MIDI to Eurockack CV module.

There are three MIDI inputs

1. DIN MIDI in
2. USB device in, for DAW
3. USB Host in, for USB keyboards

There are two voice channels. Like Euro-MPE,
each channel has

- Gate
- color LED for gate
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

A difference from the Euro-MPE design is that there is one offset DAC per voice, rather than one for all voices.

@@TODO lay out panel for one voice, to check spacing.

There are eight general CV outputs (14bit), re-using the same board design that produces the two channels of performance outputs. See [Performance DAC](../performance-dac.md)

Inspired by the use of duophony on the ARP 2600 and Odyssey.

## Modes

Any per-channel calibration curves are still applied, so in monophonic mode two different oscillators which have been calibrated will play in unison.

### Monophonic (unison MPE)

Last received note is output on channel one. Any new note overides the previous note.  Channel two duplicates channel one.
Monophonic is MPE-enabled, just outputting to a single voice.

### Monophonic (unison)

Configurable for MIDI channel, or all channels.

### Duophonic (duo MPE)

Lowest received note is output on channel one; highest recieved on channel two. Duo MPE is MPE-enabled,allocating MPE channels to voices based on lowest and highest note priority.

## Duophonic (duo)

Configurable for MIDI channel, or all channels.

## Harmonic

Like unison MPE, but channel two osc CV is offset (for exampe, by a fifth)

Not clear that the two non-MPE modes are needed. Better if the module accepts either MPE or non-MPE.






## Notes

https://support.roli.com/support/solutions/articles/36000025050-slide-absolute-vs-relative
Slide: Absolute vs. Relative