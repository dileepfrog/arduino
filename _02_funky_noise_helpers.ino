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

// used for the random based animations
int16_t dx;
int16_t dy;
int16_t dz;
int16_t dsx;
int16_t dsy;

// a 3dimensional array used to store the calculated 
// values of the different noise planes
uint8_t noise[NUM_LAYERS][kMatrixWidth][kMatrixHeight];
uint8_t noisesmoothing;

byte mode;
byte pgm;
byte spd;
byte brightness;
byte red_level;
byte green_level;
byte blue_level;

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

      leds[heartXY(i,j)] = CRGBInHeart(pixel, i, j);
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

      leds[heartXY(i,j)] = pixel;
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

      leds[heartXY(i,j)] = pixel;
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

      leds[heartXY(i,j)] = pixel;
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

        leds[heartXY(i,j)] = pixel;
      }
    }
  }
}

// All the caleidoscope functions work directly within the screenbuffer (leds array).
// Draw whatever you like in the area x(0-15) and y (0-15) and then copy it arround.

// rotates the first 16x16 quadrant 3 times onto a 32x32 (+90 degrees rotation for each one)

void Caleidoscope1() {
  for(int x = 0; x < kMatrixWidth / 2 ; x++) {
    for(int y = 0; y < kMatrixHeight / 2; y++) {
      leds[heartXY( kMatrixWidth - 1 - x, y )] = leds[XY( y, x )];    
      leds[heartXY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];    
      leds[heartXY( x, kMatrixHeight - 1 - y )] = leds[XY( y, x )];
    }
  }
}


// mirror the first 16x16 quadrant 3 times onto a 32x32

void Caleidoscope2() {
  for(int x = 0; x < kMatrixWidth / 2 ; x++) {
    for(int y = 0; y < kMatrixHeight / 2; y++) {
      leds[heartXY( kMatrixWidth - 1 - x, y )] = leds[XY( x, y )];              
      leds[heartXY( x, kMatrixHeight - 1 - y )] = leds[XY( x, y )];             
      leds[heartXY( kMatrixWidth - 1 - x, kMatrixHeight - 1 - y )] = leds[XY( x, y )]; 
    }
  }
}


// copy one diagonal triangle into the other one within a 16x16

void Caleidoscope3() {
  for(int x = 0; x <= CentreX ; x++) {
    for(int y = 0; y <= x; y++) {
      leds[heartXY( x, y )] = leds[XY( y, x )]; 
    }
  }
}  


// copy one diagonal triangle into the other one within a 16x16 (90 degrees rotated compared to Caleidoscope3)

void Caleidoscope4() {
  for(int x = 0; x <= CentreX ; x++) {
    for(int y = 0; y <= CentreY-x; y++) {
      leds[heartXY( CentreY - y, CentreX - x )] = leds[XY( x, y )]; 
    }
  }
}  


// copy one diagonal triangle into the other one within a 8x8

void Caleidoscope5() {
  for(int x = 0; x < kMatrixWidth/4 ; x++) {
    for(int y = 0; y <= x; y++) {
      leds[heartXY( x, y )] = leds[XY( y, x )];
    }
  }

  for(int x = kMatrixWidth/4; x < kMatrixWidth/2 ; x++) {
    for(int y = kMatrixHeight/4; y >= 0; y--) {
      leds[heartXY( x, y )] = leds[XY( y, x )];
    }
  }
}

// Show the current palette.

void ShowPalette() {
  for(int i = 0; i < NUM_MATRIX_LEDS; i++) {
    byte color = i / (NUM_MATRIX_LEDS / 256);
    byte bri = 255; 
    leds[i] = ColorFromPalette( currentPalette, color, bri );
  }
}



// show just one layer

void ShowLayer(byte layer, byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = noise[layer][i][j]; 

      uint8_t   bri = color;

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[heartXY(i,j)] = pixel;
    }
  }
}

// map a layer while ignoring the brightness information and replacing it by maximum

void ShowLayerBright(byte layer, byte colorrepeat) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color = noise[layer][i][j]; 

      uint8_t   bri = 255;

      // assign a color depending on the actual palette
      CRGB pixel = ColorFromPalette( currentPalette, colorrepeat * (color + colorshift), bri );

      leds[heartXY(i,j)] = pixel;
    }
  }
}

// Show 3 small 16x16 versions of the 3 noise planes
// to keep track what is going on where when.
// Useful to check before you start merging layers.
// Expects a 32x32 matrix to be the output device.

void Show3Layers() {
  for(uint8_t i = 0; i < 16; i++) {
    for(uint8_t j = 0; j < 16; j++) {
      leds[heartXY(i,j)] = ColorFromPalette( currentPalette, noise[0][i*2][j*2]*2 , 255 );
    }
  }
  for(uint8_t i = 16; i < 32; i++) {
    for(uint8_t j = 0; j < 16; j++) {
      leds[heartXY(i,j)] = ColorFromPalette( currentPalette, noise[1][(i-16)*2][j*2]*2 , 255 );
    }
  }
  for(uint8_t i = 0; i < 16; i++) {
    for(uint8_t j = 16; j < 32; j++) {
      leds[heartXY(i,j)] = ColorFromPalette( currentPalette, noise[2][i*2][(j-16)*2]*2 , 255 );
    }
  }
}

