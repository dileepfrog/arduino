#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#include <Encoder.h>
#include "RotaryEncoderWithButton.h"

#define COLOR_ORDER GRB
#define CHIPSET OCTOWS2811

#define BPM       118
#define DIMMEST   5
#define BRIGHTEST 256

#define INITIAL_BRIGHTNESS 64

// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;
#define NUM_MATRIX_LEDS (kMatrixWidth * kMatrixHeight)
#define NUM_STRIP_LEDS 144
#define NUM_STRIPS 2 // Total number of parallel strips including matrix
// The matrix has the highest LED count so this becomes the de facto led count
// per strip for OctoWS2811
//#define NUM_LEDS NUM_MATRIX_LEDS*(NUM_STRIPS+1)
CRGB leds[NUM_MATRIX_LEDS];
//CRGB *leftArmLeds(leds+NUM_MATRIX_LEDS);
//CRGB *rightArmLeds(leftArmLeds+NUM_MATRIX_LEDS);
CRGB leftArmLeds[NUM_STRIP_LEDS];
CRGB rightArmLeds[NUM_STRIP_LEDS];

// Rotary encoder to select animation
// pinA, pinB, pinButton
RotaryEncoderWithButton encoder(11,10,5);
uint8_t animationIndex = 0; // Updated by readEncoderPosition
uint8_t numAnimations = 11;
bool heartModeEnabled = false;

void loop() {
  readEncoderPosition();
  readPotentiometerAndSetBrightness();

  Serial.println(animationIndex);

  if (animationIndex == 0) {
    Lavalamp1();
  }
  else if (animationIndex == 1) {
    Lavalamp2();
  }
  else if (animationIndex == 2) { // Red+blue wind
    Constrained1();
  }
  else if (animationIndex == 3) { 
    RelativeMotion1();
  }
  else if (animationIndex == 4) { // Rainbow color splashes good with heart
    Caleido1();
  }
  else if (animationIndex == 5) { // Nifty
    Caleido2();
  }
  else if (animationIndex == 6) {
    Caleido3();
  }
  else if (animationIndex == 7) { // Smooth
    Caleido4();
  }
  else if (animationIndex == 8) { // Slow, maybe needs dithering?
    Caleido5();
  }
  else if (animationIndex == 9) { // Trippy mandalas
    Caleido6();
  }
  else if (animationIndex == 10) { // Smooth trippy mandalas
    Caleido7();
  }
  pop_fade();
  FastLED.show();
}

void setup() {
  delay(2000);
  // FIXME OctoWS2811 does not appear to be working on external power - DMA pin issue?
  // The chest matrix has the highest LED count so this becomes the de facto 
  // led count per strip for OctoWS2811
  //FastLED.addLeds<CHIPSET, COLOR_ORDER>(leds, NUM_MATRIX_LEDS);
  FastLED.addLeds<WS2811, 2, COLOR_ORDER>(leds, NUM_MATRIX_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, 14, COLOR_ORDER>(leftArmLeds, NUM_STRIP_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<WS2811, 7, COLOR_ORDER>(rightArmLeds, NUM_STRIP_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( INITIAL_BRIGHTNESS );
  //Serial.begin(9600);
  pinMode(23, INPUT);
  encoder.begin();
  CLS();
}

