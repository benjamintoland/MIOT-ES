//============================================================================
// Name        : CA1.cpp
// Author      : Sean
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "GPIO.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;
using namespace exploringBB;

int callbackFunction(int var){
	cout << "BBB Button Pressed!" << var << endl;
	return var;
}


int main() {

		GPIO inGPIO(14); //Formerly 48 - SET to 14 to take in RX signal of grove cape.
		GPIO outGPIO(2); //Formerly 60  SET to 2 to take in RX signal of grove cape.

		inGPIO.setDirection(INPUT);
		inGPIO.setEdgeType(RISING);

		outGPIO.setDirection(OUTPUT);

		inGPIO.setDebounceTime(200);
		inGPIO.waitForEdge(&callbackFunction);
		outGPIO.toggleOutput(100);
	    cout << "Poll Started: Press the button:" << endl;
	    usleep(10000000);
	    cout << "Finished sleeping for 10 seconds" << endl;
}