// allows to dim colors down
// works on the screenbuffer, after the image is computed

void ColorCorrection() {
  for(uint16_t i = 0; i < NUM_MATRIX_LEDS; i++) {
    leds[i].r = scale8(leds[i].r, red_level);
    leds[i].g = scale8(leds[i].g, green_level);
    leds[i].b = scale8(leds[i].b, blue_level);
  }
}


// a constrained noise the fills the holes with a mirrored and recolored version of the same noise

void CrossMapping(byte colorrepeat, byte limit) { 
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {

      uint8_t color1 = noise[0][i][j];
      uint8_t color2 = noise[0][j][i];

      CRGB pixel;

      if (color1 > limit) {
        pixel = ColorFromPalette( currentPalette, colorrepeat * (color1 + colorshift), color2 );
      }
      else {
        pixel = ColorFromPalette( currentPalette, colorrepeat * (color2 + colorshift + 128), color1 );
      }
      leds[heartXY(i,j)] = pixel;
    }
  }
}


// a brightness mask based on layer 0 for the complete screenbuffer

void FilterAll() {
  for(uint8_t i = 0; i < kMatrixWidth; i++) {
    for(uint8_t j = 0; j < kMatrixHeight; j++) {
      leds[heartXY(i,j)] %= noise[0][i][j];
    }
  }
}

/*
 Some color palettes. 
 Includes the predifined FastLED palettes and custom ones.
 
 -----------------------------------------------------------------
 */


// A red-black palette.

void PaletteRed() {
  currentPalette = CRGBPalette16( 
  CHSV( 0, 255, 255 ), 
  CHSV( 0, 255, 0   ), 
  CHSV( 0, 255, 0   ),
  CHSV( 0, 255, 255)); 
}


void PaletteCustom() {
  currentPalette = CRGBPalette16( 
  CHSV( 40, 255, 255), 
  CHSV( 40, 255, 255), 
  CHSV( 0, 255, 0   ),
  CHSV( 0, 255, 255)); 
}

// Set here a global color palette.
// All the the predifined FastLED palettes:

void Pal() {
  //PaletteRed();
  //PaletteCustom();
  //currentPalette = CloudColors_p;
  //currentPalette = LavaColors_p;
  //currentPalette = OceanColors_p;
  currentPalette = ForestColors_p;
  //currentPalette = RainbowColors_p;
  //currentPalette = RainbowStripeColors_p;
  //currentPalette = PartyColors_p;
  //currentPalette = HeatColors_p;
}


void SetupRandomPalette() {
  currentPalette = CRGBPalette16( 
  CHSV( random8(), 255, 32 ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 128, 255), 
  CHSV( random8(), 255, 255)); 
}


void Palette16() {
  currentPalette = CRGBPalette16(
  0x000000, 0xFF0000, 0xFF0000, 0x000000,
  0x000000, 0x00FF00, 0x00FF00, 0x000000,
  0x000000, 0x0000FF, 0x0000FF, 0x000000,
  0x000000, 0xFF0000, 0xFF0000, 0x000000); 
}


void SetupRandomPalette2() {
  currentPalette = CRGBPalette16( 
  CHSV( random8(), 255, 0 ), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 0),

  CHSV( random8(), 255, random8() ), 
  CHSV( random8(), random8(), 255), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 255),

  CHSV( random8(), 255, 0 ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), random8(), 255),

  CHSV( random8(), 255, 0 ), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 0));
}  


void SetupRandomPalette3() {
  currentPalette = CRGBPalette16( 
  CHSV( random8(), 255, 0 ), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 255),

  CHSV( random8(), 255, 0 ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 0),

  CHSV( random8(), 255, 0 ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 0),

  CHSV( random8(), 255, 255 ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 0));
}  


void SetupRandomPalette4() {
  currentPalette = CRGBPalette16( 
  CHSV( random8(), 255, random8() ), 
  CHSV( random8(), 255, random8()), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 255),

  CHSV( random8(), 255, random8() ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), random8(), random8()),

  CHSV( random8(), 255, random8() ), 
  CHSV( random8(), 255, 255), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, random8()),

  CHSV( random8(), 255, 255 ), 
  CHSV( random8(), 255, 0), 
  CHSV( random8(), 255, 255),
  CHSV( random8(), 255, random8()));
}  

// basically beatsin16 with an additional phase

uint16_t beatsin(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535, byte phase = 0)
{
  uint16_t beat = beat16( beats_per_minute);
  uint16_t beatsin = (sin16( beat+(phase*256)) + 32768);
  uint16_t rangewidth = highest - lowest;
  uint16_t scaledbeat = scale16( beatsin, rangewidth);
  uint16_t result = lowest + scaledbeat;
  return result;
}
