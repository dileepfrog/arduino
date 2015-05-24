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

uint16_t XYsafe( uint8_t x, uint8_t y)
{
  if( x >= kMatrixWidth) return -1;
  if( y >= kMatrixHeight) return -1;
  return XY(x,y);
}

// This mask is used to determine if a requested coordinate
// falls into our virtual canvas bounds
static uint8_t const heart_mask[][16] = {
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

uint16_t heartXY( uint8_t x, uint8_t y)
{
  // Because the strips are only 144 pixels vs 256 for the matrix, the 145th
  // pixel on the strip exists in our led array but is invisible, making it
  // ideal to dump colors to that we don't want to draw (outside of the heart mask)
  uint16_t safetyPixel = NUM_MATRIX_LEDS + NUM_STRIP_LEDS + 1;
  if (heartModeEnabled) {
    if( x >= kMatrixWidth) return safetyPixel;
    if( y >= kMatrixHeight) return safetyPixel;
    if(heart_mask[y][x] == 0) return safetyPixel;
  }

  return XY(x,y);
}

// Apply the mask by passing all colors through this method which will
// return black for coordinates outside our mask
CRGB CRGBInHeart( CRGB color, uint8_t x, uint8_t y )
{
  if( x >= kMatrixWidth) return CRGB(0, 0, 0);
  if( y >= kMatrixHeight) return CRGB(0, 0, 0);
  if(heart_mask[y][x] == 0) return CRGB(0, 0, 0);
  
  return color;
}

// Black out the screen
void CLS()  
{
  // TODO Can optimize by skipping sparse elements in second two rows which
  // have less LEDs that the first row (chest matrix)
  for(int i = 0; i < NUM_MATRIX_LEDS*(NUM_STRIPS+1); i++) {
    leds[i] = 0;
  }
}

