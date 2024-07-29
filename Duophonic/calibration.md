# Calibration

There are two types of calibration:

## Initial setup trimming

Pitch Vref has no pitch trimmers; calibration of Pitch DAC output to account for Vref inaccuracy, DAC zero and DAC max errors is done in software.

The 14bit performance outputs are untrimmed, 0-5V within 5-10mV is fine.

Vref needs extensive measurement (for ageing) outside case anyway.

Might be better to allow re-calibration after pitch Vref drift though (annual maintenance).

## Per-voice oscillator calibration (tuning)

Avoid non-linear calibration curves. These are fine for pulling poorly-tracking oscillators into tune but then the same errors affect the other modules using pitch (filters etc). Instead go for high linearity, and use with well tracking oscillators.

Two stages: analog coarse tune (manual oscilator tuning to a fixed pitch) then digital fine tune and linearization (measuring frequency per-oscillator at a range of voltages, for linearity and for oscillator range).

Output the same voltage (frequency calculated for the given CV, e.g. for 440Hz) on both  pitch channels. Use FreqMeasure library to provide a frequency input, showing the frequency on the LCD screen. This allows each oscilator to be tuned the same. Use the LEDs to visually inndicate high or low (y color) and closeness to exact tuning (slower pulsing means closed to exact).

Once in tune, run a frequency sweep measuring at least 30 steps and calculating a correction factor to account for slope inaccuracy (not exactly 1V/oct) and high frequency droop. Optionally, log the temperature at which tuning was done.

[FreqMeasureMulti](https://github.com/PaulStoffregen/FreqMeasureMulti) can measure multiple inputs simultaneously, providing the option to calibrate both voice channels at the same time. It is best for lower frequencies, 0.1Hz to 1kHz. Available pins 0-9,22-25,28,29,33,36,37,42-47, 48-50(dups),51, 52-53 (dups), 54.

[FreqCountMany](https://github.com/PaulStoffregen/FreqCountMany) also measures multiple inputs,best for 1kHz and up. Available pins 6, 9, 10, 11, 12, 13, 14, 15, 18, 19.

Current [panel](./Panel.md) has per-voice tuning inputs.

> calibrating only octaves it also runs foul of the same quirk in some oscillators where they go out of tune just a semitone or two before 0v – with possibly even worse results, as you would now have a bad data point for 0v.

> Not all sound sources can take a full negative to positive swing pitch control voltages. Many digital modules in particular only listen to positive voltages, and may be go of tune as you get close to 0 volts no matter what you do. Also, I’ve seen some analog modules which track differently above 0 volts than they do below 0 volts.

> [Calibr8or firmware for Ornament & Crime](https://learningmodular.com/calibr8or/)

### MIDI Tune Request

Respond to F6 Tune Request by tuning both channels. Will only work if the oscillator outputs are looked back to the tune inputs.

### Tune-in conditioning circuitry

Level from oscillator should be plenty high, no need for amplification. May need some low-pass filtering to get rid of above-audio signals that could cause false triggering?

Main requirement is to hard clip to a 0..3V3 signal with a rail-to-rail op-amp so it can be fed to the Teensy. Experiment on breadboard.

> The FlexPWM timer used by FreqMeasureMulti requires very fast edges. Your signal looks like it has rather slow rise/fall times, and that will not work with FreqMeasureMulti. Can you add a schmitt trigger on your input?
> [PJRC](https://forum.pjrc.com/index.php?threads/teensy-4-1freqmeasuremulti-library.75304/#post-345574)

OPA2365 ($3.17/1) has specified overload recovery time < 0.1μs and RRIO. Use 2k current-limiting input resistor (<10mA) then rely on protection diodes.

[Simple and fast bipolar-supply precision clamp with RRIO](https://www.eevblog.com/forum/projects/limiting-op-amp-output/msg441564/#msg441564) and [even simpler, unipolar-supply](https://www.eevblog.com/forum/projects/limiting-op-amp-output/msg732673/#msg732673)

Should this have a Schmidt-trigger inverter or something like that to ensure even low-level signals are square-wave like? Build and test the simple op-amp clamp and check results on scope.

### Calibrating the calibrator

Keysight 34465A as measurement device.

Leo Bodnar [LBE-1420 GPS locked clock source](https://www.leobodnar.com/shop/index.php?main_page=product_info&cPath=107&products_id=393&zenid=fef464de4ee6cf4f24df5e55bba72372) as reference.
