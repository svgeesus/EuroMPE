# Panel

Panel height 128.5mm but with mounting holes and rails, 
Usable panel height 112mm.

Max PCB height 108mm. 

Width for the two channels part is 6HP (30mm)
because Adafruit TLC59711 breakout is 28.38mm.

Either 6 or 7 jacks (4 perf, one or two pich, gate) 
which use 6.2mm holes,
plus 5mm LED, 7.6mm square so allow 9mm?

- (108 - 9 - (7 * 6.2) ) / 6 = 9.26mm between jack holes.
- (108 - 9 - (6 * 6.2) ) / 5 = 12.36mm between jack holes.

Thoniconn jacks are 12mm high, 9mm wide.

- (108 - 9 - (7 * 12) ) = 15mm = 3mm between jacks, seems tight.
- (108 - 9 - (6 * 12) ) = 27mm = 5.4mm between jacks.

Perf DAC boards are 4 jacks high by 2 wide.

Need another set of controls next to that,
4 pots for perf faders and one input for calibration.

[Teensy 4.1](https://www.pjrc.com/store/teensy41.html) is 61.7mm (around 12hp) long, 17.8mm wide, but allow extra space for whatever is plugged into the USB device port.

Display is likely to be above the Teensy and PCB is 20 x 35mm. 
Panel Size：30.00mm × 11.50mm × 1.45mm

Line of pushbuttons under display, 4 plus "back".

Need room for rotary encoder. Could be right of display, above the pots/input lane.

Need global outs, 8 jacks, same as perf dac board but rotated to be 4 wide and 2 high.

USB Host, USB device, DIN Midi (not on PCB; panel mount).

