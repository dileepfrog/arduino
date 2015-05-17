// a place to store the color palette
CRGBPalette16 currentPalette;

// can be used for palette rotation
// "colorshift"
byte colorshift;

// The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS 3

uint32_t x[NUM_LAYERS];
uint32_t y[NUM_LAYERS];
uint32_t z[NUM_LAYERS];
uint32_t scale_x[NUM_LAYERS];
uint32_t scale_y[NUM_LAYERS];

// used in FillNoise for central zooming
byte CentreX = (kMatrixWidth / 2) - 1;
byte CentreY = (kMatrixHeight / 2) - 1;

// a 3dimensional array used to store the calculated 
// values of the different noise planes
uint8_t noise[NUM_LAYERS][kMatrixWidth][kMatrixHeight];
uint8_t noisesmoothing;

// Fill the x/y array with 16-bit noise values 
void FillNoise(byte layer) {
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    uint32_t ioffset = scale_x[layer] * (i-CentreX);
    for(uint8_t j = 0; j < kMatrixHeight; j++) {
      uint32_t joffset = scale_y[layer] * (j-CentreY);
      byte data = inoise16(x[layer] + ioffset, y[layer] + joffset, z[layer]) >> 8;
      uint8_t olddata = noise[layer][i][j];
      uint8_t newdata = scale8( olddata, noisesmoothing ) + scale8( data, 256 - noisesmoothing );
      data = newdata;

      noise[layer][i][j] = data;
    }
  }
}

// overlay layers 0&1&2 for color, layer 2 is brightness

void MergeMethod1(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] )
        + ( noise[2][i][j] ) )
        / 3; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
} 


// overlay layers 0&1 for color, layer 2 is brightness

void MergeMethod2(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] ) )
        / 2; 

      // layer 2 gives the brightness  
      uint8_t   bri = (noise[2][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// overlay layers 0&1 for color, brightness is layer1

void MergeMethod3(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      // layer 0 and 2 interfere for the color
      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] ) )
        / 2; 

      // layer 1 gives the brightness  
      uint8_t   bri = noise[1][i][j];

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}


// overlay layers 0&1&2 for color, layer 0 is brightness

void MergeMethod4(byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = ( ( noise[0][i][j] )
        + ( noise[1][i][j] )
        + ( noise[2][i][j] ) )
        / 3; 

      uint8_t   bri = (noise[0][i][j]);

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[XY(i,j)] = pixel;
    }
  }
}

// draw the part between lower and upper limit of one layer

void ConstrainedMapping(byte layer, byte lower_limit, byte upper_limit, byte colorrepeat) {

  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t data =  noise[layer][i][j] ;

      if ( data >= lower_limit  && data <= upper_limit) {

        CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (data + colorshift), data );

        leds[XY(i,j)] = pixel;
      }
    }
  }
}

void PaletteRed() {
  currentPalette = CRGBPalette16( 
  CHSV( 0, 255, 255 ), 
  CHSV( 0, 255, 0   ), 
  CHSV( 0, 255, 0   ),
  CHSV( 0, 255, 255)); 
}

// All the caleidoscope functions work directly within the screenbuffer (leds array).
// Draw whatever you like in the area x(0-15) and y (0-15) and then copy it arround.

// rotates the first 16x16 quadrant 3 times onto a 32x32 (+90 degrees rotation for each one)

void Caleidoscope1() {
  for(int x = 0; x < kMatrixWidth / 2 ; x++) {
    for(int y = 0; y < kMatrixHeight / 2; y++) {
      leds[XY( kMatrixWidth - 1 - x, y )] = leds[XY( y, x )];    
      leds[XY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];    
      leds[XY( x, kMatrixHeight - 1 - y )] = leds[XY( y, x )];    
    }
  }
}


// mirror the first 16x16 quadrant 3 times onto a 32x32

void Caleidoscope2() {
  for(int x = 0; x < kMatrixWidth / 2 ; x++) {
    for(int y = 0; y < kMatrixHeight / 2; y++) {
      leds[XY( kMatrixWidth - 1 - x, y )] = leds[XY( x, y )];              
      leds[XY( x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];             
      leds[XY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )]; 
    }
  }
}


// copy one diagonal triangle into the other one within a 16x16

void Caleidoscope3() {
  for(int x = 0; x <= CentreX ; x++) {
    for(int y = 0; y <= x; y++) {
      leds[XY( x, y )] = leds[XY( y, x )]; 
    }
  }
}  


// copy one diagonal triangle into the other one within a 16x16 (90 degrees rotated compared to Caleidoscope3)

void Caleidoscope4() {
  for(int x = 0; x <= CentreX ; x++) {
    for(int y = 0; y <= CentreY-x; y++) {
      leds[XY( CentreY - y, CentreX - x )] = leds[XY( x, y )]; 
    }
  }
}  


// copy one diagonal triangle into the other one within a 8x8

void Caleidoscope5() {
  for(int x = 0; x < kMatrixWidth/4 ; x++) {
    for(int y = 0; y <= x; y++) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }

  for(int x = kMatrixWidth/4; x < kMatrixWidth/2 ; x++) {
    for(int y = kMatrixHeight/4; y >= 0; y--) {
      leds[XY( x, y )] = leds[XY( y, x )];
    }
  }
}

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
  // have less LEDs that the matrix
  for(int i = 0; i < NUM_MATRIX_LEDS*TOTAL_NUM_STRIPS; i++) {
    leds[i] = 0;
  }
}

