#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

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
uint8_t animationIndex = 0;
uint8_t numAnimations = 2;
Encoder encoder(5, 9);
long currentEncoderPosition  = -999;

void loop()
{
  long newEncoderPosition = encoder.read();
  if (newEncoderPosition != currentEncoderPosition) {
    if (newEncoderPosition > currentEncoderPosition)
      animationIndex = (animationIndex+1)%numAnimations; 
    else
      animationIndex = (animationIndex-1)%numAnimations; 
      
    currentEncoderPosition = newEncoderPosition;
    Serial.println(currentEncoderPosition);
  }

  if (animationIndex == 0)
    pinWheelLoop();
  else
    heartLoop();
}

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;      
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}

void setup() {
  FastLED.addLeds<CHIPSET, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  Serial.begin(9600);
}

