
# Performance (Control Change) DAC

One octal 14-bit DAC does 2 voices  of attack veocity, lift velocity, pressure, glide. High Resolution Velocity prefix supported.

*Note* final MPE specification reduces precision of pressure & glide to 7bit. However, later expansion to 14bit not ruled out. Implement 14bit as an experiment; 7bit still supported. See [MIDI](./MIDI.md) on precision, smoothing and update rate.

Similar circuitry also used to provide the global performance controls (mod wheel, sustain, etc). Can support 14-bit MSB/LSB Control Change, and also RPN and NRPN.

## Chip selection

Needs to be 14-bit capable to fully implement the HR aspect, but precision requirement lower than for pitch; most devices send 7bit data and the ones that are high resolution have ENOB less than 14. 1 LSB at 14bit is 300μV but 1 LSB at 7bit is a huge 39mV. Aim for at least 10 ENOB (4.8mV), preferably 12 ENOB (1.2mV).

_AD5648-2_ octal 14-bit DAC ($19.91, got one) NO unsuitable due to zero and gain offsets. Internal VRef of 2.5V gives unipolar 5V output. Better performance from the -2 devices at 5V than the -1 devices at 3V3. Fig. 31 shows **100mV** (!!) error when sourcing or sinking 2mA. Internal 2V5 reference with 2x gain, can use external (5V) ref. Notice most of the graphs in datasheet use an external reference :)
Vref seems to give a couple of mV error in output wrt temperature. Fig.54 shows 4mV error in internal ref wrt temperature.

**DAC8168C** (or DAC8168ICPW  seems more available) octal 14-bit DAC (TSSOP-16, $27.26, **got 3, 4 Jan 2018** ) much better offsets ±1 / ±4 mV.

(_Note to self:_ This is the 14-bit version of the DAC8568 which I used in another  (mainly through-hole) project, OctalDAC wavemix).

TSSOP-14 DAC has neither CLR nor LDAC, only needs 3 lines through level shifter. However best (C) grade is TSSOP-16.

![DAC](./img/DAC8168-16-pinout.png)

## Power

Clean 5.5V. Most graphs in datasheet are at AVdd =  5.5V.

## Digital interface

Logic level high is 0.625 * Vdd so 3.4V, ie it is a 5V logic device with that Vdd.

Needs level shifter for SPI. Use second SPI channel on Teensy 4.1. One quad shifter handles CS (SYNC) plus SCLK and MOSI and optionally LDAC.

**74AHCT125** Quad Level-Shifter (SSOP-14, **got 10 Jan 2018** plus 10 DIP)  good for SPI, fast enough.
Vdd abs max -0.5V to +7V so good for 5V5.

Enables are active low, so tie all to ground.

