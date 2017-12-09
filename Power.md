# Power supply

## Eurorack power

Usual (bad) Eurorack power over ribbon cable with +12V, 0V, -12V and optional +5V, Gate and CV (latter two not used here). Module assumes each power rail is poorly regulated, with up to 0.5V droop likely.

## Digital and Analog 0V

Module uses two ribbon cables. 0V on one is treated as digital ground, and a +5.5V supply for DACs is derived from +12V. 0V on the other is treated as analog ground. This means the connection between them occurs at the PSU busboard. This should reduce digital return currents on analog ground.

## DAC power

Pitch DACs, Performance DACs, and the Global DACs all run at 5V5 so there is headroom above the 5V references. High voltage side of the logic level converters use 5V5 also.

## Microcontroller power

Both microcontrollers (Teensy 3.6 main board and Teensy LC display/oven controller) are 3V3 devices with onboard 3V3 regulators and a Vin pin which accepts 3V6 to 6V0 input. Two options:

- Use Eurorack 5V supply (reduces asymmetric loading on +12V rail)
- Use the 5V5 DAC supply (as it is there anyway and not all Euroracks have the 5V rail)

Board should support either configuration, select with jumper.

## Analog power

Direct from Eurorack, perhaps with Schotsky diodes for reverse power protection. Usual electrolytic caps onboard, and small ceramics by each op-amp package.

## Voltage reference power

Derived from Eurorack +12V and -12V using a pair of tiny (SOT-23), low dropout regulators, this provides a low current but well-regulated +11.5V -11.5V supply for the voltage reference. AD588 draws 6 (typ) 10 (max) mA.

## Oven heater

To be determined. Low current, but needs investigation.

## Gate

To be determined. Options for (about) 5V, 8V gates useful. Exact level not critical.
