#include <debouncedButton.h>

#define LED1 12
#define LED2 13

boolean led1State = HIGH;
boolean led2State = HIGH;

button button1(2);
button button2(3);
int mode = 0;

void setup() {
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	digitalWrite(LED1, led1State);
	digitalWrite(LED2, led2State);
	Serial.begin(115200);
}

void loop() {
	if (button1.press()) {
		mode++;
		digitalWrite(LED1, HIGH);
		delay(100);
		digitalWrite(LED1, LOW);
	}
	switch (mode) {
		case 0:
			if (button2.press()) {
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		case 1:
			if (button2.relese()) {
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		case 2:
			if (button2.repeat()) {
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		default:
			mode = 0;
			break;
	}
}