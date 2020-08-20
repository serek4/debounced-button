#pragma once

#include "debouncedButton.h"

button::button(int pin, DEBOUNCERANGE debounceTimer)
    : _pcf8574(nullptr) {
	pinMode(pin, INPUT_PULLUP);
	_pin = pin;
	_debounceTimer = debounceTimer;
	_active = LOW;
}

button::button(int pin, int active, DEBOUNCERANGE debounceTimer)
    : _pcf8574(nullptr) {
	pinMode(pin, INPUT);
	_pin = pin;
	_debounceTimer = debounceTimer;
	_active = active;
}

button::button(int pin, PCF8574 &pcf8574, DEBOUNCERANGE debounceTimer)
    : _pcf8574(&pcf8574) {
	_pcf8574->pinMode(pin, INPUT_PULLUP);
	_pin = pin;
	_debounceTimer = debounceTimer;
	_active = LOW;
}

button::button(int pin, PCF8574 &pcf8574, int active, DEBOUNCERANGE debounceTimer)
    : _pcf8574(&pcf8574) {
	_pcf8574->pinMode(pin, INPUT);
	_pin = pin;
	_debounceTimer = debounceTimer;
	_active = active;
}

boolean button::_readButtonStatus() {
	if (_pcf8574 != nullptr) {
		if (_pcf8574->digitalRead(_pin) == _active ? HIGH : LOW) {
			return true;
		}
	} else {
		if (digitalRead(_pin) == _active ? HIGH : LOW) {
			return true;
		}
	}
	return false;
}

boolean button::press() {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_press = true;
		_buttonPressed = _debounceTimer;
#if DEBUG
		_pressTime = millis();
		Serial.print(_pin);
		Serial.print(" press: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
	}
	return _press;
}

boolean button::repeat(int _repeatSpeed1, int _repeatSpeed2, int _repeatSpeed2delay) {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_press = true;
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
		_repeatedPress = _pressTime;
#if DEBUG
		Serial.print(_pin);
		Serial.print(" Press: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && _readButtonStatus()) {    // repeat
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
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
	}
	return _press;
}

boolean button::longPress(int _pressDelay) {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_longPressLock = false;
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
	} else if (_buttonPressed && millis() - _pressTime > _pressDelay && _readButtonStatus()) {    // long press
		if (!_longPressLock) {
			_press = true;
			_longPressLock = true;
#if DEBUG
			_releaseTime = millis();
			Serial.print(_pin);
			Serial.print(" LongPress: ");
			Serial.println(_releaseTime);
#endif
		}
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
	}
	return _press;
}

boolean button::relese() {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_buttonPressed = _debounceTimer;
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
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

boolean button::hold(int _pressDelay) {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
#if DEBUG
		Serial.print(_pin);
		Serial.print(" hold start: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && millis() - _pressTime > _pressDelay && _readButtonStatus()) {    // hold
		_press = true;
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
#if DEBUG
		if (!_buttonPressed) {
			_releaseTime = millis();
			Serial.print(_pin);
			Serial.print(" hold stop: ");
			Serial.println(_releaseTime);
		}
#endif
	}
	return _press;
}
