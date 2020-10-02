#pragma once

#include "debouncedButton.h"

button::button(int pin, DEBOUNCERANGE debounceTimer)
    : _customButton(false)
    , _pinStatus(nullptr)
    , _pin(pin)
    , _debounceTimer(debounceTimer)
    , _active(LOW) {
	pinMode(pin, INPUT_PULLUP);
}

button::button(int pin, bool active, DEBOUNCERANGE debounceTimer)
    : _customButton(false)
    , _pinStatus(nullptr)
    , _pin(pin)
    , _debounceTimer(debounceTimer)
    , _active(active) {
	pinMode(pin, INPUT);
}

button::button(bool customButton, uint8_t &pinStatus, DEBOUNCERANGE debounceTimer)
    : _customButton(customButton)
    , _pinStatus(&pinStatus)
    , _debounceTimer(debounceTimer)
    , _active(LOW) {
}

button::button(bool customButton, uint8_t &pinStatus, bool active, DEBOUNCERANGE debounceTimer)
    : _customButton(customButton)
    , _pinStatus(&pinStatus)
    , _debounceTimer(debounceTimer)
    , _active(active) {
}

bool button::_readButtonStatus() {
	if (_customButton) {
		if (*_pinStatus == _active ? HIGH : LOW) {
			return true;
		}
	} else {
		if (digitalRead(_pin) == _active ? HIGH : LOW) {
			return true;
		}
	}
	return false;
}

/**
 * button activated on press
 */
bool button::press() {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_press = true;
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
#if DEBUG
		Serial.print(_pin);
		Serial.print(" press: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
	}
	return _press;
}

/**
 * button activated repeatedly when held down
 * @param repeatSpeed1 repeat speed [in milliseconds]
 * @param repeatSpeed2 repeat speed after speed2delay time [in milliseconds]
 * @param repeatDelay hold time delay before repeatSpeed2 [in milliseconds]
 */
bool button::repeat(int repeatSpeed1, int repeatSpeed2, int repeatSpeed2delay) {
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
		if (millis() - _pressTime > repeatSpeed2delay) {
			if (millis() - _repeatedPress > repeatSpeed2) {
				_repeatedPress = millis();
				_press = true;
#if DEBUG
				Serial.print(_pin);
				Serial.print(" RepeatedPress @speed2: ");
				Serial.println(_repeatedPress);
#endif
			}
		} else if (millis() - _repeatedPress > repeatSpeed1) {
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

/**
 * button activated after specified time
 * @param pressDelay hold time [in milliseconds]
 */
bool button::longPress(int pressDelay) {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_longPressLock = false;
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
	} else if (!_longPressLock && _buttonPressed && millis() - _pressTime > pressDelay && _readButtonStatus()) {    // long press
		_press = true;
		_longPressLock = true;
#if DEBUG
		_releaseTime = millis();
		Serial.print(_pin);
		Serial.print(" LongPress: ");
		Serial.println(_releaseTime);
#endif
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
		if (!_buttonPressed) {
			_longPressLock = false;
		}
	}
	return _press;
}

/**
 * button activated on release
 */
bool button::release() {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
	} else if (_buttonPressed && !_readButtonStatus()) {    // debounce
		_buttonPressed++;
		if (!_buttonPressed) {    // release
			if (!_longPressLock) {
				_press = true;
			}
			_longPressLock = false;
#if DEBUG
			_releaseTime = millis();
			Serial.print(_pin);
			Serial.print(" release: ");
			Serial.println(_releaseTime);
#endif
		}
	}
	return _press;
}

/**
 * button active as long as it is pressed
 * @param pressDelay hold time [in milliseconds]
 */
bool button::hold(int pressDelay) {
	_press = false;
	if (!_buttonPressed && _readButtonStatus()) {    // press
		_buttonPressed = _debounceTimer;
		_pressTime = millis();
#if DEBUG
		Serial.print(_pin);
		Serial.print(" hold start: ");
		Serial.println(_pressTime);
#endif
	} else if (_buttonPressed && millis() - _pressTime > pressDelay && _readButtonStatus()) {    // hold
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
