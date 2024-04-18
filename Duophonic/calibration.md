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

### Tune-in conditioning circuitry

Level from oscillator should be plenty high, no need for amplification. May need some low-pass filtering to get rid of above-audio signals that could cause false triggering?

Main requirement is to hard clip to a 0..3V3 signal with a rail-to-rail op-amp so it can be fed to the Teensy. Experiment on breadboard.

OPA2365 ($3.17/1) has specified overload recovery time < 0.1μs and RRIO. Use 2k current-limiting input resistor (<10mA) then rely on protection diodes.

[Simple and fast precision clamp with RRIO](https://www.eevblog.com/forum/projects/limiting-op-amp-output/msg441564/#msg441564) and [even simpler](https://www.eevblog.com/forum/projects/limiting-op-amp-output/msg732673/#msg732673)

### Calibrating the calibrator

Keysight 34465A as measurement device.

Leo Bodnar [LBE-1420 GPS locked clock source](https://www.leobodnar.com/shop/index.php?main_page=product_info&cPath=107&products_id=393&zenid=fef464de4ee6cf4f24df5e55bba72372) as reference.
