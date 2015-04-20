#define USE_OCTOWS2811
#include<OctoWS2811.h>
#include<FastLED.h>

#define LED_PIN  2

#define COLOR_ORDER GRB
#define CHIPSET     OCTOWS2811

#define BPM       118
#define DIMMEST   10
#define BRIGHTEST 40

#define BRIGHTNESS 64

// Helper functions for an two-dimensional XY matrix of pixels.
// Simple 2-D demo code is included as well.
//
//     XY(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             No error checking is performed on the ranges of x and y.
//
//     XYsafe(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             Error checking IS performed on the ranges of x and y, and an
//             index of "-1" is returned.  Special instructions below
//             explain how to use this without having to do your own error
//             checking every time you use this function.  
//             This is a slightly more advanced technique, and 
//             it REQUIRES SPECIAL ADDITIONAL setup, described below.


// Params for width and height
const uint8_t kMatrixWidth = 16;
const uint8_t kMatrixHeight = 16;

// This function will return the right 'led index number' for 
// a given set of X and Y coordinates on your matrix.  
// IT DOES NOT CHECK THE COORDINATE BOUNDARIES.  
// That's up to you.  Don't pass it bogus values.
//
// Use the "XY" function like this:
//
//    for( uint8_t x = 0; x < kMatrixWidth; x++) {
//      for( uint8_t y = 0; y < kMatrixHeight; y++) {
//      
//        // Here's the x, y to 'led index' in action: 
//        leds[ XY( x, y) ] = CHSV( random8(), 255, 255);
//      
//      }
//    }
//
//
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if(x & 0x01) {
    // Odd rows run backwards
    uint8_t reverseY = (kMatrixWidth - 1) - y;
    i = (x * kMatrixWidth) + reverseY;
  } else {
    // Even rows run forwards
    i = (x * kMatrixWidth) + y;
  }
  
  return i;
}


// Once you've gotten the basics working (AND NOT UNTIL THEN!)
// here's a helpful technique that can be tricky to set up, but 
// then helps you avoid the needs for sprinkling array-bound-checking
// throughout your code.
//
// It requires a careful attention to get it set up correctly, but
// can potentially make your code smaller and faster.
//
// Suppose you have an 8 x 5 matrix of 40 LEDs.  Normally, you'd
// delcare your leds array like this:
//    CRGB leds[40];
// But instead of that, declare an LED buffer with one extra pixel in
// it, "leds_plus_safety_pixel".  Then declare "leds" as a pointer to
// that array, but starting with the 2nd element (id=1) of that array: 
//    CRGB leds_with_safety_pixel[41];
//    const CRGB* leds( leds_plus_safety_pixel + 1);
// Then you use the "leds" array as you normally would.
// Now "leds[0..N]" are aliases for "leds_plus_safety_pixel[1..(N+1)]",
// AND leds[-1] is now a legitimate and safe alias for leds_plus_safety_pixel[0].
// leds_plus_safety_pixel[0] aka leds[-1] is now your "safety pixel".
//
// Now instead of using the XY function above, use the one below, "XYsafe".
//
// If the X and Y values are 'in bounds', this function will return an index
// into the visible led array, same as "XY" does.
// HOWEVER -- and this is the trick -- if the X or Y values
// are out of bounds, this function will return an index of -1.
// And since leds[-1] is actually just an alias for leds_plus_safety_pixel[0],
// it's a totally safe and legal place to access.  And since the 'safety pixel'
// falls 'outside' the visible part of the LED array, anything you write 
// there is hidden from view automatically.
// Thus, this line of code is totally safe, regardless of the actual size of
// your matrix:
//    leds[ XYsafe( random8(), random8() ) ] = CHSV( random8(), 255, 255);
//
// The only catch here is that while this makes it safe to read from and
// write to 'any pixel', there's really only ONE 'safety pixel'.  No matter
// what out-of-bounds coordinates you write to, you'll really be writing to
// that one safety pixel.  And if you try to READ from the safety pixel,
// you'll read whatever was written there last, reglardless of what coordinates
// were supplied.

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);

// This mask is used to determine if a requested coordinate
// falls into our virtual canvas bounds
prog_uint8_t const heart_mask[][16] PROGMEM = {
  {0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0}
};

CRGB CRGBInBounds( CRGB color, uint8_t x, uint8_t y )
{
  if( x >= kMatrixWidth) return CRGB(0, 0, 0);
  if( y >= kMatrixHeight) return CRGB(0, 0, 0);
  if(heart_mask[y][x] == 0) return CRGB(0, 0, 0);
  
  return color;
}

// Demo that USES "XY" follows code below

void loop()
{
  for( byte i = 0; i < 15; i++) {
    for( byte y = 0; y < kMatrixHeight; y++) {
      for( byte x = 0; x < kMatrixWidth; x++) {
        /*uint8_t original_red = pgm_read_byte(&(red_frames[i][y*kMatrixWidth+x]));
        uint8_t original_green = pgm_read_byte(&(green_frames[i][y*kMatrixWidth+x]));
        uint8_t original_blue = pgm_read_byte(&(blue_frames[i][y*kMatrixWidth+x]));
        uint8_t red = cubicwave8(beatsin8( BPM, (original_red-100)%255, (original_red+100)%255));
        uint8_t green = cubicwave8(beatsin8( BPM, (original_green-100)%255, (original_green+100)%255));
        uint8_t blue = cubicwave8(beatsin8( BPM, (original_blue-100)%255, (original_blue+100)%255));
        CRGB color = CRGB(red, green, blue);*/
        //leds[ XYsafe(x, y)].setRGB(red, green, blue);
        //CRGB color = pinwheelAnimationColorAtFramePosition(i, x, y);
        CRGB color = heartunnelAnimationColorAtFramePosition(i, x, y);
        leds[ XY(x, y)] = color;
      }
    }

    FastLED.show();
    FastLED.delay(25);
  }
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
  FastLED.addLeds<OCTOWS2811, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

