# Power supply

## Eurorack power

Usual (bad) Eurorack power over ribbon cable with +12V, 0V, -12V and optional +5V, Gate and CV (latter two not used here). Module assumes each power rail is poorly regulated, with up to 0.5V droop likely.

## Digital and Analog 0V

Module uses two ribbon cables. 0V on one is treated as digital ground, and +5V on that cable powers digital circuits. 
0V on the other is treated as analog ground, with +12 and -12 powering op-amps and secondary voltage regulators.  
This means the connection between digital and analog occurs at the PSU busboard. 
This should reduce digital return currents on analog ground. However, digital 0V to SPI DAC meets analog 0V unless isolator is used, and other connections between grounds will occur due to panel jacks and external patching.

## DAC power

Performance DACs, and the Global DAC  run at 5V5 so there is headroom above the internal 5V reference. High voltage side of the logic level converters for those DACs use 5V5 also **is that still true**. Derived from +12V.

Pitch DACs run at ±9.5V and the Vref for those uses ±9.5V as well. No logic level conversion needed, pitch DAC is 3V3 compatible.

Use LT1763 (SOIC-8, 500mA) adjustable regulators (500mA is ample) to produce 5V5 and 9V5, with 6μ8 C0G output cap. -9V5 from LT1964-BYP adjustable (SOT-23, 200mA still plenty).

## Microcontroller power

Teensy 4.1 main board is a 3V3 device with onboard 3V3 regulators and a Vin pin which accepts 3V6 to 6V0 input. Use Eurorack 5V supply (reduces asymmetric loading on +12V rail). Current draw can be 100mA, depends on clock speed.

## Analog power

Direct from Eurorack, perhaps with Schotsky diodes for reverse power protection. Usual electrolytic caps onboard, and small ceramics by each op-amp package.

Pitch DAC op-amps can run on ±9.5V to handle ±5V CV.

## Voltage reference power

Same as Pitch DAC power.

## Gate

To be determined. Options for (about) 5V, 8V gates useful. Exact level not critical.

## Analog power board

Eurorack power connector, usual 10μF smoothing caps (or greater)?
Top plane mostly gnd (for low impedance and also cooling) with traces for ±9V5;
bottom plane gnd (again for cooling), well stitched to top, with traces for ±12V.

### LT1763 for 9V5

10μF input cap, 1nF noise bypass cap, 6μ8F to 10μF output cap (see Fig 3. Stability and discussion of ceramic dielectrics: go for X7R).

_Minimum_ top resistor (R2) value is 2.6k. Try with 47k. First approximation:

9.5 = 1.22(1 + r2/r1)

9.5 = 1.22 + 1.22 × r2/r1

8.28 = 1.22 × r2/r1

r1 = (1.22 × r2)/8.28

r1 = (1.22 × 47000)/8.28

r1 = 6k98 which happily is an E96 value.

Second approximation:

Vout = 1.22(1 + 47000/6980) + 30E-9 ×  47000 = 9.436V. Second term is 1.41mV so ignore.

Current through r1 is 1.22/6980 = 170μA
Current through r2 is (9.5-1.22)/47000 = 170μA. 

Power = output current times in-out voltage differential, plus ground current times input voltage. Say 50mA × (12.5 - 9.5) + 2mA × 12.5 = 175mW.

From datasheet, table 2: use 100mm² top-side ground plus 25000mm² lower ground plane = 10 × 10mm top; no go bigger, 1000mm² = 32 ×² 32mm top. 50 × 500mm bottom seems unfeasibly large (19 inches!!). Going to need much better current draw estimates!

But assuming 74C/W, rise above ambient is just 13C.

![lt1763](./img/LT1763-adj.png)

### LT1763 for 5V5

Higher rise due to bigger voltage drop.

5.5 = 1.22 + 1.22 × r2/r1

4.28 = 1.22 × r2/r1

r1 = (1.22 × 47000)/4.28

r1 = 13k4, closest E96 is 13k3.

Vout = 1.22(1 + 47000/13300) + 30E-9 ×  47000 = 5.53V.

Power = 50mA × (12.5 - 5.5) + 2mA × 12.5 = 375mW.

At 74C/W, rise above ambient is 28C.

### LT1964-BYP for -9V5

10μF? input cap, 1nF noise bypass cap, 6μ8F to 10μF output cap

-9.5 = -1.22(1 + r2/r1)

-9.5 = -1.22 + 1.22 × r2/r1

-8.28 = -1.22 × r2/r1

r1 = (-1.22 × r2)/-8.28

r1 = (-1.22 × 47000)/-8.28

r1 = 6k98

Vout = -1.22(1 + 47000/6980) - 30E-9 ×  47000 = -9.436V.

Power = -50mA × (-12.5 - -9.5) + -2mA × -12.5 = 175mW.

At 135C/w, 23.5C above ambient.

![lt1964](./img/lt1964-adj.png)

## Work Plan

Measure current draw of perf DAC board (5V5 regulator needs to power 2) before ordering PSU parts, to be sure the SMD 5V5 will be suitable. Might need a 3-pin regulator.
