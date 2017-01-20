/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<unistd.h> //for usleep
#include"GPIO.h"
using namespace exploringBB;
using namespace std;



int callbackFunction(int var){
	cout << "BBB Button Pressed!" << var << endl;
	return var;
}

int main(){
   GPIO outGPIO(14), inGPIO(4);	// LED out=2 UART2, Button in=14 UART1

   inGPIO.setDirection(INPUT);
   inGPIO.setEdgeType(RISING);

   cout << "Kicking off waiting thread..." << endl;
   inGPIO.waitForEdge(&callbackFunction);

   outGPIO.setDirection(OUTPUT);
   cout << "Kicking off blinking thread..." << endl;
   outGPIO.toggleOutput(20, 500);


   for (int i=0; i<10; i++){
	  cout << "Blowing Bubbles..." << endl;
      usleep(1000000);
   }

   return 0;
}

