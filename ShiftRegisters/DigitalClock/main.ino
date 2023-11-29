// Latch pin (ST_CP) of 74HC595 to pin 5
int latchPin = 5;      
// Latch pin (SH_CP) of 74HC595 to pin 6
int clockPin = 3;
// Data pin (DS of 74HC595 to pin 6
int dataPin = 7;       
// Make sure MR is HIGH (connect ot Vcc)

int minutes = 0;
int seconds = 0;

byte digits[10] = {
	0b11011101,	// 0
	0b01010000,	// 1
	0b11001110,	// 2
	0b11011010,	// 3
	0b01010011,	// 4
	0b10011011,	// 5
	0b10111111,	// 6
	0b11010000,	// 7
	0b11011111,	// 8
	0b11111011	// 9
};

void setup() {
	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	digitalWrite(clockPin, LOW);
  
	Serial.begin(9600);
}

void show_time(int min, int sec) {
	show_number(min / 10);
	show_number(min % 10);
	show_number(sec / 10);
	show_number(sec % 10);
}

void show_number(int digit) {
	digitalWrite(latchPin, LOW);
	if (digit < 0 or digit > 9) {
		return;
	}
	shiftOut(dataPin, clockPin, LSBFIRST, digits[digit]);
	digitalWrite(latchPin, HIGH);
}

void loop() {
	if (Serial.available() == 5) {
		String data = Serial.readStringUntil(':');
		minutes = data.toInt();
		data = Serial.readStringUntil('\n');
		seconds = data.toInt();

		for (int i = 0; i < 3600; ++i) {
			show_time(minutes, seconds);
			delay(1000);

			if (++seconds == 60) {
				seconds = 0;
				if (++minutes == 60) {
					minutes = 0;
				}
			}
		}
	}
}
