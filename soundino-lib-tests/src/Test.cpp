#include "cute.h"
#include "ide_listener.h"
#include "cute_runner.h"
#include "soundino.h"

void thisIsATest() {
	ASSERT_EQUAL(1,getFive());
}

void runSuite(){
	cute::suite s;
	//TODO add your test here yeahhh hossa
	s.push_back(CUTE(thisIsATest));
	cute::ide_listener lis;
	cute::makeRunner(lis)(s, "The Suite");
}

int main(){
    runSuite();
    return 0;
}



