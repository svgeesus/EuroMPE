# Power supply

## Eurorack power

Usual (bad) Eurorack power over ribbon cable with +12V, 0V, -12V and optional +5V, Gate and CV (latter two not used here). Module assumes each power rail is poorly regulated, with up to 0.5V droop likely.

## Digital and Analog 0V

Module uses two ribbon cables. 0V on one is treated as digital ground, and +5V on that capble powers digital circuits. 0V on the other is treated as analog ground, with +12 and -12 powering op-amps and and a +5.5V supply for DACs is derived from +12V. This means the connection between them occurs at the PSU busboard. This should reduce digital return currents on analog ground.

## DAC power

Performance DACs, and the Global DAC  run at 5V5 so there is headroom above the internal 5V reference. High voltage side of the logic level converters for those DACs use 5V5 also.

Pitch DACs run at ±9.5V and the Vref for those can use the +9.5 rail as well. No logic level conversion needed.

Use LT1763 adjustable regulators (500mA is ample) to produce 5V5 and 9V5, with 6μ8 C0G output cap. -9V5 from LT1964 adjustable (200mA still plenty).

## Microcontroller power

Teensy 4.1 main board is a 3V3 device with onboard 3V3 regulators and a Vin pin which accepts 3V6 to 6V0 input. Use Eurorack 5V supply (reduces asymmetric loading on +12V rail)

## Analog power

Direct from Eurorack, perhaps with Schotsky diodes for reverse power protection. Usual electrolytic caps onboard, and small ceramics by each op-amp package.

## Voltage reference power

Depends on Vref chip used.

## Gate

To be determined. Options for (about) 5V, 8V gates useful. Exact level not critical.