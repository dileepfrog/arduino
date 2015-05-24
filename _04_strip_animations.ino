uint8_t gHue = 0; // rotating "base color" used by many of the patterns
void stripbpm()
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
