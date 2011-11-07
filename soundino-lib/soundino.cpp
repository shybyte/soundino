#include <stdint.h>
#include <math.h>
#include "soundino.h"

/* Math Hossa */

const float PI = 3.1415926535897932384626433832795;

int getSoundinoVersion() {
	return 1;
}

void initSinTable(uint16_t *sinTable) {
	for (int i = 0; i < 256; i++) {
		float x = ((float) i) * 2 * PI / 256;
		sinTable[i] = (int) (sin(x) * 100 + 128);
	}
}
