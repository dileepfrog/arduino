uint32_t lastTurnAt;
int32_t lastPosition = 0;
int32_t positionAtStartOfDebounce = 0;
bool isDebouncingRotary = false;

void readEncoderPosition() {
  int32_t currentPosition = encoder.read();
  if (currentPosition != lastPosition) {
    lastTurnAt = millis();
    lastPosition = currentPosition;
    if (!isDebouncingRotary) {
      positionAtStartOfDebounce = currentPosition;
      isDebouncingRotary = true;
    }
  }
  
  // Check if rotary position is now stable
  if (isDebouncingRotary && millis() - lastTurnAt > 500) {
    int32_t delta = currentPosition - positionAtStartOfDebounce;
    if (delta > 0)
      animationIndex++;
    else if (delta < 0)
      animationIndex--;

    // Wrap around
    if (animationIndex < 0)
      animationIndex = numAnimations + animationIndex;
    else if (animationIndex > 0)
      animationIndex = animationIndex % numAnimations;

    Serial.println(animationIndex);

    CLS();
    isDebouncingRotary = false;
  }
  
  if (encoder.buttonClicked()) {
    heartModeEnabled = !heartModeEnabled;
  }
}

