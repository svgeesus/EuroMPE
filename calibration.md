# Calibration

There are two types of calibration:

## Initial setup trimming

Ensure trimmers are on front panel so unit can be trimmed while in the rack, powered and warm. Firstly setting precise 5V output from reference; then setting offset (by adjusting the 2V offset reference) and gain on output of each pitch DAC.

The 14bit performance outputs are untrimmed, 0-5V within 5-10mV is fine.

Vref needs extensive trimming and measurement (for ageing) outside case anyway. Provide a buffered +5V jack on front panel, offset trimmer near it.

Needs outputs at 0V (before offset, so provide test point? or trim outside case as a first check), -2V (= 0V after offset, for offset trim), -4.8 = -2.8V, +4.8 = +6.8V (to avoid DAC extremes where zero and FS offset complicate gain setting, for gain trim).

Global offset also needs an output, for calibration, plus handy to expose for other patching.

## Per-voice oscillator calibration

Avoid non-linear calibration curves. These are fine for pulling poorly-tracking oscillators into tune but then the same errors affect the other modules using pitch (filters etc). Instead go for high linearity, and use with well tracking oscillators.

Output the same voltage (frequency calculated for the given CV, e.g. for 440Hz) on all pitch channels. Also a sine output from one of the T3.6 DACs, e.g. 440Hz, for beat frequency tuning). Use FreqMeasure library to provide a frequency input, showing the frequency on the LCD screen. This allows each oscilator to be tuned the same. These pins support measuring frequency:

- #define FTM0_CH0_PIN 22
- #define FTM0_CH1_PIN 23
- #define FTM0_CH2_PIN  9
- #define FTM0_CH3_PIN 10
- #define FTM0_CH4_PIN  6
- #define FTM0_CH5_PIN 20
- #define FTM0_CH6_PIN 21
- #define FTM0_CH7_PIN  5