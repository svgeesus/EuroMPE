# Voltage ref 

Post-COVID supply chain has much less availability for good voltage references.

MAX6226_50 (Ceramic LCC) is available at MOQ 1, 
has great stability due to the hermetic ceramic package,
excellent temperature stability

![pinout](./img/MAX6226-pinout.png)

 - [EEV metrology thread on MAX6226](https://www.eevblog.com/forum/metrology/max6226-voltage-reference/msg3569644/#msg3569644)

Vref board needs to provide both +5V and -5V to drive the pitch DACs.

## Absolute initial accuracy

Good initial accuracy ±0.02% (±1mV). But not trimmable, would need digital-domain trim.


## Temperature

Very good at 1ppm/C typ, 3ppm/C max (15μV max over 0 to 70C)


## Thermal Hysteresis

Very good at 5ppm.

## Long term drift

"The data sheets of many references specify long-term drift—typically about 25 ppm/1000 hr. This error is proportional to the square root of elapsed time, so 25 ppm/1000 hr ≈ 75 ppm/year. The actual rate is likely (but not certain) to be somewhat better than this as the ageing rate often diminishes after the first few thousand hours. So, again, we have a figure around 14 bits."
[Choosing Voltage References](http://www.analog.com/en/analog-dialogue/raqs/raq-issue-114.html)

11ppm/1k hr (55μV), worst on new device and settling over time.

## Current / Load

Load regulation good: ±50μV/mA

As this design drives two DACs, the kelvin connections only take us to the Vref and Vrefn buffers.

## Line regulation

Okay at 9 (typ) to 80 max μV/V.

### Secondary regulation

Use same regulated +9.5V supply as pitch DACs.

LT1761 LDO positive reg, 300mV dropout, 100ma SOT-23

## Board design

> Note that although the MAX6226’s ceramic package
prevents the output voltage from being affected by
mechanical stresses due to humid environments, the
PC board may be affected by humidity, and may in turn
cause mechanical stress to the MAX6226 mounted on it.
Take extra care with PC board layout. Power and ground
planes should be avoided under the voltage reference IC
for best stability. Closely follow the layout guidelines in the
Output Bypassing and Output Force and Sense.

> Pin 8 must be clear of any mechanical and electrical
contact. Neither copper nor solder/paste mask must
be placed underneath its land pattern. The absence of
the mechanical contact will eliminate the possibility of
paddle induced stress to the die. The absence of electrical
contact will eliminate the possibility of any ground current
redistribution.


