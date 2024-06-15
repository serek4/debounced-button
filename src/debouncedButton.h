#pragma once

#include "Arduino.h"

// Serial monitor debug, you need initialize Serial monitor in sketch. [0 = off, 1 = on]
#ifndef DEBUG
#define DEBUG 0
#endif
// range for debounce pseudo timer [uint8_t(byte) or uint16_t(unsigned int)], default uint8_t
#ifndef DEBOUNCERANGE
#define DEBOUNCERANGE uint8_t
#endif

#define ACTIVE_LOW false
#define ACTIVE_HIGH true
// defaults
#define DEBOUNCE_TIMER 1
#define REPEAT_SPEED_1 400
#define REPEAT_SPEED_2 100
#define REPEAT_SPEED_2_DELAY 1500
#define LONG_PRESS_DELAY 2000
#define HOLD_DELAY 0

class Button {
  public:
	Button(int pin, DEBOUNCERANGE debounceTimer = DEBOUNCE_TIMER);
	Button(int pin, bool active, DEBOUNCERANGE debounceTimer = DEBOUNCE_TIMER);

	Button(uint8_t& pinStatus, DEBOUNCERANGE debounceTimer = DEBOUNCE_TIMER);
	Button(uint8_t& pinStatus, bool active, DEBOUNCERANGE debounceTimer = DEBOUNCE_TIMER);

	bool press();
	bool repeat(int rptSpeed1 = REPEAT_SPEED_1, int rptSpeed2 = REPEAT_SPEED_2, int rptSpeed2del = REPEAT_SPEED_2_DELAY);
	bool longPress(int pressDelay = LONG_PRESS_DELAY);
	bool release();
	bool hold(int pressDelay = HOLD_DELAY);

  private:
	int _pin;
	DEBOUNCERANGE _debounceTimer;
	bool _active;
	bool _press;
	DEBOUNCERANGE _buttonPressed;
	unsigned long _repeatedPress;
	bool _longPressLock;
	unsigned long _pressTime;
#if DEBUG
	unsigned long _releaseTime;
#endif
	bool _customButton;
	uint8_t* _pinStatus;
	bool _readButtonStatus();
};
