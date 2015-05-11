// MirroredNoise() draws a symmetric noise pattern
// 108 fps
void MirroredNoise() {
  // move within the noise space
  x[0] += 100;
  z[0] += 100;
  scale_x[0] = 4000;
  scale_y[0] = 4000;

  // calculate the noise array
  FillNoise(0);

  currentPalette = RainbowStripeColors_p;
  noisesmoothing = 10;

  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {
      // map the noise values down
      uint16_t index = (noise[0][i][j] + noise[0][kMatrixWidth - 1 - i][j] ) / 2;
      uint16_t bri = 255;
      // assign a color from the HSV space
      CRGB color = ColorFromPalette(currentPalette, index, bri);
      if (heartModeEnabled)
        leds[XY(i,j)] = CRGBInHeart(color, i, j);
      else
        leds[XY(i,j)] = color;
    }
  }
}

// RedClouds() draws a constrained noise space with a palette 
// softening the edges.
// Upstreaming red clouds.
// 108 fps

void RedClouds() {

  // clear the screenbuffer
  CLS();

  PaletteRed();
  colorshift = 240;

  // move within the noise space
  x[0] = beatsin16(1)*10;
  y[0] += 2000;
  z[0] += 100;
  scale_x[0] = 6000;
  scale_x[0] = 6000;

  // calculate the noise array
  FillNoise(0);

  for(int i = 0; i < kMatrixWidth; i++) {
    for(int j = 0; j < kMatrixHeight; j++) {

      // map the noise values down to a byte range
      uint16_t index = noise[0][i][j];
      uint16_t   bri = 255;
      // assign a color depending on the actual palette
      CRGB color = ColorFromPalette( currentPalette, index + colorshift, bri);

      // draw only the part lower than the threshold
      if (index < 128) { 
        leds[XY(i,j)] = color;
      }
    }
  }
}


// Lavalamp1
// works good with the RedBlack palette

void Lavalamp1() {  

  PaletteRed();
  colorshift = 0;

  x[0] = beatsin16(3, 200, 64000);
  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 8000;
  FillNoise(0); 

  x[1] = beatsin16(2, 200, 64000);
  y[1] += 130;
  z[1] = 7000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);   

  x[2] = beatsin16(4, 200, 6400);
  y[2] += 1000;
  z[2] = 3000;
  scale_x[2] = 7000;
  scale_y[2] = 8000;
  FillNoise(2);

  noisesmoothing = 200;

  MergeMethod1(2);
  //Show3Layers();
}


// with a scrolling palette

void Lavalamp2() {  

  currentPalette = PartyColors_p;

  noisesmoothing = 200;

  x[0] = beatsin16(3, 200, 64000);
  y[0] = beatsin16(4, 200, 64000);
  z[0] = 7000;
  scale_x[0] = beatsin16(2, 6000, 8000);
  scale_y[0] = beatsin16(1, 4000, 12000);
  FillNoise(0);  

  x[1] = beatsin16(5, 200, 64000);
  y[1] = beatsin16(6, 200, 64000);
  z[1] = 6000;
  scale_x[1] = 6000;
  scale_y[1] = 8000;
  FillNoise(1);  

  x[2] = beatsin16(4, 200, 6400);
  y[2] += 1000;
  z[2] = 3000;
  scale_x[2] = 7000;
  scale_y[2] = 8000;
  FillNoise(2);

  colorshift++;

  MergeMethod1(2);
  //Show3Layers();
}


// a very slow one 

void Lavalamp3() { 

  noisesmoothing = 40;
  currentPalette = ForestColors_p;

  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);  

  y[1] += 200;
  z[1] = 3000;
  scale_x[1] = 7000;
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 250;
  z[2] = 6000;
  scale_x[2] = 20000;
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod1(1);
  //Show3Layers();
}


// the palette can also be defined within the animation

void Lavalamp4() { 

  currentPalette = CRGBPalette16( 
  CHSV(   0, 255, 0    ), 
  CHSV(   0, 255, 255  ), 
  CHSV(   0, 255, 0    ),
  CHSV( 160, 255, 255  )); 

  noisesmoothing = 150;

  y[0] += 100;
  z[0] = 7000;
  scale_x[0] = 6000;
  scale_y[0] = 6000;
  FillNoise(0);  

  y[1] += 200;
  z[1] = 3000;
  scale_x[1] = 7000;
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 250;
  z[2] = 6000;
  scale_x[2] = 20000;
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod1(2);
  //Show3Layers();
}


// lets play with the scaling of 2 layers

void Lavalamp5() { 

  currentPalette = CRGBPalette16( 
  CHSV(   0, 255, 0   ), 
  CHSV(   0, 200, 255 ), 
  CHSV(  63, 150, 255 ),
  CHSV( 160, 255, 0   )); 

  noisesmoothing = 50;

  y[0] += 1000;
  z[0] = 7000;
  scale_x[0] = beatsin16(3, 1000, 20000);
  scale_y[0] = 6000;
  FillNoise(0);  

  y[1] += 2000;
  z[1] = 3000;
  scale_x[1] = beatsin16(4, 1000, 20000);
  scale_y[1] = 8000;
  FillNoise(1);

  y[2] += 3000;
  z[2] = 6000;
  scale_x[2] = beatsin16(5, 1000, 20000);
  scale_y[2] = 8000;
  FillNoise(2);  

  MergeMethod2(3);
  //Show3Layers();
}

