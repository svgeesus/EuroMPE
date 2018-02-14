# Voltage ref with divided-down LM399

## Intro

LM399AH has ultra-high long term stability and drift over temperature, as it has an on-chip thermal stabilisation. It uses a hermetically sealed metal can so is unaffected by relative humidity, and the long leads protect against board stress. It is used in mid-range 6.5 digit multimeters as a voltage reference, for this reason (the higher-end 7.5 digit and above meters use the more stable, less noisy and more expensive LTZ1000A).

On the downside, the initial accuracy is _really terrible_; each device needs to be burned in for hundreds of hours and then measured to see what voltage it produces.

## Absolute initial accuracy

6.95V (typ) with a huge variation - 6.75 to 7.3V !! -200 to +350mA !! Absolutely requires measurement of initial value, monitoring of value over time for burning until stable to desired precision, then trimming in the output conditioning stage to desired output voltage.

## Temperature

Specified as 0.3 (typ) 1.0 (max) ppm/C over 0 to 70C ambient (A grade). Major effect on the tempco of the eventual 5V reference is the tempco of the resistive divider.

Has its own heater and thermal regulation circuit, so there is a stable temperature inside the can.
It works well with a large voltage difference, and the use of negative as well as positive supplies also helps. Limit is 40V. Agilent 34401 uses +15 and -15 for the heater circuit.

Use +12 and -12 direct from Eurorack power. Also avoids dumping current into the 0V common reference.

Temperature is not adjustable, chip runs at a die temperature of 95C and junction temperature 90C. Besides the supplied plastic insulator cap, additional foam insulation above and below the LM399 means the heater circuit does less work, improving stability.

Heater supply current stated to be 8.5mA (typ) 15mA (max) on 30V heater. For the same power, 10.6 mA (typ) 18.8 mA (max) on 24V. Note huge inrush current of 140mA (typ) 200mA (max) (on 30V, so higher on 24V). Can use inrush currrent limiting resistor (max 200R, see datasheet Fig G09) to damp this, with slower time to thermal stabilisation. However the normal time is 3s so plenty of leeway there. 180R limits inrush current to 130mA at 24V.

## Current / Load

Load regulation not directly specified, as this is a shunt regulator. In this project the load will be a resistive divider trimmed to give exactly 5.000V after the output buffer op-amp. This constant load (influence of the input impedance of the kelvin amps used to sent the voltage reference to the DACs is minimal) means load regulation term can be ignored.

## Long term drift

Higher at first, reduces with square root of 1000 hours in use. Needs 100 hours burn-in before initial measurement.

Specified as 8ppm/√kHr at ambient 22 to 28C, 1kHr, 1mA zener current ±0.1%. So 4ppm at 4kHr, 2ppm at 16kHr (almost 2 years). Note influence of zener current stability, hence the popularity of bootstrapped references.

- [LM399 10 channel ageing/monitoring](http://www.ianjohnston.com/index.php/onlineshop/10-ch-lm399-testing-detail)

"Vz changes by 1uV for every deviation of 1uA of zener current from the nominal 1mA"

## Line regulation

Not directly specified. heater circuit seems to tolerate a large voltage range. Zener circuit is current driven, so high stability current is needed (bootstrapped from output voltage, not power rail).

## Output conditioning

### Buffer op-amp

Chopper-stabilised zero-drift op-amps seem to be favoured for stability.

LTC2050HV (used on the Ian Johnston adjustable reference) has almost zero Vos and drift, while also having low switching noise.

Max PSU voltage is 11V though, so needs a separate derived PSU. Supply current is 0.8mA.

LTC2057 is up to 36V. LTC2057HS8#PBF
Compare carefully.

"Placing a capacitor across the feedback resistor reduces either form of clock feedthrough by limiting the bandwidth of the closed loop gain. "

100nF film cap for noise filtering.

Keep the 100nF decoupling cap very close to the positive power input.

The output voltage is bootstrapped round via a resistor to set the zener current to 1mA (range is 0.5 to 10mA, above 1mA has no benefit). A 200k pull-up resistor allows the circuit to start (there seems to be debate on whether this is needed; less so if the output buffer uses a single rail rather than split rails).

For 5V output, use a unity-gain non-inverting follower then a resistive divider load to get the exact 5V on the distribution board.

### Adjusting to 5.000V

Most circuits amp up the output to 10V; in this case needs low tempco resistor divider to 5V.


- [LM399 influence of resistors](http://www.eevblog.com/forum/metrology/influence-of-resistors-in-lm399-reference-circuit/msg1214075/#msg1214075)
- [xdevs LM399 ref](https://xdevs.com/article/kx-ref/#lm399opt)
- [xdevs LM399 calculator](https://xdevs.com/lmx99/)


