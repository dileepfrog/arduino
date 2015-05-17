## Summary

This code is designed to run on a Teensy 3.1 with OctoWS2811 adapter board. There should be 3 Neopixel strips connected via the first RJ45 jack:
  - 1 16x16 matrix mounted on the chest
  - 2 144 1m strips running down each arm

Pin allocation:
  -  1 GND
  -  2 LED Strip #1
  -  3 (Reserved OctoWS2811)
  -  4 (Reserved OctoWS2811)
  -  5 Rotary pin A
  -  6 (Reserved OctoWS2811)
  -  7 LED Strip #3
  -  8 (Reserved OctoWS2811)
  -  9 Rotary pin B
  - 10 Rotary button
  - 11 AVAILABLE
  - 12 (Reserved OctoWS2811)
  - 13 AVAILABLE
  - 14 LED Strip #2
  - 15 (Reserved OctoWS2811)
  - 16 (Reserved OctoWS2811)
  - 17 AVAILABLE
  - 18 AVAILABLE
  - 19 AVAILABLE
  - 20 (Reserved OctoWS2811)
  - 21 (Reserved OctoWS2811)
  - 22 AVAILABLE
  - 23 AVAILABLE

## Components

Teensy 3.1

  https://www.pjrc.com/teensy/teensy31.html
  
  
OctoWS2811 Adapter
  https://www.pjrc.com/store/octo28_adaptor.html
  https://www.pjrc.com/teensy/td_libs_OctoWS2811.html
Rotary quadrature encoder + momentary switch with knob
  https://www.adafruit.com/product/377
  http://www.adafruit.com/datasheets/pec11.pdf
  http://www.adafruit.com/datasheets/EPD-200732.pdf


