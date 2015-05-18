/*
 * RotaryEncoderWithButton.cpp
 *
 *  Created on: Aug 9, 2014
 *      Author: Konstantin Gredeskoul
 *
 *  (c) 2014 All rights reserved.  Please see LICENSE.
 */

#include "RotaryEncoderWithButton.h"

RotaryEncoderWithButton::RotaryEncoderWithButton(
		uint8_t rotaryPinA,
		uint8_t rotaryPinB,
		uint8_t buttonPin) {

	_lastButtonPressedAt = 0;
	_buttonPin = buttonPin;
  _lastRotaryValue = 0;
	encoder = new Encoder(rotaryPinA, rotaryPinB);
}

void RotaryEncoderWithButton::begin() {
	pinMode(_buttonPin, INPUT);
	digitalWrite(_buttonPin, HIGH);
  encoder->write(0);
}

int32_t RotaryEncoderWithButton::read() {
	return encoder->read();
}

bool isDebouncingButton = false;
uint32_t lastDebounceTime;
bool currentState = false;
bool previousState = false;

bool RotaryEncoderWithButton::buttonClicked() {
    bool pressed = false;

    if (millis() - _lastButtonPressedAt < BUTTON_PRESS_DELAY)
      return false;

    currentState = digitalRead(_buttonPin);      // store switch state
    if (currentState != previousState && currentState) {
        pressed = true;
        _lastButtonPressedAt = millis();
    }

    previousState = currentState;  // and save the current currentState

    // here when the switch currentState has been stable longer than the debounce period
    return pressed;
}
