# Voltage ref 

Linear LT1236LS8 may be an alternative to [MAX6226](./voltage-ref-MAX6226.md) if MAX6226_50 cannot be made to work reliably.

Both use ceramic LS8 package with copper pads underneath the device. This gives improved stability, particularly to moisture and PCB flex, but makes soldering difficult without a reflow oven. Hot air tends to blow the lightweight ceramic chip off the board, so holding in place with tweezers is required.

![pinout](./img/LT1236LS8-pinout.png)

Vref board needs to provide both +5V and -5V to drive the pitch DACs.

## Absolute initial accuracy

Good initial accuracy ±0.05% (±2.5mV) for A grade; although less good than MAX6226_50 ±0.02% (±1mV). But it **is** trimmable, using a resistor, diode and multiturn cermet trimmer.

Pin 5 is trim, unconnected if not used; different from MAX6226_50 where pin 5 is output sense.

![trim](./img/LT1236LS8-trim.png)

## Temperature

Fairly good at 2ppm/C typ, 5ppm/C max, not as good as MAX6226_50 1ppm/C typ, 3ppm/C max.

## Thermal Hysteresis

Very good at 3ppm.

## Long term drift

> The data sheets of many references specify long-term drift—typically about 25 ppm/1000 hr. This error is proportional to the square root of elapsed time, so 25 ppm/1000 hr ≈ 75 ppm/year. The actual rate is likely (but not certain) to be somewhat better than this as the ageing rate often diminishes after the first few thousand hours. So, again, we have a figure around 14 bits.
[Choosing Voltage References](http://www.analog.com/en/analog-dialogue/raqs/raq-issue-114.html)

20ppm/1k hr (100μV), less good than MAX6226_50 11ppm/1k hr (55μV). But trimmable after pre-ageing.

## Current / Load

Load regulation very good: ±25μV/mA max, and the load impedance is high and constant.

This design drives two DACs, so the lack of a kelvin connections is less of an issue.

The [DAC boards](./pitch-dac.md) provide their own force/sense buffers to drive the voltage reference inputs on each DAC.

## Line regulation

Good at 4 (typ) to 12 max μV/V for supply voltage between 7.2V and 10V.

So a [well-regulated supply](./Power.md) is helpful.

### Secondary regulation

Use same regulated +9.5V supply as pitch DACs. Off-board, to protect from thermal effects.

## Noise

**Spectral noise** is 3 μV p-p 0.1-10Hz
**Flicker noise** is 3.5 μV RMS (typ) at 10Hz-1kHz.

These specs are not directly comparable to MAX6226_50 due to different units and bandwidths.

## Buffering

A non-auto-zero, e-trim op-amp such as **OPA2192** is suitable, and input bias current is even lower.

> For the reference buffers, the AD8676 dual amplifier is recommended, based on its low noise, low offset error, low offset error drift, and low input bias currents.
>
> Egan, M. _The 20-Bit DAC Is the Easiest Part of a 1-ppm-Accurate Precision Voltage Source_

 <table>
 <tr><td></td><th>noise</th><th>offset error</th><th>offset error drift</th><th>input bias current</th></tr>
 <tr><th>AD8676B</th><td>125nV RMS</td><td>±12/50 µV</td><td>±0.2/±0.6 μV/°C</td><td>±4.5nA</td></tr>
 <tr><th>OPA2186</th><td>0.1μV (100nV) p-p</td><td>±1/±10 µV</td><td>±0.01/±0.04 μV/°C</td><td>±4.8nA</td></tr>
 <tr><th>OPA2192</th><td>4 μV p-p</td><td>±8/±50 μV</td><td>±0.1/±0.5 μV/°C</td><td><b>±5/±20 pA</b></td></tr>
 </table>

So the OPA2186 is comparable on some criteria and much superior on offset and offset drift. OPA219 worse on offsets (still very good though) and better on input bias current.

> The input bias current specification of the reference buffers is important, as excessive bias currents will degrade the dc linearity. The degradation of integral nonlinearity, in ppm, as a function of input bias current, is typically:
>
> Extra INL error = (0.2 * Ibias) / (Vrefp - Vrefn)^2
>
> where IBIAS is in nA; VREFP and VREFN are in volts. For example, with a ±10-V reference input span, an input bias current of 100 nA will increase the INL by 0.05 ppm.
>
> Egan, M. _The 20-Bit DAC Is the Easiest Part of a 1-ppm-Accurate Precision Voltage Source_

Maybe try a non-autozero non-chopper op-amp. **OPA2192** looks suitable: Vos ±8μV typ, ±50μV max, drift ±0.1 typ, ±0.5 max μV/°C.

See figs 32 and 33 in the datasheet:

![fig32](./img/opa2192-fig32.png) ![fig33](./img/opa2192-fig33.png)

See Figure 69. Precision Reference Buffer in the datasheet (driving a 10μF capacitive load with great stability).

Pair of close-tolerance low thermal drift resistors for the VrefN op-amp.
Use of Vishay matched-pair resistors [suggested on EEVBlog](https://www.eevblog.com/forum/metrology/max6226-vref-and-ground-planes/msg4725761/#msg4725761) such as [these](https://www.mouser.com/ProductDetail/Vishay-Thin-Film/MPM2002QT3?qs=KOdD7VNvzR83jUiiLOdaaQ%3D%3D) 10k/10k 0.01% ratio, 25ppm/C.
Is it feasible to make a board that accepts either?

Decoupling caps.


## Schematic

not yet

## Board design

Power input: 4 wires +9.5, 0V, 0V, -9.5.

1μF and 100nF input caps, 1μF (or less; that is a maximum) and 100nF output caps, all C0G ceramic. Later went for X7R on the 1μF for cost and footprint reasons.

Voltage output: pair of 4-pin headers

- +5 +5 0V 0V
- -5 -5 0V 0V

Pin 8 is (per spec sheet) clear of copper, mask (no mechanical connection).

Full bottom ground plane; top ground connections with vias by caps and GND connection, runs under device 

Pin 1 of MAX6226 is indicated by a circular mark and is aligned to the top-left corner on this board (indicated by small dot on silkscreen).

Pin 1 of OPA2186 is on the side with the bevel, and is aligned closest to JP2 on this board.

![soic-8](./img/soic-8.png)

## BOM

### Voltage reference

(1) [LT1236AILS8-5#PBF ](https://www.mouser.com/ProductDetail/Analog-Devices/LT1236AILS8-5PBF?qs=ytflclh7QUXCvMb52%2F4eDQ%3D%3D) Ceramic LCC $11.77/1 = **$11.77** get 3 to 5
IC1

### Low Vos low Ibias precision op-amp (dual)

(1) TI [OPA2192IDR](https://www.mouser.com/ProductDetail/Texas-Instruments/OPA2192IDR?) SOIC-8 $4.63/10 = **$46.30**
U1

### Decoupling caps

(4) Kemet C1206C104K3GEC7210 25V 100nF C0G 1206 ceramics $0.051/100 = **$5.10 GOT**

### Bulk ceramic caps

(2) Kemet C1206X105K3RACTU  25V 1μF X7R 10% 1206 ceramic $0.839/10 = **$8.39 GOT**

### Low-ppm close tolerance resistors

(2) Susumu RG2012V-182-P-T1 1.8k 0.02% 5ppm 0805 $2.30/10 = **$23.00** 6 ordered 8 Dec 2023

### PCB

### Pin connectors

Right-angle 0.1" connectors
(1) Adafruit Break-away 0.1" 36-pin strip right-angle male header (10 pack) **$5.95**
https://www.adafruit.com/product/1540 out of stock

TE 9-103329-0 40-position **$2.31/1** **GOT 3**

Double ended connectors
Adafruit Extra-long break-away 0.1" 16-pin strip male header (5 pieces) **$3.00 GOT**
https://www.adafruit.com/product/400

## Work plan

- [ ] Design v0.1 Vref board with dual op-amp
- [ ] Fab v0.1 board
- [x] Check existing component inventory
- [ ] order Vref, op-amp (same as pitch DAC uses)
- [ ] Build, test
- [ ] Burn-in 
- [ ] Do stability analysis for innie OPA2192 with 2 R and 1 C
- [ ] Design and fab temporary [power board](./Power.md) to test for ripple, noise, need for isolation resistors

## Test results - positive reference

### Initial power-up

### Burn-in

### RC filtering

## Test results - negative reference
