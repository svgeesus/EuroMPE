# Panel

## Max PCB dimensions

Panel height 128.5mm but with mounting holes and rails, 
Usable panel height 112mm.

Max PCB height **108mm**.

## Width of two output channels

Width for the two channels part is 6HP (30mm)
because Adafruit TLC59711 breakout is 28.38mm.

If using paired jacks (gate and trig, two pitch)
then 8HP (40mm).

[Panel mockup (svg)](./mock-channel.svg)

## Front jack spacing for duophonic output

8 Thoniconn PJ398SM jacks (4 perf, two pitch, trigger, gate)
which use 6.2mm holes,
plus 5mm LED, 7.6mm square so allow 9mm?

![LED](./img/FD-115TRGB.png)

- For 8 jacks: (108 - 9 - (8 * 6.2) ) / 7 = 7.06mm between jack holes.
- For 7 jacks: (108 - 9 - (7 * 6.2) ) / 6 = 9.26mm between jack holes.
- For 6 jacks: (108 - 9 - (6 * 6.2) ) / 5 = 12.36mm between jack holes.

Thoniconn jacks are 12mm high, 9mm wide.

![Thonkiconn](./img/thonkiconn-pj398sm.jpg)

- (108 - 9 - (8 * 12) ) = 3mm will not fit!
- (108 - 9 - (7 * 12) ) = 15mm = 3mm between jack bodies, seems tight.
- (108 - 9 - (6 * 12) ) = 27mm = 5.4mm between jack bodies.

Or, used width-wise,

- (108 - 9 - (8 * 9) ) = 27mm = 3.8mm between jack bodies, a bit tight.
- (108 - 9 - (7 * 9) ) = 36mm = 6mm between jack bodies
- (108 - 9 - (6 * 9) ) = 45mm = 9mm between jack bodies.

So 7 jacks plus LED seems fine with jacks width-wise;
8 jacks plus LED may be ok but seems tight.

Pairing gate & trig, pitch outs gives effective 6 high,
but now wider.

## Perf DAC boards

Perf DAC boards are 4 jacks high by 2 wide (for the front panel, jack board).

Behind that an op-amp board for buffering and slew, likely similar size;
behind that the Perf DAC board itself, likely smaller.

- (4 * 9) + (3 * 5.08) = **51.24mm** long side,
- (2 * 12) + 5.08? = 24 to 30mm short side, probably squeeze up a bit to make less wide.

Need another set of controls next to that,
4 pots for perf faders and one input for calibration.

So keep these in a single line.

## Pitch DAC boards

Pitch DAC boards are 2 jacks high by 2 wide (for the front panel, jack board).

Behind that a sandwich (three horizontal boards) two pich dac with the vref in between.

- (2 * 9) + 5.08 = **23.08mm** high
- (2 * 12) = 5.08? = again 24 to 30mm wide.

Or with paired outputs they are now 1 high by 4 wide,
with spacing between the channels for visual grouping.
Put the jacks vertically in that case,
so 12mm high for jack body by 4 * 9 = 36mm wide
in 8HP that leaves (40 - 36) = 4mm between channels,
even if jacks but exactly against each other!

Stacking headers 2.54mm allow 3mm.
Tall ones are 8.5 to 11mm above the board; shorter ones are 7mm.
Ensure good mechanical support (square pins, not blade pins)

- (1 * 12) + (2 * 3) = **18mm** high
- (4 * 9) = 36mm wide, minimum plus inter-channel space **40mm**

## Gate, Trigger & LED board

Board for gate and trigger transistors, level translators for LEDs, horizontal behind jack panel and LED board.

- 9 + 9 + 9 + 2? = 27 - **29mm** high.

Check: 51.24 + 23.08 + 29 +5.08 + 5.08 = 113.48mm so will not fit in 108!

Will need to use staggered or offset jacks, not a single vertical row per channel.

- (1 * 12) + 3 + 9 = **24mm** high
- (4 * 9) = 36mm wide, minimum plus inter-channel space **40mm**

One edge connector for all connections so:

- 4 for gate and trig
- 6 for two rgb
- 4 for gnd

at 40mm max width, 2.54 per connection, up to 15. So 14 wide fits well.

Check: 51.24 + 18 + 24 + 5.08 + 5.08 = 103.4 so fits nicely in 108mm.
But is now super wide and a bit cramped.

## MPU board

[Teensy 4.1](https://www.pjrc.com/store/teensy41.html) is 61.7mm (around 12hp) long, 17.8mm wide, but allow extra space for whatever is plugged into the USB device port.

Display is likely to be above the Teensy and PCB is 20 x 35mm.
Panel Size：30.00mm × 11.50mm × 1.45mm

Line of pushbuttons under display, 4 plus "back".

Need room for rotary encoder. Could be right of display, above the pots/input lane.

## Global outputs

Need global outs, 8 jacks, same as perf dac board but rotated to be 4 wide and 2 high.

## MIDI inputs

USB Host, USB device, DIN Midi (not on PCB; panel mount).

- [Panel mount USB-B to Micro-B cable](https://www.sparkfun.com/products/15463)
(M3 holes 28.5mm spacing)
- [USB host cable](https://www.pjrc.com/store/cable_usb_host_t36.html) no data sheet; **GOT**