[Check for MISO tri-state behavior](https://www.pjrc.com/better-spi-bus-design-in-3-steps/) with a pair of 10 resistors.

## Vref

DAC8168C Internal 2.5V VRef has initial accuracy of 100μV (0.004%), good temperature stability (especially 2x gain C grade) and long-term drift. External Vref not needed.
Internal Vref in DAC8168C is adequate for general non-pitch CV duties. Output is 0 to 5V.
Note internal Vref is disabled by default, see datasheet Table 5 for enable command.

## Initial accuracy

From **DAC8168C** datasheet, assuming internal Vref:

INL 300μV / 1.2mV (±1 / ±4 LSB, typ / max)

DNL 30μV / 150μV (±0.1 / ±0.5 LSB)

Gain error 0.5mV / 7.5mV (±0.01% / ±0.15% of FSR) with  5μV (±1 ppm of FSR)/°C drift

Offset error ±1 / ±4 mV with ±0.5 μV/°C drift

Zero error 1mV / 4mV with  ±2 μV/°C drift

At 0 to 5V, 1LSB is 306μV. TL071 with 3mV offset is now significant wrt typical (but not max) offsets.

DAC is not trimmable without external conditioning circuitry.

Long term drift (Fig.7) inside ±500μV (±100ppm) /2k hours

## Line regulation

10μV/V is fine here

"The power applied to AVdd should be well-regulated and low noise. Switching power supplies and dc/dc converters often have high-frequency glitches or spikes riding on the output voltage. In addition, digital components can create similar high-frequency spikes as their internal logic switches states. This noise can easily couple into the DAC output voltage through various paths between the power connections an analog output"

So powering from the analog, regulated 5V5 supply helps here.

## Load

30μV/mA (sourcing)

Internally buffered, but _poor_ load regulation; will need external buffer to protect from modular environment (like getting 12V plugged into an output by mistake), to provide a constant high-impedance load for the DAC, and to give current drive with low output impedance.

> **8.2.3 Output Amplifier**
> The output buffer amplifier is capable of generating rail-to-rail voltages on its output, giving a maximum output range of 0V to AVDD. It is capable of driving a load of 2kΩ in parallel with 3000pF to GND. The source and sink capabilities of the output amplifier can be seen in the Typical Characteristics. The typical slew rate is 0.75V/μs, with a typical full-scale settling time of 5μs with the output unloaded.

## Support circuitry

4.7μF + 100nF supply bypass caps on AVdd.

> a 1μF to 10μF capacitor and 0.1μF bypass capacitor are strongly recommended.

Optional 150nF cap for lower noise on Vref (likely not needed, but place footprint on board).

## Output conditioning

3mV offset on TL074B is 3LSB; not that significant, though if an inexpensive alternative op-amp is better (1mV or less, does not need to be high precision) use that.

Quad amps: TL074A is cheap (TL074ACDT 3mV/6mV, $0.867/10) but consider **OPA4172IPW** (200μV/1mV, 10 V/μs, 75mA 60Ω, $3.32/10, TSSOP-14, currently in stock) or **OPA4197ID** (25μV/100μV, 20 V/μs, $3.53/10 **GOT 10**), **OPA4202ID** (20μV/250μV, low slew rate 0.35V/μs, $2.81/10 SOIC-14 in stock) as non-inverting output buffers. Use innie current limiting resistor.

OR to save needing bipolar power, use quad RRIO low voltage single-supply op-amps. Key features are swing to 0V (sing to positive rail not an issue due to 5V signal from DC nd 5V5 rail), current drive, offset. Must allow 5V5 rail. Must be unity gain stable.

| Op-amp    | Swing    | Offset (typ/max)   | Price        |
|:----------|----------|--------------------|--------------|
| TLV4316   | -/35 mV  | ±0.75/±3 mV        | **$1.06/10** |
| OPA4340UA | 1/5 mV   | ±150/±500 μV       | **$7.22/1**  |

Note [OPA4340UA](https://www.mouser.com/ProductDetail/Texas-Instruments/OPA4340UA?qs=wgAEGBTxy7nRw0DNGsw%252Bcg%3D%3D) has both _abs max_ and _recommended_ supply rail at 5V5. Much better swing to rail, 6x lower offset, 7x the price. Somewhat worse swing if load is below 50 kΩ, which is easily possible:

> A class AB output stage with common-source transistors is used to achieve rail-to-rail output. For light resistive loads (> 50 kΩ), the output voltage is typically a few millivolts from the supply rails. With moderate resistive loads (2 kΩ to 50 kΩ), the output can swing to within a few tens of millivolts from the supply rails and maintain high open-loop gain (see Figure 15).

Not exactly clear what they are dodging around, here:

> The OPA340 series operational amplifiers are fully specified from 2.7 V to 5 V. However, supply voltage may range from 2.5 V to 5.5 V. Parameters are ensured over the specified supply range—a unique feature of the OPA340 series.

DAC settling time, 1/4 scale to 3/4 scale is 10μs typ, so 20μs full scale and 40μs for fastest possible square wave = 25kHz in theory (will be slower). Spec sheet slew rate is 0.75 V/μs.

Better to use a slew limiter or low-pass filter on the output to avoid stair-stepping and VCA crackles; so one quad op-amp only does for 2 outputs. See circuit in TI [Single Op-Amp Slew Rate Limiter](http://www.ti.com/lit/pdf/TIDU026) for slew limiter. Needs fast recovery from overload, adequate slew rate. 5V/ms (160Hz lowpass) seems like a good starting point. Breadboard then examine stepped ramp on scope to determine optimal slew rate. "Op-amp slew rate = 10x-100x slew rate limiter value." OPA4202 likely too slow.
OPA4192 out of stock at Mouser. OPA4187?? (0.2V/μs $5.60/10 in stock) seems over-specified for Vos, slow slew, and expensive

OR do slew in software so it is configurable and can be disabled for true 14bit inputs.
Uses half the amount of op-amps (one per output). Needs testing. Use [ellapsedMicros](https://www.pjrc.com/teensy/td_timing_elaspedMillis.html) variables to update DAC on a regular clock, eg 10kHz. Interleave the updates for the two DACs so they don't coincide.

Capture some MIDI output from the Roli Seaboard to see what rate of changes is sent on changes in pressure etc; suspect it is fairly slow (and 7bit).

Test using pitch bend, or slow filter sweeps with some resonance

Remember same board is used for the analog general CC CV section, so also capture some data on that (mod wheel, breath, etc). Some of these are 14bit CC:

- Modulation Wheel (CC01+CC33)
- Breath (CC02+CC34)
- Expression Controller (CC11+CC43)

might also be driven by 14bit NRPN. Avoid over-slewing on 14bit inputs, which argues for software limiting.

- [Mathematical analysis of lag/slew](https://modwiggler.com/forum/viewtopic.php?t=270580&sid=4bbb23507cc03653a38cad97c82f4882)
- [my slew rate question on MW in 2018](https://modwiggler.com/forum/viewtopic.php?t=202240)
- [interpolation in midi-vs-cv](https://www.elektronauts.com/t/midi-vs-cv/156000/123)
- [zipper noise in MIDI to CV](https://gearspace.com/board/electronic-music-instruments-and-electronic-music-production/718498-zipper-noise-kenton-pro-2000-mkii.html)
- [MIDI vs I2C CV control on Haken Continuum driving Moog Voyager](https://www.hakenaudio.com/voltage-converter) notes excessive smoothing in Moog MIDI to CV generation limits expressive attack

Bipolar operation is possible (datasheet p.47) but not needed here, all the MIDI CC are unipolar for the MPE performance controls. So *could* use singe-rail, rail-to-rail input and output op-amps which swing to 0V on input and output. Probably easier to use bipolar devices though.

Given wide variety of op-amp capabilities, fluctuating pricing and wildly varying lead times and availability, split perf dac board into the DAC part and the buffer/slew part so one can be built with cheap parts then another with better parts, for testing and to avoid replacing the DAC. Standardize on one op-amp footprint, so SOIC-14 quad or TSSOP-14 quad.

Op-amp board needs bipolar supply OR R/R in and out op-amps to ensure accurate 0V. Bipolar is easier. Check CMRR though.

## Code

Enable internal ref (disabled by default) in setup: command 090A0000h.

Then use broadcast mode to set all channels to zero (but zero is the C-grade power-on default).

See [32bit SPI to DAC8168](https://forum.pjrc.com/threads/72317-Dac8568-gt-dac8168?p=321896&viewfull=1#post321896) using SPI.transfer32()

[Asynchronous writes to multiple SPI channels](https://forum.pjrc.com/threads/61676-SPI-pins-teensy-4-1?p=245308&viewfull=1#post245308)

```C++
#define PerfDAC_CS1 32
#define PerfDAC_CS2 31
#define PerfDAC_CS3 30
#define PerfDAC_MOSI1 26
#define PerfDAC_SCLK1 27
pinMode(PerfDAC_CS1, OUTPUT);
pinMode(PerfDAC_CS2, OUTPUT);
pinMode(PerfDAC_CS3, OUTPUT);
SPI1.beginTransaction(SPISettings(24000000, MSBFIRST, SPI_MODE1));
```

## Fading (all voice 'attenuators')

(Not on same board).

Use 4 pots with rail-to-rail input and output buffer amps connected to 4 adc inputs. Teensy 4.1 only has 10bit (enob) ADC. Then use these values to digitally scale the 14bit performance values (on *all* voices), providing per-performance-output attenuator function across all channels.

Experiment to see if buffering is needed. 10k pots should be low enough to drive ADC but check current draw of 4 in parallel (1.3mA, seems fine).

Allow a dead zone at each end so easy to get "fully off" or "fully on" values. Experiment with curve between the two extremes, linear is probably not what is wanted here.

- [Alpha 9mm T18 shaft pot, 10k](https://www.thonk.co.uk/shop/alpha-9mm-pots-vertical-t18/) [GOT]
- [T18 micro knobs](https://www.thonk.co.uk/shop/micro-knobs/) [GOT]



 Use Responsive Analog Read:

- [Writing a better noise-reduction algorithm](http://damienclarke.me/code/posts/writing-a-better-noise-reducing-analogread)
- [RAR on GitHub](https://github.com/dxinteractive/ResponsiveAnalogRead)
- [Example responsive analog read](https://forum.pjrc.com/threads/45376-Example-code-for-MIDI-controllers-with-Pots-and-Buttons)

May need buffering, though this project does not need a fast rate of reading these pots. Would use a 3V3 RR-I/O op-amp.

"It is advisable to place a 10nF capacitor from the wiper to ground against RF and other interference." [Gerrit, PJRC forum](https://forum.pjrc.com/threads/55126-MIDI-Faderbox-fader-choice?p=197818&viewfull=1#post197818)

[Discussion of ADC input capacitance, SPICE models](https://forum.pjrc.com/index.php?threads/some-comments-on-circuits-using-the-analog-input.75431/#post-346211)

Maybe use [elapsedMillis](https://www.pjrc.com/teensy/td_timing_elaspedMillis.html) to only ready the pots every so often.

## Board

Three separate boards, stacking and parallel to panel, for:

- DAC and level shifter (5V5 power)
- op-amp buffers/slew (±12V power, now changed to 5V5 power)
- jacks

SYNC is active-low CS. Din is MOSI. SCLK is SCLK :)
Synchronous update mode so LDAC tied to GND.
But check this allows updating only some channels, not (re)writing to them all.
Better to bring out LDAC through level shifter so there is flexibility on controlling it from Teensy or tying low permanently.

### DAC board

#### Schematic v0.1

![Schematic](./img/PerfDAC-schematic.png)

#### DAC PCB v0.1

> We detected a 2 layer board of 1.33 x 1.03 inches (33.8 x 26.3mm)
> 3 boards will cost $6.85

Board ordered 29 Jan 2024

![top](img/PerfDAC-top-osh.png)

![bottom](img/PerfDAC-bottom-osh.png)

3D views, front and back

![3D](img/PerfDAC-3D.png)

![3D back](img/PerfDAC-3D-back.png)

_Notice that the buffer board connections are on the other side of the board!_

This board will not fit. Needs a v0.2 with the connectors staggered, see buffer board. However, still build one for testing.

### DAC BOM v0.1  (per DAC board, need 2)

#### Resistors

(1) R1 10k 0805 any tolerance **GOT**
(1) R2 0R 0805 or solder bridge, optional (**GOT**)

#### Capacitors

(1) C1 Kemet C1206X105K3RACTU  25V 1μF X7R 10% 1206 ceramic $0.839/10 = **$8.39 GOT**
or
(1) [Kemet C1206C475K3RACTU](https://www.mouser.com/ProductDetail/KEMET/C1206C475K3RACTU?qs=sGAEpiMZZMvsSlwiRhF8qok1fkaRbtAXC1WGxIuEg%252Bg%3D) 25V 4.7μF X7R 10% 1206 MLCC $0.156/10 =  **$1.56**

(2) C2, C3 Kemet C1206C104K3GEC7210 25V 100nF C0G 1206 ceramics $0.051/100 = **$5.10 GOT**

(1) C4  [Murata GRM31C5C1H154JE02L](https://www.mouser.com/ProductDetail/Murata-Electronics/GRM31C5C1H154JE02L?qs=qSfuJ%252Bfl%2Fd70Xb0PJyHSQA%3D%3D) 50V 0.15μF 1206 C0G $0.241/10 = **$2.41**
spec sheet says **150nF or higher** and fig 8 shows 4.8μF so try 470nF?

OR

(1) C4 [Murata GRM31C5C1E474JE01L](https://www.mouser.com/ProductDetail/Murata-Electronics/GRM31C5C1E474JE01L?qs=hd1VzrDQEGj8GOC097EZLg%3D%3D) 25V 470nF C0G 1206 $0.429/10 = **$4.29**

#### DAC

(1) DAC8168ICPW  14-bit octal $28.34/1 = **$28.34 GOT** but _lost_, get 3 more!

#### Logic

(1) [SN74AHCT125QDRQ1](https://www.mouser.com/ProductDetail/Texas-Instruments/SN74AHCT125QDRQ1?qs=zhgwDAIOVxtE5BLiD9k5oQ%3D%3D) Quad bus buffer $0.385/10 = **$3.85 GOT**

### Buffer board

Eight identical copies of this:

![one buffer](./img/perfdac-buffer-schematic-one.png)

Because of limited board space, the buffer board takes 5V5 from the power board then passes it on to the v0.2 DAC board.

 > We detected a 2 layer board of 1.83 x 0.92 inches (46.5 x 23.4mm)
> 3 boards will cost $8.40

![buffer board](./img/perfDAC_buffer_board.png)

![board 3d front](./img/PerfDAC_buffers_board3D_front.png)

![board 3d back](./img/PerfDAC_buffers_board3D_back.png)

- Two rows of connectors to jacks board are 40.894mm apart, measured from the connector centres.
- Two rows of connectors to DAC board are 33.274mm apart.

Ordered from OSH Park 19 May 2025.

### Buffers BOM

#### Op Amps

(2) [OPA4340UA SOIC-14](https://www.mouser.com/ProductDetail/Texas-Instruments/OPA4340UA?qs=wgAEGBTxy7nRw0DNGsw%252Bcg%3D%3D)  **$7.22/1** get 6

#### Resistors

(8) [47R 0805](https://www.mouser.com/ProductDetail/YAGEO/RC0805FR-1347RL?qs=m6lXFsvg5e1DgwoB9H70oA%3D%3D) **$0.012/100 = $1.20** get 100

(8) [4k7 0805](https://www.mouser.com/ProductDetail/YAGEO/RC0805FR-134K7L?qs=m6lXFsvg5e0pEdZpmZIKLA%3D%3D) **$0.01/100 = $1.00** get 100

#### Capacitors

(8) [100pF 0805](https://www.mouser.com/ProductDetail/KEMET/C0805C101K3GACTU?qs=8Ol5LxMUegri7nW0mRT3Bw%3D%3D) **$0.077/10 = $0.77** get 30

(2) [100nF 1206](https://www.mouser.com/ProductDetail/KEMET/C1206C104K3GEC7210?qs=55YtniHzbhB4%2FhY0bEvvcQ%3D%3D) **$0.392/10 = $3.92** get 20

|Reference                   |Value             |Qty|
|----------------------------|------------------|---|
|C5,C7,C8,C9,C10,C11,C12,C13 |100pF 0805        |8  |
|C6,C14                      |100nF 1206        |2  |
|J1,J2                       |Conn_01x06_Pin    |2  |
|J3,J4                       |Conn_01x05_Socket |2  |
|PWR1                        |Conn_01x04_Pin    |1  |
|R3,R6,R9,R10,R15,R16,R17,R18|47R 0805          |8  |
|R4,R5,R7,R8,R11,R12,R13,R14 |4k7 0805          |8  |
|U1,U2                       |OPA4340UA SOIC-14 |2  |

![board-photo](./img/buffers-photo.jpg)

## Workplan

- [ ] Breadboard op-amp slew circuit, test with any DAC stepped at 7bit resolution. Get good capacitor values for sufficient slew.
- [x] Check existing parts stock, to be sure.
- [x] Select an op-amp for post-dac conditioning.
- [ ] Lay out panel jack PCB check enough room for connectors and easy to route
- [x] Lay out perf DAC board
- [x] send DAC board to OSH Park to enable testing a populated DAC board.
- [x] Order DAC parts
- [ ] Build perf DAC board
- [ ] Test perf DAC board with 5V5 supplies.
- [ ] Lay out perf DAC v0.2 board
- [x] Lay out buffer board
- [x] Send buffer board to OSH Park
- [x] Order buffer parts
- [ ] Build and test buffer
- [ ] Test perf DAC with output to  buffer board
- [ ] Order panel board from OSH Park
- [ ] Assemble completed perf DAC, measure, check against panel drawing