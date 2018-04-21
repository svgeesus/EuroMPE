# MIDI decoding & DAC driving

Based on Teensy 3.6, because it has two separate SPI busses easily accessible. The speed will also likely help when driving a lot of DACs ( 5 or 9 × 16bit on one bus, 3 or 5 × octal 14bit on the other). Fast single-precision float allows all frequency calculations to be done in float, then rounded for output to DACs. Teensy 3.6 also has USB Host.

Teensy 3.6 uses about 80 mA.

## MIDI connections

Primary interface is likely to be as a USB MIDI device, using the Teensy 3.x USB MIDI implementation. That connection requires a host, such as a computer running a DAW.

Secondary interface could be DIN MIDI input, using the Arduino MIDI library and an optocoupler. (Linnstrument has DIN MIDI option, as well as USB).

- [Professional MIDI, by Graham Hinton](http://www.hinton-instruments.co.uk/reference/midi/promidi/index.htm)

[Deltron 650-0500](http://www.newark.com/deltron-components/650-0500/connector-din-jack-5-position/dp/69K6137?st=Deltron%20650-0500) DIN 5-pin panel socket (circular, nut fixing)

Third one is USB Host MIDI: new in Teensy 3.6 with a separate USB Host connection port. Allows direct, DAWless connection of an MPE keyboard or other controller.

All three should register (the same) callbacks to allow input from any of the threee interfaces. All running at once? or switching between then from front panel.

Try to use direct manipulation rather than menu-diving for normal operation.

"The other alternative, which I would not recommend you try until you've at least checked with other ports and maybe measured with a voltmeter, is an external 5V supply with the VUSB-VIN pads cut apart on the bottom side of the Teensy. Then your 5V supply will power everything, and you can use one that's rated for 1A or more to make sure you have enough power."

## Per-voice MIDI connections

[Roli Seaboard MIDI expression](https://support.roli.com/manuals/roli-dashboard-creator-manual/#dashboard-features-and-settings-2-4-midi-settings-panel)


- Gate out (8-10V) with LED, avoid PWM
- Pitch CV out 16bit (includes per-voice Glide. Channel Coarse Tuning, Channel Fine Tuning, & global pitchbend)
- Secondary Pitch CV out (derived from Pitch CV, voice modifier and global shift)
- Secondary Pitch modifier input
- Strike 14bit, (attack velocity, including CC88)
- Lift 14bit, (release velocity, including CC88)
- Slide 14bit, (per-voice vertical movement, CC74)
- Press 14bit, (per-voice channel pressure)

Because Lift is hard to use and kind of lame, check if MPE allows use of CC 71 (resonance) on voice channels; if so, allow switching this output to use that instead.

Middle C is MIDI note 60.

## Global channel MIDI outputs

8 14bit outputs, same board as the per-voice expression outputs.
4 on/off outputs (like gates, but 0 or 5V) for switches

To accomodate overall volume changes, and also the Mute all Notes CC, it may be useful to allocate one of these outputs to control an overall "volume" VCA. Mute All Notes drops all gates, but also sets volume to zero regardless of sustain or release settings.

CC68 Legato - ignore in MPE as monophonic only
CC84 Portamento Control - unclear whether to support this


### Used by Roli Seaboard

Roli MIDI ID code is 00H 21H 10H

- Slider1 (CC107)
- Slider2 (CC109)
- Slider3 (CC111)
- XY Pad X (CC114)
- XY Pad Y (CC113)
- Sustain (CC64)  on/off switch, see spec

Note that RISE auto switches between switch and continuous pedals on the pedal input. Check the other options in dashboard. A continuous pedal would be more useful. Is this CC04 Foot Controller?

### Used by Eigenharp

Eigenlabs has no MIDI ID code

- Strip controller 1 -> MIDI continuous input 1 (CC 16 + CC 48)
- Strip controller 2 -> MIDI continuous input 2 (CC 17 + CC 49)
- Breath -> MIDI continuous input 3 (CC 18 + CC 50)
- Key pressure -> MIDI channel aftertouch
- Key roll -> MIDI pitch bend wheel
- Key yaw -> MIDI polyphonic aftertouch (um, is that compliant with MPE spec?)
- Pedal 1 -> MIDI damper pedal on/off (sustain) (CC 64)

### Used by Linnstrument

Roger Linn Design ID code is 00H 01H 37H (suspended!)

Supports splits.

(to-do)

### Used by Futuresonus Parva (v61 on)

- Slide CC74 hardwired to VCF cutoff
- CC1 Mod Wheel
- CC2 Breath Controller

The current Parva MPE implementation only supports one zone

https://community.futureson.us/t/firmware-v61-release/503

### Used by K-Board 4

- Sustain
- Expression


### Other possibilities

14-bit controllers

- Modulation Wheel (CC01+CC33)
- Breath (CC02+CC34)
- Foot Controller (CC04+CC36)
- Expression Controller (CC11+CC43)

switches

- Portamento (CC65)
- Sostenuto (CC66) needs to mark all notes that were currently on
- Soft (CC67) affects global volume
- Legato (CC68) not clear how to handle this in EuroMPE
- Hold 2 affects gates

## MIDI 1.0 & MPE state handling

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

These would provide a second level of callbacks; the Euro-MPE implementation then provides functions for each callback to actually control the DACs. The MIDI wrapper and the MPE wrapper should be re-usable for other Teensy projects; eventually becoming a library.

## Comparable projects

Useful but more basic (12bit, mono) MIDI2CV with Highest, lowest and Last note priorities:

- [useful T2 MIDI2CV](https://github.com/elkayem/usbMIDI2CV_MC/blob/master/usbMIDI2CV_MC.ino)

8-voice Eurorack module (but just gate, Pitch CV, Velocity and one other). Not quite MPE. 12V gate, 0-12V pitchCV at unspecified accuracy.

- [Polyend Poly](https://polyend.com/poly/) .
