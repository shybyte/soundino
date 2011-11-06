#include <WProgram.h>
#include <avr/interrupt.h>
#include "soundino.h"

/* Math */
const float Pi = 3.14159;

/* PINs */
const int AUDIO = 9;

uint16_t phase[4];
uint16_t speed[4];
uint16_t sample = 0;

const uint16_t freqTable[] = { 33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62,
		65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123, 131, 139, 147, 156,
		165, 175, 185, 196, 208, 220, 233, 247, 262, 277, 294, 311, 330, 349,
		370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784,
		831, 880, 932, 988, 1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568,
		1661, 1760, 1865, 1976, 2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136,
		3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978 };

uint16_t sinTable[256];

const uint16_t scale[] = { 0, 2, 3, 5, 7, 8, 11, 12, 14, 15, 17 };
//const uint16_t scale[] = { 0, 2, 3, 5, 7, 8, 11, 12, 14, 15, 17 };

int distance;

int main_old(void) {
	init();
	setup();
	for (;;)
		loop();
	return 0;
}

void writeAudio(uint8_t val) {
	OCR1A = (val);
	//analogWrite(AUDIO,val);
}

SIGNAL(TIMER1_OVF_vect) {
	writeAudio(sample);
	phase[0] += speed[0];
	phase[1] += speed[1];
	int sample1 = (phase[0] >> 10) & 127;

	int sample2 = (sinTable[(phase[1] >> 6) & 0xff]
			+ sinTable[(phase[1] >> 5) & 0xff]) >> 1;
	sample = (sample1 + sample2) >> 1;
}

void setupSinTable() {
	for (int i = 0; i < 256; i++) {
		float x = ((float) i) * 2 * PI / 256;
		sinTable[i] = (int) (sin(x) * 100 + 128);
		Serial.print(i);
		Serial.print("=");
		Serial.println(sinTable[i]);
	}
}

void setup() {
	Serial.begin(9600);
	Serial.println("Sound!");
	int five = getFive();
	Serial.print("Five::");
	Serial.println(five);

	setupSinTable();

	pinMode(AUDIO, OUTPUT);
	pinMode(13, OUTPUT);

	TCCR1A = _BV(WGM10) | _BV(COM1A1);
	TCCR1B = _BV(CS10) | _BV(WGM12);
	TIMSK1 |= _BV(TOIE1);

	distance = analogRead(5);
	speed[0] = 0;
	speed[1] = 0;
	phase[0] = 0;
	phase[1] = 0;

}

void playNote(int note, int time, int channel) {
	Serial.print("Note ");
	Serial.println(note);
	if (note >= 0) {
		int freq = freqTable[note + 21];
		speed[channel] = freq + freq / 21;
	} else {
		speed[channel] = 0;
	}
}

void playPattern(int baseNote, int channel) {
	playNote(baseNote, 50, channel);
	/*
	 playNote(baseNote + 4, 200);
	 playNote(baseNote + 7, 200);
	 playNote(baseNote + 4, 200);
	 */
}

int readNote(int pin) {
	int sensorValue = analogRead(pin);
	if (sensorValue < 100) {
		return -1;
	}
	float volts = sensorValue * 0.0048828125;
	float distance = 27 / volts;
	int baseNote = scale[map(constrain((int)distance,10,40), 10, 40, 0, 7)];
	return baseNote;
}

void loop() {
	int i;
	int noteL = readNote(4);
	int patternSpeed = 50;

	playNote(noteL, 10, 0);
	for (i = 0; i < 4; i++) {
		playNote(readNote(5), 10, 1);
		delay(patternSpeed);
	}

	if (noteL >= 0) {
		playNote(noteL + 12, 10, 0);
	}
	for (i = 0; i < 4; i++) {
		playNote(readNote(5), 10, 1);
		delay(patternSpeed);
	}
}

