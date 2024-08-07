# Panel

Doepfer standard height is **128.5mm** and 2mm thickness but perhaps for a large panel with many heavy components mounnted behind it, 2.5mm for rigidity is better.

## Max PCB dimensions

Panel height 128.5mm but with mounting holes and rails, 
Usable panel height 112mm.

Max PCB height **108mm**.

## Panel width

Current design is **24HP** = (24 × 5.08 = 121.92) - about 0.3 = 121.60mm, see [A100 Construction Details](https://www.doepfer.de/a100_man/a100m_e.htm).

[Panel concept](./mock-channel-front-v2-bigscreen-wider.svg) (SVG)

![panel](./img/panel-front.png)

and also [current panel with boards and components](./mock-channel-v2-bigscreen-wider.svg) (SVG)

![panel](./img/panel.png)

Completely blank 24Hp panel (black anodizing, white infill, no holes at all) is $34.00 and with 4 mounting holes, $38.23.

## Front jack spacing for duophonic output

8 Thoniconn PJ398SM jacks (4 perf, two pitch, trigger, gate)
which use 6.2mm holes,

Thoniconn jacks are 12mm high, 9mm wide.

![Thonkiconn](./img/thonkiconn-pj398sm.jpg)

Staggered spacing allows more room for inserting jacks; single-in-line looked neater but would not fit (9 jacks plus LED all in one line) and impossibly cramped.

Consider using reverse-mounted pin connectors for shallower depth; electronics boards need to be snug against thr jack boards to not foul the MPU board.

## Perf DAC boards

Perf DAC boards are 4 jacks high by 2 wide (for the front panel, jack board).

Behind that an op-amp board for buffering and slew, likely similar size;
behind that the Perf DAC board itself, likely smaller.

Need another set of controls next to that,
4 pots for perf faders.

## Pitch DAC boards

Pitch DAC boards are 2 jacks high by 2 wide (for the front panel, jack board).

Behind that a sandwich (three horizontal boards) two pich dac with the vref in between.

- (2 * 9) + 5.08 = **23.08mm** high
- (2 * 12) = 5.08? = again 24 to 30mm wide.

With paired outputs they are now 1 high by 4 wide,
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

Also needs the two tuning inputs.

Check: 51.24 + 18 + 24 + 5.08 + 5.08 = 103.4 so fits nicely in 108mm.
But is now super wide and a bit cramped.

5mm LED body is 7.6mm square. Body is 2.5mm above PCB while Thonkicon jacks are 9mm above PCB so need to raise them up by 6.5mm ...  daughter board? Will still shine through if a bit lower, but not that much. LED is 5mm diameter so to be flush with top of a 2.5mm panel, raise by 4mm.

![LED](./img/FD-115TRGB.png)

Maybe make a tiny panel (2 jacks plus LED) to experiment with.

## MPU board

Main constraint in [layout mockup](./mock-channel.svg) is mechanical attachment of MPU board to display/button/encoder board (at the top) and fader board (at the bottom) while clearing  the global output (secondary perfDAC) board which will need to be fairly shallow.

[Teensy 4.1](https://www.pjrc.com/store/teensy41.html) is 60.96mm long plus 0.7mm for the USB connector = 61.7mm plus (if a cable is plugged in there, but not if a cable goes to D+ and D- pads.) around 40mm more to leave room for the USB plug. Width is 17.78±0.6 so say 18.3.

![Teensy 4.1](./img/dimensions_teensy41.png)

### Display

The Adafruit [1.3" 128x64 OLED graphic display](https://www.adafruit.com/product/938) **GOT** I2C or SPI.

![oled screen photo](./img/oled_13.png)

35.19mm wide by 35.18mm high (including lower mounting lugs). Display width 35.19mm, height 19.0mm.

![1.3"](./img/adafruit-1.3-128x64-fabpring.png)

### Buttons

Group of pushbuttons to left of display, 3 plus "back". See [User Interface](./ui.md)

[Adafruit Mini Soft Touch Push-button Switches](https://www.adafruit.com/product/3983) 6x6mm SMD.

![tact photo](./img/tact.png)

![soft touch](./img/adafruit-soft-touch-push.png)

3.5mm body height above pcb, 5mm to top of button. Will need to be on a small daughter board to be at same height as the top of the screen.

### Encoder

Rotary encoder to right of display.

[Rotary Encoder - Illuminated](https://www.sparkfun.com/products/15141) Sparkfun
[Bourns PEC11 Series encoder, Adafruit](https://www.adafruit.com/product/377)

Use Sparkfun encoder as placeholder, not clear RGB led is needed here. It does need to be firmly attached to panel as it supports this board and then the MPU board too. Better to use mounting standoffs as well.

Sparkfun 14mm wide 13.2m high but that excludes pins (14mm with pins). M9 threaded shaft, 6mm turning shaft. M9 implies 11mm clearance hole diameter **but check this**

Bourns PEC11 has M7 x0.75 shaft, so 8mm clearance hole diameter **but check this**

## Perf fading pots

Song Huei short trimmer 9mm pots [from Thonk](https://www.thonk.co.uk/shop/short-trimmer-pots/). 10k gives 330μA per pot, *4 is 1.32mA total which seems fine. May be enough to avoid a buffer for the ADC?

![trimmer](./img/SongHuei-trimmer.png)

10.6mm wide at PCB footprint, 9.7mm wide body, 6mm shaft. 12.5mm high including pins. Shaft 5.5mm from top of body. Measure width at bushing, hole should be a snap fit for the trimmer.

These are _not threaded_ so cannot be used to attach the front PCB to the panel. Maybe make one PCB for the perfdac jacks and the trimmers?

## Global outputs

Need global outs, 8 jacks, same as perf dac board but rotated to be 4 wide and 2 high.

## MIDI inputs

USB Host, USB device, DIN Midi (not on PCB; panel mount).

- [Panel mount USB-B to Micro-B cable](https://www.sparkfun.com/products/15463)
(M3 holes 28.5mm spacing) **GOT**
- [USB host cable](https://www.pjrc.com/store/cable_usb_host_t36.html) no data sheet; **GOT**
- [Deltron 650-0500](https://www.mouser.com/ProductDetail/Deltron/650-0500?) **GOT**

![Deltron](./img/Deltron-650-0500.png)

Deltron 5-way 45° needs 8mm radius circular cut-out plus 3.2mm wide 1.2mm rect key. Outer flange is 17.85mm diameter,

USB-B socket is 12mm wide, 11.16mm high. Hole is 8.45mm high, 7.78mm wide. Internal block 3.18mm high 5.6mm wide.

![USB-B](./img/USB-B-receptacle.png)

Dimensions of external bracket unknown, measure. For now assume 34mm wide, 16mm high. Actually measures 36.91mm wide, 14.17mm high.

![USB-B panel](./img/USB-panel.png)

Dimensions of PJRC Host cable 11.15mm high and 38.85mm wide. (33.72 + 24.76) ÷ 2 = 29.24 centers (really?).
Dimensions of receptacle 12.5mm by 5.12mm.

![USB-A](./img/USB-A_receptacle.png)

**NO** If adding a TRS MIDI input, use a circuit which accepts either Type-A or Type-B, like [this one](https://github.com/kay-lpzw/LPZW_TRS_MIDI/)

## Panel measurements 

(relative to bottom left corner at (x,y) = (0,0), in mm)

- width 121.6
- height 128.5

### Mounting

- mounting hole bottom-left: (7.45, 3.0) double, dia 6.1 width 3.5 rotate 90 cutter 2.4mm
- mounting hole top-left: (7.45, 125.4) double, dia 6.1 width 3.5 rotate 90 cutter 2.4mm
- mounting hole bottom-right (N=21): (114.13, 3.0) double, dia 6.1 width 3.5 rotate 90 cutter 2.4mm
- mounting hole top-right (N=22): (119.21, 125.4) double, dia 6.1 width 3.5 rotate 90 cutter 2.4mm

### LED

Assume a 5.2mm hole for a 5mm LED for now.

- low  ( 88.9,  114.35)
- high (109.9,  114.35)

### Jacks, Gate and Trigger

Jacks use 6.3mm dia hole. 10mm between pairs of the same channel, extra 1mm to separate low and high channels.

- low gate  ( 83.9,  101.09)
- low trig  ( 93.9,  101.09)
- high gate (104.9,  101.09)
- high trig (114.9,  101.09)

### Jacks, tune

- low ( 88.9,  88.93)
- high (109.9, 88.93)
- text ( 99.4, 88.93)

### Jacks, PitchDAC

- low 1  ( 83.9, 72.62)
- low 2  ( 93.9, 72.62)
- high 1 (104.9, 72.62)
- high 2 (114.9, 72.62)

### Jacks, PerfDAC

13mm spacing vertically, 21mm horizontal

#### Strike

- low ( 88.9, 56.25)
- high (109.9, 56.25)
- text ( 99.4, 56.25)

#### Lift

- low ( 88.9,  43.25)
- high (109.9, 43.25)
- text ( 99.4, 43.25)

#### Slide

- low ( 88.9, 30.25)
- high (109.9, 30.25)
- text ( 99.4, 30.25)

#### Press

- low ( 88.9, 17.25)
- high (109.9, 17.25)
- text ( 99.4, 17.25)

### Pots

6mm shaft with bushing so 6.1mm hole

- strike ( 69.17, 56.25)
- lift   ( 69.17, 43.25)
- slide  ( 69.17, 30.25)
- press  ( 69.17, 17.25)

### Jacks, PerfDAC 2

6.3mm hole, 13mm horizontal, 15mm vertical spacing

- A ( 11.9, 72.03)
- B ( 24.9, 72.03)
- C ( 37.9, 72.03)
- D ( 50.9, 72.03)
- A text (11.9, 78.79)

- E ( 11.9, 57.03)
- F ( 24.9, 57.03)
- G ( 37.9, 57.03)
- H ( 50.9, 57.03)
- E text (11.9, 63.89)

### USB-B receptacle

Allow 1mm all round to cope with rounded corners on the front panel.

Mounting bolts are 28.5mm apart, assume m3 bolts for now so 3.6 clearance hole.

- hole 9.78 wide 10.45 high, center (9.49 30.42) corner radius 0.5mmm
- top mount (9.49 44.67)
- bottom mount (9.49 16.17)
<!-- thus in FPD coords, x = x +71.4; y = 128.5 - y -->
<!-- center-x -61.91 center-y 98.08 -->
<!-- <rect  style="fill: maroon; fill-opacity: 0.3" x="-65.8" y="93.85" width="7.78" height="8.45" /> -->

### USB-A receptacle

<!-- 12.5mm by 5.12mm -->
Allow 1mm all round for rounded corners? Mounting bolts 29.24mm apart, assume M3 so 3.6 clearance hole.

- hole 7.12 wide 14.5mm high, center (27.42,30.42)
- top mount (27.42, 45.04)
- bottom mount (27.42, 15.8)
<!-- center-x -43.98 center-y 98.08 -->
<!-- <rect  style="fill: maroon; fill-opacity: 0.3" x="-47.54" y="90.75" width="7.12" height="14.5" /> 
-->

### DIN receptacle

16mm circular hole with 3.2 x 2.4 rectangle at top, , center is 0.6mm above circular hole edge.
Use same y-dimension as the two usb receptacles, was 30.46

- circle (45.45, 30.42) radius 8
- rectangle center (45.45, 38.42) width 3.2 height 2.4

<!-- thus in FPD coords, x = x +71.4; y = 128.5 - y -->
<!-- <circle style="fill: maroon; fill-opacity: 0.3" cx="-25.95" cy="98.04" r="7.6"/> -->
### Screen

Display width 35.19mm, height 19.0mm

- screen center (38, 101) width 35.19 height 19

## Work Plan

- [x] Experiment with font sizes on 128x64 display
- [x] Order the tact switches **2023-11-04, GOT**
- [ ] Mock up starting menu with Tune, Conf, Run
