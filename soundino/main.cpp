#include <WProgram.h>
#include <avr/interrupt.h>
#include "soundino.h"

/* PINs */
const int AUDIO = 9;

uint16_t phase[4];
uint16_t speed[4];
uint16_t sample = 0;

uint16_t sinTable[256];

const uint16_t scale[] = { 0, 1, 3, 5, 7, 8, 10, 12, 13, 15, 17 };
const uint16_t scaleL[] = { 5, 8, 3, 0, 1};
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

void setup() {
	Serial.begin(9600);
	Serial.println("Sound!");
	int five = getFive();
	Serial.print("Five::");
	Serial.println(five);

	initSinTable(sinTable);

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
	//Serial.print("Note ");
	//Serial.println(note);
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

int readNote(int pin, int noteNumber) {
	int sensorValue = analogRead(pin);
	if (sensorValue < 100) {
		return -1;
	}
	float volts = sensorValue * 0.0048828125;
	float distance = 27 / volts;
	int baseNote = 0;
	if (pin==4) {
		baseNote = scaleL[map(constrain((int)distance,8,40), 8, 40, 0, 5)];
		Serial.println(baseNote);
	} else {
		baseNote = scale[map(constrain((int)distance,8,40), 8, 40, 0, 7)];
	}
	return baseNote;
}

void loop() {
	int i;
	int noteL = readNote(4,4);
	int patternSpeed = 60;

	playNote(noteL, 10, 0);
	for (i = 0; i < 4; i++) {
		playNote(readNote(5,7), 10, 1);
		delay(patternSpeed);
	}

	if (noteL >= 0) {
		playNote(noteL + 12, 10, 0);
	}
	for (i = 0; i < 4; i++) {
		playNote(readNote(5,7), 10, 1);
		delay(patternSpeed);
	}
}

