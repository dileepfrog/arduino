uint16_t lastTurnAt;
uint8_t lastTurnDelta;

void readEncoderPosition() {
  long delta = encoder.rotaryDelta();
  if (delta != 0) {
    lastTurnAt = millis();
    lastTurnDelta = delta;
  }
  
  if (lastTurnDelta != 0 && millis() - lastTurnAt > 200) {  
    if (lastTurnDelta > 0) {
      animationIndex = (animationIndex+1)%numAnimations;
      lastTurnDelta = 0;
      CLS();
    }
    else if (lastTurnDelta < 0) {
      animationIndex = (animationIndex-1)%numAnimations;
      lastTurnDelta = 0;
      CLS();
    }
  }
  
  if (encoder.buttonClicked()) {
    heartModeEnabled = !heartModeEnabled;
  }
}

