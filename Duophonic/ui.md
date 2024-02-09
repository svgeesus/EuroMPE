# User Interface

Goal is that normal operation does not involve interacting with screen or buttons.

They are instead used for:

- Calibration and Tuning
  - Manual (rough) tune via oscillator coarse and fine knobs. OV CV. Tuning feedback.
  - Starting auto-tune
  - Setting MIDI Note corresponding to 0V
  - Setting tuning of A4 (440Hz default)
- Configuration
  - MPE/nonMPE mode?
  - MIDI channel selection (if non-MPE)
  - Global CC/NRPN configuration

## Menu (mockup)

Three screens: Tune, Config, Run. Activated by the three buttons. _might not need "back"_. OK selects current option, goes back to previous screen. If Back not needed, split config into A-H and MPE config

- **Tune**
  - Manual
    - Gives text instructions, feedback via LEDs and onscreen cents sharp/flat per channel
  - Auto (assumes manual is done)
    - Gives progress feedback
  - Set 0V
    - (list of MIDI note names, ike C3 A#5 etc)
  - Set A4
    - 440Hz, encoder to increase/decrease

- **Config**
  - Outputs A-H _use same 4 by 2 layout as the actual jacks_
    - A
      - (list of CC names and associated numbers)
    - B
    - C
    - D
    - E
    - F
    - G
    - H
  - MPE mode
    - On
    - Off
  - MIDI channel (ignored in MPE mode)
    - Omni
    - 1
    - 2
    _etc_
    - 16

The list of CC names should also include RPNs? How to include NRPN?

Last state should persist over power cycling.

## Screen

Monochrome OLED, 128x64 pixels 1.3 inch diagonal.

- [Many fonts for 128x32 displays](https://github.com/datacute/TinyOLED-Fonts/tree/master)
- [Adafruit TTF font converter](https://github.com/adafruit/Adafruit-GFX-Library/tree/master/fontconvert)

## Encoder

Rotary encoder for menu scrolling, with integrated switch for "accept".

## Buttons

1. Back/Escape
2. Tune
3. Config
4. Run (normal operation)

[Thonk C&K Tactile D6R](https://www.thonk.co.uk/shop/radio-music-switch/)

## Development & Testing plan

- [ ] Investigate [GEM](https://github.com/Spirik/GEM/wiki) menu system
