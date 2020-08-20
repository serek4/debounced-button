#pragma once

#include "Arduino.h"

// Serial monitor debug, you need initialize Serial monitor in sketch. [0 = off, 1 = on]
#ifndef DEBUG
#define DEBUG 0
#endif
// range for decounce pseudo timer [uint8_t(byte) or uint16_t(unsigned int)], default uint8_t
#ifndef DEBOUNCERANGE
#define DEBOUNCERANGE uint8_t
#endif

class button {
  public:
	/**
     * @param pin pin number
	 * @param active active state, LOW or HIGH
     * @param debounceTimer pseudo timer for debouncing,
	 * higher number shorter debounce time, default 1,
     * 1-255 for DEBOUNCERANGE uint8_t,
     * 1-65535 for DEBOUNCERANGE uint16_t
     */
	button(int pin, DEBOUNCERANGE debounceTimer = 1);
	button(int pin, int active, DEBOUNCERANGE debounceTimer = 1);
	/**
	 * button activated on press
	 */
	bool press();
	/**
	 * button activated repeatedly when held down
	 * @param repeatSpeed1 repeat speed [in milliseconds], default = 500
	 * @param repeatSpeed2 repeat speed after speed2delay time, [in milliseconds], default = 100
	 * @param repeatDelay hold time delay before repeatSpeed2, [in milliseconds], default = 1000
	 */
	bool repeat(int repeatSpeed1 = 400, int repeatSpeed2 = 100, int repeatSpeed2delay = 1500);
	/**
	 * button activated after specified time
	 * @param pressDelay hold time [in milliseconds], default = 2000
	 */
	bool longPress(int pressDelay = 2000);
	/**
	 * button activated on relese
	 */
	bool relese();
	/**
	 * button active as long as it is pressed
	 * @param pressDelay hold time [in milliseconds], default = 0
	 */
	bool hold(int pressDelay = 0);

  private:
	int _pin;
	DEBOUNCERANGE _debounceTimer;
	bool _active;
	bool _press;
	DEBOUNCERANGE _buttonPressed = 0;
	unsigned long _repeatedPress = 0;
	bool _longPressLock = false;
	unsigned long _pressTime = 0;
#if DEBUG
	unsigned long _releaseTime = 0;
#endif
	bool _readButtonStatus();
};
