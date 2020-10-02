#include <PCF8574.h>
#include <debouncedButton.h>

#define LED1 13
#define LED2 14

boolean led1State = HIGH;
boolean led2State = HIGH;

PCF8574 keypad(0x20);
button button1(true, keypad.digitalInput.p0);
button button2(true, keypad.digitalInput.p1);
int mode = 0;
char *modeStr[6] = {"press", "repeat", "long press", "release", "hold", "press"};

void setup() {
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	digitalWrite(LED1, led1State);
	digitalWrite(LED2, led2State);
	keypad.pinMode(P0, INPUT_PULLUP);
	keypad.pinMode(P1, INPUT_PULLUP);
	keypad.begin();
	Serial.begin(115200);
}

void loop() {
	keypad.digitalReadAll();
	if (button1.press()) {
		mode++;
		Serial.println(modeStr[mode]);
		digitalWrite(LED1, HIGH);
		delay(100);
		digitalWrite(LED1, LOW);
	}
	switch (mode) {
		case 0:
			if (button2.press()) {
				Serial.println("button 2 press");
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		case 1:
			if (button2.repeat()) {
				Serial.println("button 2 repeat");
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		case 2:
			if (button2.longPress()) {
				Serial.println("button 2 long press");
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		case 3:
			if (button2.release()) {
				Serial.println("button 2 release");
				led2State = !led2State;
				digitalWrite(LED2, led2State);
			}
			break;
		case 4:
			if (button2.hold()) {
				Serial.println("button 2 hold");
				led2State = true;
			} else {
				led2State = false;
			}
			digitalWrite(LED2, led2State);
			break;
		default:
			mode = 0;
			break;
	}
}
