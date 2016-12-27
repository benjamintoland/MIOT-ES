/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<unistd.h> //for usleep
#include"GPIO.h"
#include"PWM.h"
#include "I2CDevice.h"
#include<fstream>
#include<string>
#include<sstream>
#include <math.h>
using namespace exploringBB;
using namespace std;
#define TEMP_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
const int B =4275;
int readAnalog(int number){
   stringstream ss;
   ss << TEMP_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;
}

int main(){
	if(system("echo BB-ADC > $SLOTS"))
	{
		cout << "BB-ADC DTO already loaded" << endl;
	}
	if(system("echo bone_pwm_P9_22 > $SLOTS"))
		{
			cout << "PWM  DTO already loaded" << endl;
		}
	else
	{

		cout << "PWM  DTO has just been loaded, now for am33xx..." << endl;
		system("echo am33xx_pwm > $SLOTS");
	}

	PWM pwm("pwm_test_P9_22.13");
		GPIO outGPIO(14), inGPIO(2);//gpio(2) is J6 rx (SERV0 CONTROL INPUT), gpio(14) is j2 rx (LED CONTROL OUTPUT)
		outGPIO.setDirection(OUTPUT);
		outGPIO.setValue(LOW);
		 cout << "heater off:" << endl;

   pwm.setPeriod(20000000);         // Set the period in ns, 20ms
   	     pwm.setDutyCycle(5.0f);       // Set the duty 5%
   	     pwm.setPolarity(PWM::ACTIVE_HIGH);
   	     cout << "vent closed:" << endl;// using active low PWM
   	     pwm.run();
   cout << "Starting the readTHermistor program" << endl;
   while(1)
   {

	   usleep (5000000);// there was a bit of bounce.
	   int value = readAnalog(0);
   cout << "The Thermistor value was " << value << " out of 4275." << endl;
   float R= 4096.0/((float)value)-1.0;
   R = 100000.0*R;
   float temperature =1.0/(log(R/10000.0)/B+1/298.15)-273.15;// convert an val to temp C.
   cout << "Temerature is : " << temperature << "Degrees Celcius" << endl;
   usleep(500000);
   if ((value >1150)&&(value <1250))
   {
	   pwm.setDutyCycle(5.0f); //low duty cycle,9 o clock..
	   outGPIO.setValue(LOW);
	   cout << "heater still off:" << endl;
	   cout << "vent closed:" << endl;
   }

   else if (value <= 1150)
   {
	   outGPIO.setValue(HIGH);
	   cout << "heater ON!" << endl;
   }
   else if (value >= 1250)
   {
	   outGPIO.setValue(LOW);
	   cout << "very hot,turn off heater!" << endl;
	   pwm.setDutyCycle(13.0f);//duty 75%
	   	     cout << "vent open" << endl;// servo at 3 o clock i hope..
   }


 /*  // Basic Output - Flash the LED 10 times, once per second
   outGPIO.setDirection(OUTPUT);
   for (int i=0; i<10; i++){
      outGPIO.setValue(HIGH);
      usleep(500000); //micro-second sleep 0.5 seconds
      outGPIO.setValue(LOW);
      usleep(500000);
   }
   // Basic Input example
   inGPIO.setDirection(INPUT);
   cout << "The value of the input is: "<< inGPIO.getValue() << endl;

   // Fast write to GPIO 1 million times
   outGPIO.streamOpen();
   for (int i=0; i<1000000; i++){
      outGPIO.streamWrite(HIGH);
      outGPIO.streamWrite(LOW);
   }
   outGPIO.streamClose();
*/
   }
   return 0;

}
