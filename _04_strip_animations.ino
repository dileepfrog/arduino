uint8_t gHue = 0; // rotating "base color" used by many of the patterns
void stripPushPulse()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_STRIP_LEDS; i++) { //9948
    leftArmLeds[i] = rightArmLeds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(leftArmLeds, NUM_STRIP_LEDS, 20);
  fadeToBlackBy(rightArmLeds, NUM_STRIP_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leftArmLeds[beatsin16(i+7,0,NUM_STRIP_LEDS)] |= CHSV(dothue, 200, 255);
    rightArmLeds[beatsin16(i+7,0,NUM_STRIP_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leftArmLeds, NUM_STRIP_LEDS, 20);
  fadeToBlackBy(rightArmLeds, NUM_STRIP_LEDS, 20);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  int pos = random16(NUM_STRIP_LEDS);
  leftArmLeds[pos] = rightArmLeds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(leftArmLeds, NUM_STRIP_LEDS, 20);
  fadeToBlackBy(rightArmLeds, NUM_STRIP_LEDS, 20);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  int pos = beatsin16(13,0,NUM_STRIP_LEDS);
  leftArmLeds[pos] += CHSV(gHue, 255, 192);
  rightArmLeds[pos] += CHSV(gHue, 255, 192);
}

// pop_fade variables ----------------------------------------------------------------------
int ranamount = 50;                                           // The higher the number, lowers the chance for a pixel to light up.
// uint8_t thisdelay = 50;                                       // Standard delay value.
bool boolcolours = 1;                                         // True = random, False = array of colours (up to 10)
uint8_t numcolours = 2;                                       // Number of colours in the array to display.
unsigned long colours[10] = {0xff0000, 0x00ff00, 0x0000ff, 0xffffff};  // Just assign the first 3 of these selectable colours.
uint8_t maxbar = 1;                                           // Maximum # of pixels to turn on in a row
uint8_t fadeval = 224;                                        // Fade rate
void pop_fade() {
  uint8_t ranamount = NUM_STRIP_LEDS;
  unsigned long thiscolour;
  if (ranamount > NUM_STRIP_LEDS) ranamount = NUM_STRIP_LEDS;              // Make sure we're at least utilizing ALL the LED's.
  int idex = random16(0, ranamount);

  if (idex < NUM_STRIP_LEDS) {                                      // Only the lowest probability twinkles will do.
    boolcolours ? thiscolour = random(0, 0xffffff) : thiscolour =  colours[random16(0, numcolours)];     
    int barlen = random16(1,maxbar);
    for (int i = 0; i <barlen; i++) 
      if (idex+i < NUM_STRIP_LEDS) leftArmLeds[idex+i] = thiscolour;       // Make sure we don't overshoot the array.
  }
     nscale8(leftArmLeds,NUM_STRIP_LEDS,fadeval);                          // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
}
