# Calibration

There are two types of calibration:

## Initial setup trimming

Pitch Vref has no pitch trimmers; calibration of Pitch DAC output to account for Vref inaccuracy, DAC zero and DAC max errors is done in software.

The 14bit performance outputs are untrimmed, 0-5V within 5-10mV is fine.

Vref needs extensive measurement (for ageing) outside case anyway.

Might be better to allow re-calibration after pitch Vref drift though (annual maintenance).

## Per-voice oscillator calibration (tuning)

Avoid non-linear calibration curves. These are fine for pulling poorly-tracking oscillators into tune but then the same errors affect the other modules using pitch (filters etc). Instead go for high linearity, and use with well tracking oscillators.

Two stages: coarse tune (manual oscilator tuning to a fixed pitch) then fine tune (measuring frequency per-oscillator at a range of voltages, for linearity and for oscillator range).

Output the same voltage (frequency calculated for the given CV, e.g. for 440Hz) on both  pitch channels. Use FreqMeasure library to provide a frequency input, showing the frequency on the LCD screen. This allows each oscilator to be tuned the same.

[FreqMeasureMulti](https://github.com/PaulStoffregen/FreqMeasureMulti) can measure multiple inputs simultaneously, providing the option to calibrate both voice channels at the same time. Would imply moving the input jacks to the main per-voice area of the panel, not off to the side. Experiment and see if worthwhile before comitting to a panel layout.