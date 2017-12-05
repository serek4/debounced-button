#define LED 13
#define BUTTON 8

boolean led = LOW;
byte button_lock = 0;

void setup() {
	pinMode(BUTTON, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	delay(10);
}

void loop() {
	if (!button_lock && digitalRead(BUTTON) == LOW) {
		// button pressed
		button_lock = 1;
		led = !led;
	} else if (button_lock && digitalRead(BUTTON) == HIGH) {
		// debounce
		button_lock++;
	}
	digitalWrite(LED, led);
}
