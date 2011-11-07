#include  <iostream>
#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "soundino.h"

void testSinTable(){
	uint16_t sinTable[256];
	initSinTable(sinTable);
	for(int i=0;i<256;i++) {
		std::cout << sinTable[i] << "\n";
	}
}



void thisIsATest() {
	ASSERT_EQUAL(1,getFive());
}

void runSuite(){
	cute::suite s;
	//TODO add your test here yeahhh hossa
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(testSinTable));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "The Suite");
}

int main(){
    runSuite();
    return 0;
}



