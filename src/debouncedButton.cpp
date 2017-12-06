#pragma once

#include "debouncedButton.h"

button::button(int pin, DEBOUNCERANGE debounceTimer) {
	pinMode(pin, INPUT_PULLUP);
	_pin = pin;
	_debounceTimer = debounceTimer;
}

boolean button::press() {
	_press = false;
	if (!_buttonPressed && digitalRead(_pin) == LOW) {    // press
		_press = true;
		_buttonPressed = _debounceTimer;
#if DEBUG
		_pressTime = millis();
		Serial.print(_pin);
		Serial.print(" press: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && digitalRead(_pin) == HIGH) {    // debounce
		_buttonPressed++;
	}
	return _press;
}

boolean button::repeat(int _repeatSpeed1, int _repeatSpeed2, int _repeatSpeed2delay) {
	_press = false;
	if (!_buttonPressed && digitalRead(_pin) == LOW) {    // press
		_press = true;
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
		_repeatedPress = _pressTime;
#if DEBUG
		Serial.print(_pin);
		Serial.print(" Press: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && digitalRead(_pin) == LOW) {    // repeat
		if (millis() - _pressTime > _repeatSpeed2delay) {
			if (millis() - _repeatedPress > _repeatSpeed2) {
				_repeatedPress = millis();
				_press = true;
#if DEBUG
				Serial.print(_pin);
				Serial.print(" RepeatedPress @speed2: ");
				Serial.println(_repeatedPress);
#endif
			}
		} else if (millis() - _repeatedPress > _repeatSpeed1) {
			_repeatedPress = millis();
			_press = true;
#if DEBUG
			Serial.print(_pin);
			Serial.print(" RepeatedPress @speed1: ");
			Serial.println(_repeatedPress);
#endif
		}
	} else if (_buttonPressed && digitalRead(_pin) == HIGH) {    // debounce
		_buttonPressed++;
	}
	return _press;
}

boolean button::relese() {
	_press = false;
	if (!_buttonPressed && digitalRead(_pin) == LOW) {    // press
		_buttonPressed = _debounceTimer;
	} else if (_buttonPressed && digitalRead(_pin) == HIGH) {    // debounce
		_buttonPressed++;
		if (!_buttonPressed) {    // relese
			_press = true;
#if DEBUG
			_releaseTime = millis();
			Serial.print(_pin);
			Serial.print(" relese: ");
			Serial.println(_releaseTime);
#endif
		}
	}
	return _press;
}
