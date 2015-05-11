#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#include <Encoder.h>
#include "RotaryEncoderWithButton.h"

#define COLOR_ORDER GRB
#define CHIPSET OCTOWS2811

#define BPM       118
#define DIMMEST   10
#define BRIGHTEST 40

#define BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

// Rotary encoder to select animation
// pinA, pinB, pinButton
RotaryEncoderWithButton encoder(5,9,10);
uint8_t animationIndex = 0; // Updated by readEncoderPosition
uint8_t numAnimations = 5;
bool heartModeEnabled = false;

void loop() {
  readEncoderPosition();

  if (animationIndex == 0) {
    pinWheelLoop();
    FastLED.show();
  }
  else if (animationIndex == 1) {
    MirroredNoise();
    FastLED.show();
  }
  else if (animationIndex == 2) {
    RedClouds();
    FastLED.show();
  }
  else if (animationIndex == 3) {
    Lavalamp1();
    FastLED.show();
  }
  else if (animationIndex == 4) {
    Lavalamp2();
    FastLED.show();
  }
}

void setup() {
  FastLED.addLeds<CHIPSET, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);
  encoder.begin();
}

