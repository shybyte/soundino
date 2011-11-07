#include  <iostream>
#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "soundino.h"

void print(uint16_t *sinTable, int size) {
	for (int i = 0; i < size; i++) {
		std::cout << sinTable[i] << "\n";
	}
}

void testSinTable() {
	uint16_t sinTable[256];
	initSinTable(sinTable);
	//	print(sinTable,256);
}

void testGetSoundinoVersion() {
	ASSERT(getSoundinoVersion()>0);
}

void runSuite() {
	cute::suite s;
	s.push_back(CUTE(testGetSoundinoVersion));
	s.push_back(CUTE(testSinTable));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "The Soundino Suite");
}

int main() {
	runSuite();
	return 0;
}

