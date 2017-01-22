/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<unistd.h> //for usleep
#include"GPIO.h"
#include"PWM.h"
#include"util.h"
using namespace exploringBB;
using namespace std;


#include <stdio.h>
#include <stdlib.h>

#define PWM_PATH "/sys/devices/ocp.3/"		// pwm_test_P9_22.13

int main(int argc, char** argv)
{
	if(system("echo bone_pwm_P9_22 > $SLOTS"))
	{
		printf("P9_22 PWM Already loaded!\n");
	}
	else
	{
		printf("P9_22 PWM Loaded!\n");
		system("echo am33xx_pwm > $SLOTS");
	}

		// My code

	PWM pwm("pwm_test_P9_22.13");		// Keep this if not using 'System' method for servo control***

	GPIO outGPIO1(14), outGPIO2(2);		// 14 is LED and 2 is Servo

	outGPIO1.setDirection(OUTPUT);		// Output to LED
	outGPIO1.setValue(LOW);				// LED Off			Does this need to be here?

	outGPIO2.setDirection(OUTPUT);		// Output to Servo
	outGPIO2.setValue(LOW);				// 					Does this need to be here?

	pwm.setPeriod(20000000);			// Period in ns
	pwm.setPolarity(PWM::ACTIVE_HIGH);	// Set Polarity to 0
//	pwm.setDutyCycle(10.5f);			// 2100000ns	Brighter
	pwm.setDutyCycle(2.5f);				// 500000ns		Dim
	pwm.run();


	return 0;
}




//	system("echo 0 > run > $PWM_PATH");						// Turn OFF

//	system("echo 1 > run");						// Turn ON

/*
	outGPIO1.setDirection(OUTPUT);				// Output to LED
	outGPIO1.setValue(LOW);					// LED Off			Does this need to be here?
*/

/*

	outGPIO2.setDirection(OUTPUT);				// Output to Servo
	outGPIO2.setValue(LOW);						// 					Does this need to be here?
    system("echo 0 > run");						// Turn off
	system("echo 0 > polarity");				// Already declared in GPIO Enable
	system("echo 20000000 > period");			// Already declared in GPIO Enable
	system("echo 500000 > duty");				// Duty cycle to Close Vent
//	system("echo 2100000 > duty");				// Vent Open
	system("echo 1 > run");						// Turn on
*/





/*
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
}*/

