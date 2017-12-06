#define LED 13
#define BUTTON1 8
#define BUTTON2 9

boolean led = LOW;
byte button1_lock = 0;
byte button2_lock = 0;
unsigned long pressTime1 = 0;
unsigned long pressTime2 = 0;
unsigned long repeatTime1 = 0;
unsigned long repeatTime2 = 0;

void setup() {
	pinMode(BUTTON1, INPUT_PULLUP);
	pinMode(BUTTON2, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	Serial.begin(115200);
	delay(10);
}

boolean press(int button, boolean repeat = false, int repeatSpeed = 250, int repeatDelay = 1000) {
	boolean activate = false;
	if (!button1_lock && digitalRead(button) == LOW) {
		// button pressed
		button1_lock = 1;
		activate = true;
		pressTime1 = millis();
		Serial.print("press: ");
		Serial.println(pressTime1);
	} else if (button1_lock && millis() - pressTime1 > repeatDelay && digitalRead(button) == LOW && repeat) {
		// repeat button presses
		if (millis() - repeatTime1 > repeatSpeed) {
			activate = true;
			repeatTime1 = millis();
			Serial.print("repeat press: ");
			Serial.println(repeatTime1);
		}
	} else if (button1_lock && digitalRead(button) == HIGH) {
		// debounce
		button1_lock++;
	}
	return activate;
}

boolean release(int button, boolean repeat = false, int repeatSpeed = 250, int repeatDelay = 2000) {
	boolean activate = false;
	if (!button2_lock && digitalRead(button) == LOW) {
		// button pressed
		button2_lock = 1;
		pressTime2 = millis();
	} else if (button2_lock && millis() - pressTime2 > repeatDelay && digitalRead(button) == LOW && repeat) {
		// repeat button presses(releases)
		if (millis() - repeatTime2 > repeatSpeed) {
			activate = true;
			repeatTime2 = millis();
			Serial.print("repeat release: ");
			Serial.println(repeatTime2);
		}
	} else if (button2_lock && digitalRead(button) == HIGH) {
		// debounce
		button2_lock++;
		if (!button2_lock) {
			// realase
			activate = true;
			Serial.print("release: ");
			Serial.println(millis());
		}
	}
	return activate;
}

void loop() {
	if (press(BUTTON1, true, 100, 1500)) {
		led = !led;
	}
	if (release(BUTTON2, true, 200, 2000)) {
		led = !led;
	}
	digitalWrite(LED, led);
}
