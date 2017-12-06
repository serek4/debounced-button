#define LED 13
#define BUTTON 8

boolean led = LOW;
byte button_lock = 0;
unsigned long pressTime = 0;

void setup() {
	pinMode(BUTTON, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
	digitalWrite(LED, LOW);
	Serial.begin(115200);
	delay(10);
}

void loop() {
	if (button(BUTTON)) {
		led = !led;
	}
	digitalWrite(LED, led);
}

boolean button(int button) {
	boolean press = false;
	if (!button_lock && digitalRead(button) == LOW) {
		// button pressed
		button_lock = 1;
		press = true;
		pressTime = millis();
		Serial.print("press: ");
		Serial.println(pressTime);
	} else if (millis() - pressTime > 1000 && digitalRead(button) == LOW) {
		// repeat button presses
		button_lock = 0;
	} else if (button_lock && digitalRead(button) == HIGH) {
		// debounce
		button_lock++;
	}
	return press;
}
