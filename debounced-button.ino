#define LED 13
#define BUTTON1 8
#define BUTTON2 9

boolean led = LOW;
byte button1_lock = 0;
byte button2_lock = 0;
unsigned long pressTime1 = 0;
unsigned long pressTime2 = 0;

void setup() {
	pinMode(BUTTON1, INPUT_PULLUP);
	pinMode(BUTTON2, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	Serial.begin(115200);
	delay(10);
}

void loop() {
	if (press(BUTTON1)) {
		led = !led;
	}
	if (release(BUTTON2)) {
		led = !led;
	}
	digitalWrite(LED, led);
}

boolean press(int button) {
	boolean activate = false;
	if (!button1_lock && digitalRead(button) == LOW) {
		// button pressed
		button1_lock = 1;
		activate = true;
		pressTime1 = millis();
		Serial.print("press: ");
		Serial.println(pressTime1);
	} else if (button1_lock && millis() - pressTime1 > 1000 && digitalRead(button) == LOW) {
		// repeat button presses
		button1_lock = 0;
	} else if (button1_lock && digitalRead(button) == HIGH) {
		// debounce
		button1_lock++;
	}
	return activate;
}

boolean release(int button) {
	boolean activate = false;
	if (!button2_lock && digitalRead(button) == LOW) {
		// button pressed
		button2_lock = 1;
		pressTime2 = millis();
	} else if (button2_lock && millis() - pressTime2 > 2000 && digitalRead(button) == LOW) {
		// repeat button presses(releases)
		button2_lock = 1;
		activate = true;
		pressTime2 = millis();
		Serial.print("release: ");
		Serial.println(pressTime2);
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
