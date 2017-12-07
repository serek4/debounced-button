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
	 * initialization of button on given pin
     * @param pin: pin number
     * @param debounceTimer: pseudo timer for debouncing,
	 * higher number shorter debounce time, default 1,
     * 1-255 for DEBOUNCERANGE uint8_t,
     * 1-65535 for DEBOUNCERANGE uint16_t
     */
	button(int pin, DEBOUNCERANGE debounceTimer = 1);
	/**
	 * button activated on press
	 */
	boolean press();
	/**
	 * button activated repeatedly when held down
	 * @param _repeatSpeed1: repeat speed [in milliseconds], default = 500
	 * @param _repeatSpeed2: repeat speed after speed2delay time, [in milliseconds], default = 100
	 * @param _repeatDelay: hold time delay before repeatSpeed2, [in milliseconds], default = 1000
	 */
	boolean repeat(int _repeatSpeed1 = 400, int _repeatSpeed2 = 100, int _repeatSpeed2delay = 1500);
	/**
	 * button activated after specified time
	 * @param _pressDelay: hold time [in milliseconds], default = 2000
	 */
	boolean longPress(int _pressDelay = 2000);
	/**
	 * button activated on relese
	 */
	boolean relese();

  private:
	int _pin;
	DEBOUNCERANGE _buttonPressed = 0;
	DEBOUNCERANGE _debounceTimer;
	unsigned long _pressTime = 0;
#if DEBUG
	unsigned long _releaseTime = 0;
#endif
	unsigned long _repeatedPress = 0;
	boolean _press;
	boolean _longPressLock = false;
};