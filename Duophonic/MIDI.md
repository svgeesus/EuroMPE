# MIDI implementation

## Libraries

Low level, collect data and dispatch to handlers

- Francois Best [MIDI Library](https://www.pjrc.com/teensy/td_libs_MIDI.html) (version 5 on [GitHub](https://github.com/FortySevenEffects/arduino_midi_library) but 4 may be simpler to use here)
- Paul Stoffregen USB MIDI Host using [USBHost_t36](https://github.com/PaulStoffregen/USBHost_t36)
- Teensyduino [USB MIDI](https://www.pjrc.com/teensy/td_midi.html)

Note that [Arduino USBMIDI](https://github.com/lathoub/Arduino-USBMIDI) claims to support Teensy 4.1 but does not support USB Host.

Note [bug in MIDI.hpp re system common messages](https://forum.pjrc.com/threads/73743-midi-tune-select-problem?p=333274&viewfull=1#post333274)

High level, implement MIDI 1.0, HRV and and MPE specifications

- My own library.

Doesn't yet exist as a library but verious sketches implement parts.

Voice allocation is not in that library, module specific.

## Note range

Range of [musical frequencies for MIDI notes](https://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies) assuming :

- Equal temperament tuning
- MIDI 60 is C4
- A4 is concert A, 440Hz
- C4 is 0V, Doepfer Eurorack standard
- pitch CV range +5V to -5V
- DAC verification should avoid the note extremes to minimize linearity error

127 (G9)           = 12543.85
120 (C9, 5V)       =  8372.02
119 (B8, 4 11/12V) =  7902.13
117 (A8, 4.75V)    =  7040.00 exactly
108 (C8, 4V)       =  4186.01
105 (A7)           =  3520.00 exactly
93  (A6)           =  1760.00 exactly
81  (A5)           =   880.00 exactly
69  (A4)           =   440.00 exactly
60  (C4, 0V)       =   261.63
57  (A3)           =   220.00 exactly
45  (A2)           =   110.00 exactly
36  (C2, -2V)      =    65.41
33  (A1, -2.25V )  =    55.00 exactly
24  (C1, -3V)      =    32.70
21  (A0, -3.25V)   =    27.50 exactly
16  (E0)           =    20.60
9   (A-1, -4.25V)  =    16.35 exactly

Note that:

- A8 is a convenient 4.750 V corresponding to an exact integer frequency 7040 Hz
- C8 is a convenient 4v for testing, and also corresponds to the _highest_ note on an 88-key piano
- C2 is -2V and also the _lowest_ note on an 88-key piano
- A2 is a convenient -2.250 V corresponding to an exact integer frequency 55 Hz
- E0 is the lower limit of audibility
- A-1 is -4.25V and exact 16.350 Hz, useful for auto tuning
- MIDI notes 121 to 127 are unreachable with a ±5V DAC swing if the C4 = 0V Doepfer tuning is maintained. Also not musically useful notes.
- MIDI notes 0 to 15 inaudible, so not musically useful

## Note priority

Dispatching key events to the two channels: first one is lowest note priority, second channel is highest note priority. So can be used as one of two popular monosynth styles, as well as duophonic.

- [Priorities & Triggers](https://www.soundonsound.com/techniques/priorities-triggers)

## Smoothing and update rate

Interesting quote [from Haken MPE+](https://www.hakenaudio.com/mpe):

> Data smoothing is not a bad thing for the synthesizer to do — in fact, the Sampling Theorem tells us that smoothing is always necessary to avoid aliasing!  Aliasing is not a problem unique to audio sample streams, but also X,Y,Z control streams encoded in Midi. The common sample rates for audio are 44.1, 48, or 96 kHz.  The effective sample rate for the most popular MPE controllers’  Z data is ~25 Hz, and ~500 Hz for the Continuum Fingerboard. MPE+ lets the controller specify the best amount of smoothing to avoid update noise (aka zipper noise or aliasing) but still retain the finger motion information in the data.  [Clarification: The Continuum internally has an effective sample rate of ~3 kHz for Z; this data is processed to create the optimized ~500 Hz effective sample rate control stream for Midi transmission.]

and, from the [Sound on Sound review of the Expressive-E Osmose](https://www.soundonsound.com/reviews/expressive-e-osmose):

> The Osmose’s internal control scheme is not based on MPE MIDI but an extension of it termed MPE+ that improves resolution in the domains of time (for very quick response) and magnitude (for very smooth). Interestingly, it does away with the concept of key velocity (or, in MPE terms, Strike Value) completely, and instead derives note onset information from ‘attack trajectory’, a much more precise assessment of key travel over time that can take into account key dip as well as rate and acceleration.

> However, the Osmose will function perfectly well as an MPE or basic MIDI controller. It does that by deriving velocity data (or MPE Strike Value) from the initial key touch, and various types of channel or polyphonic pressure from the aftertouch. An ‘Ext MIDI’ mode, with a dedicated button, toggles the Osmose’s interface entirely over to MIDI control functions. And then a handful of dedicated MIDI control profiles are provided, including a full MPE implementation, ‘classic keyboard’ (which turns off all expressive features except velocity and channel aftertouch), and ‘poly aftertouch’ (which gives good compatibility with synths like the Oberheim OB‑6 that offer this response, but not full MPE). The point in the key dip at which notes trigger varies between modes: higher/earlier for MPE, and lower/later for ‘classic keyboard’. That’s some real sophistication, and very nicely considered.

See also the section [**14-bit Controllers are Flawed**](https://www.hakenaudio.com/mpe) and [Control Voltage Converter (CVC) for Analog Synthesizers](https://www.hakenaudio.com/voltage-converter) which has W,X,Y,Z outputs on four channels (using I2C, oddly), and similar [Evaton Technologies µCVC](https://www.evatontechnologies.com/mcvc).

Useful discussion on [Osmose, CC87, CC74 for 14-bit output](https://community.vcvrack.com/t/osmose-expressive-e/19100/9)

[MIDI BigBuffer to avoid losing data with high throughput](https://forum.pjrc.com/threads/70227-USBhost_t36-MIDI-losing-midi-note-on-off-events-(Teensy-3-6)-during-modwheel-change?p=306436&viewfull=1#post306436)