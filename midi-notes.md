# MIDI decoding & DAC driving

Based on Teensy 3.6, because it has two separate SPI busses easily accessible. The speed will also likely help when driving a lot of DACs ( 1 × 16bit on one bus, 5 8 × 14bit on the other). Fast single-precision float allows all frequency calculations to be done in float, then rounded for output to DACs. Teensy 3.6 also has USB Host.

Teensy 3.6 uses about 80 mA.

## MIDI connections

Primary interface is likely to be as a USB MIDI device, using the Teensy 3.x USB MIDI implementation. That connection requires a host, such as a computer running a DAW.

Secondary interface could be DIN MIDI input, using the Arduino MIDI library and an optocoupler. (Linnstrument has DIN MIDI option, as well as USB)

Third one could be USB Host MIDI: new in Teensy 3.6 with a separate USB Host connection port.

All three should register (the same) callbacks to allow input from any of the threee interfaces. All running at once? or switching between then from front panel.

Try to use direct manipulation rather than menu-diving for normal operation.

"The other alternative, which I would not recommend you try until you've at least checked with other ports and maybe measured with a voltmeter, is an external 5V supply with the VUSB-VIN pads cut apart on the bottom side of the Teensy. Then your 5V supply will power everything, and you can use one that's rated for 1A or more to make sure you have enough power."

## Per-voice MIDI connections

[Roli Seaboard MIDI expression](https://support.roli.com/manuals/roli-dashboard-creator-manual/#dashboard-features-and-settings-2-4-midi-settings-panel)

- Gate out (10V) with LED
- Pitch CV out 16bit (includes transpose, per-voice Glide & global pitchbend)
- Secondary Pitch CV out (derived from Pitch CV)
- Secondary Pitch modifier input
- Strike 14bit, (attack velocity, including CC88)
- Lift 14bit, (release velocity, including CC88)
- Slide 14bit, (per-voice vertical movement, CC74)
- Press 14bit, (per-voice channel pressure)

## Global channel MIDI outputs

8 14bit outputs, same board as the per-voice expression outputs

### Used by Roli Seaboard

- Slider1 (CC107)
- Slider3 (CC109)
- Slider3 (CC111)
- XY Pad X (CC114)
- XY Pad Y (CC113)
- Sustain (CC64) is just on/off gate though

Note that RISE auto switches between switch and continuous pedals on the pedal input. Check the other options in dashboard. A continuous pedal would be more useful.

### Used by Eigenharp

- Strip controller 1 -> MIDI continuous input 1 (CC 16)
- Strip controller 2 -> MIDI continuous input 2 (CC 17)
- Breath -> MIDI continuous input 3 (CC 18)
- Key pressure -> MIDI channel aftertouch
- Key roll -> MIDI pitch bend wheel
- Key yaw -> MIDI polyphonic aftertouch
- Pedal 1 -> MIDI damper pedal on/off (sustain) (CC 64)


### Other possibilities

- Modulation Wheel (CC01+CC33)
- Breath (CC02+CC34)
- Foot Controller (CC04+CC36)
- Expression Conrtoller (CC11+CC43)

## MPE handling

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

These would provide a second level of callbacks; the Euro-MPE implementation then provides funtions for each callback to actually control the DACs. The MIDI wrapper and the MPE wrapper should be re-usable for other Teensy projects; eventually becoming a library.

Useful but more basic (12bit, mono) MIDI2CV with Highest, lowest and Last note priorities:

- [useful T2 MIDI2CV](https://github.com/elkayem/usbMIDI2CV_MC/blob/master/usbMIDI2CV_MC.ino)
