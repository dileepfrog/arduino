#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#include <Encoder.h>
#include "RotaryEncoderWithButton.h"

#define COLOR_ORDER GRB
#define CHIPSET OCTOWS2811

#define BPM       118
#define DIMMEST   10
#define BRIGHTEST 220

#define INITIAL_BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;
#define NUM_MATRIX_LEDS (kMatrixWidth * kMatrixHeight)
#define NUM_STRIP_LEDS 144
#define NUM_STRIPS 2 // Total number of parallel strips including matrix
// The matrix has the highest LED count so this becomes the de facto led count
// per strip for OctoWS2811
CRGB leds[NUM_MATRIX_LEDS*(NUM_STRIPS+1)];

// Rotary encoder to select animation
// pinA, pinB, pinButton
RotaryEncoderWithButton encoder(5,9,10);
uint8_t animationIndex = 0; // Updated by readEncoderPosition
uint8_t numAnimations = 22;
bool heartModeEnabled = false;

void loop() {
  readEncoderPosition();

  if (animationIndex == 0) {
    pinWheelLoop();
    FastLED.show();
  }
  else if (animationIndex == 1) {
    Lavalamp1();
    FastLED.show();
  }
  else if (animationIndex == 2) {
    Lavalamp2();
    FastLED.show();
  }
  else if (animationIndex == 3) {
    Constrained1();
    FastLED.show();
  }
  else if (animationIndex == 4) {
    RelativeMotion1();
    FastLED.show();
  }
  else if (animationIndex == 5) {
    Water();
    FastLED.show();
  }
  else if (animationIndex == 6) { // Awesome
    TripleMotion();
    FastLED.show();
  }
  else if (animationIndex == 7) {
    CrossNoise();
    FastLED.show();
  }
  else if (animationIndex == 8) {
    CrossNoise2();
    FastLED.show();
  }
  else if (animationIndex == 9) {
    Caleido1();
    FastLED.show();
  }
  else if (animationIndex == 10) {
    Caleido2();
    FastLED.show();
  }
  else if (animationIndex == 11) {
    Caleido3();
    FastLED.show();
  }
  else if (animationIndex == 12) {
    Caleido4();
    FastLED.show();
  }
  else if (animationIndex == 13) {
    Caleido5();
    FastLED.show();
  }
  else if (animationIndex == 14) {
    Caleido6();
    FastLED.show();
  }
  else if (animationIndex == 15) {
    Caleido7();
    FastLED.show();
  }
}

void setup() {
  // The chest matrix has the highest LED count so this becomes the de facto 
  // led count per strip for OctoWS2811
  FastLED.addLeds<CHIPSET, COLOR_ORDER>(leds, NUM_MATRIX_LEDS);
  FastLED.setBrightness( INITIAL_BRIGHTNESS );
  Serial.begin(9600);
  encoder.begin();
}

