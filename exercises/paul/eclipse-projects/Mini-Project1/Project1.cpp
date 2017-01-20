/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <unistd.h> // for usleep
#include <fstream>	// tmp36
#include <string>	// tmp36
#include <sstream>	// tmp36
#include <stdlib.h>	// tmp36
#include <math.h>	// Grove Temperature sensor
#include <pthread.h>
#include <iomanip>	// Check this out??
#include "GPIO.h"	// GPIO functions
#include "PWM.h"	// For PWM
#include "I2CDevice.h"	// I2C devices
#include "util.h"
using namespace exploringBB;
using namespace std;

#define TEMPSENS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"	// tmp36 Path changed from LDR
#define SLOTS "/sys/devices/bone_capemgr.9/slots"	// Our BBB has this loaded into .profile but other systems may not

const int B = 4275;	// B value of thermistor from Grove sensor code

int count = 0;		// See if I can get this else where

/* Code from A TMP36 temperature sensor application
 * Conversion values from Grove temperature Sensor code*/

int readAnalog(int number){ // returns the input as an int
	stringstream ss;
	ss << TEMPSENS_PATH << number << "_raw";
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> number;
	fs.close();
	return number;
}

float getTemperature(int adc_value){
    float R = 4096.0/((float)adc_value)-1.0;	// Vcc = 5V for Temperature Sensor, Range = 0-4095
    R = 100000.0*R;
    float temperature = 1.0/(log(R/100000.0)/B+1/298.15)-273.15;
    return (temperature);
}

// Button Press with current time displayed

int callbackFunction(int var){
	cout << "BBB Button Pressed!" << var << endl;
		if(count%2==0)
		{
			system("date");	// Add script at boot to get current time
		}
				count++;	// Do I need a callback?
				return var;
}


int main(int argc, char* argv[]){

	if(system("echo BB-ADC > $SLOTS"))	// Check or Load DTO to enable Analog input for Temperature Sensor
	{
		cout << "DTO BB-ADC for Analog inputs already loaded\n" << endl;
	}
	else
	{
		cout << "DTO BB-ADC for Analog inputs loaded\n" << endl;
	}

	if(system("echo bone_pwm_P9_22 > $SLOTS"))	// Check or Load DTO to enable PWM for Servo Motor
	{
		cout << "DTO PWM P9_22 for Servo already loaded\n"  << endl;
	}
	else
	{
		cout << "DTO PWM P9_22 for Servo loaded\n" << endl;
		system("echo am33xx_pwm > $SLOTS");	// Enable hardware on SOC
	}

// Enable Backlight and LCD

	char row1 [16];
	char row2 [16];

	I2CDevice BL(1, 0x62);			// Enable Backlight
	BL.open();
	BL.writeRegister(0x00, 0x00);	// Initialise registers
	BL.writeRegister(0x08, 0xff);
	BL.writeRegister(0x01, 0x20);
	BL.writeRegister(0x02, 0x55);	// Backlight Blue medium
	BL.writeRegister(0x03, 0x55);	// Backlight Green medium
	BL.writeRegister(0x04, 0x55);	// Backlight Red medium

	I2CDevice LCD(1,0x3e);			// Enable LCD
	LCD.open();
	LCD.writeRegister(0x80, 0x01);	// Clear screen
	LCD.writeRegister(0x80, 0x28);	// 2 line mode
	LCD.writeRegister(0x80, 0x0f);	// Switch on with blinking
	LCD.writeRegister(0x80, 0x02);	// Cursor Home


/*
	LCD.writeRegister(0x80, 0x82);	// R0C2
	LCD.writeRegister(0x40, 0x33);	// Print 3
	LCD.writeRegister(0x80, 0x84);	// R0C4
	LCD.writeRegister(0x40, 0x31);	// Print 1
	LCD.writeRegister(0x80, 0xc5);	// R1C5
	LCD.writeRegister(0x40, 0x32);	// Print 2
*/



	int ain = 0;
	cout << "Starting the Temperature Sensor program" << endl;
	if (argc>1) ain = atoi(argv[1]);
	int value = readAnalog(ain);
	cout << "The ADC value is: " << value << endl;	// Remove at later stage
	float temperature = getTemperature(value);
	cout << "The temperatures is: " << temperature << " degrees Celsius." << endl;
	usleep(50000);

	/* End of temperature code, need to do further work on this later
	 * added DTO BB-ADC to SLOTS code
	 */




	// Enable GPIO

	PWM pwm("pwm_test_P9_22.13");

	GPIO outGPIO1(14), outGPIO2(2);	// 14 is LED and 2 is Servo

	outGPIO1.setDirection(OUTPUT);
	outGPIO1.setValue(LOW);			// Does this need to be here?

	outGPIO2.setDirection(OUTPUT);
	outGPIO2.setValue(LOW);			// Does this need to be here?
    system("echo 0 > run");				// Turn off
    system("echo 0 > polarity");		// Set Pulse polarity
	system("echo 20000000 > period");	// Set Period = 20ms

	while(1)

	{

	// LED Heater On

	if ((value <= 2000)&&(count%2==0));					// Change this value later
	{
		outGPIO1.setValue(HIGH);		// Turn LED On
		cout << "Heater On, Vent Closed" << endl;

		sprintf(row1, "HeatON.VentCLOSE");
		sprintf(row2, "Degrees C:");
		LCD.writeRegister(0x80, 0x01);	// Clear screen
		LCD.writeRegister(0x80, 0x80);	// Row 0, Col 0 change to Home
			int i=0;
			while(row1[i]!='\0');
			{
				LCD.writeRegister(0x40, row1[i]);
				i++;
			}
			LCD.writeRegister(0x80, 0xc0);
				i=0;
				while(row2[i]!='\0');
				{
					LCD.writeRegister(0x40, row2[i]);
					i++;
				}
	}

		// Vent Open

	else if ((value >=2200)&&(count%2==0));	//Change value later
	{
		outGPIO2.setValue(LOW);
		cout << "Vent Open, Heater Off" << endl;

	    system("echo 0 > run");				// Turn off
//	    system("echo 0 > polarity");
//		system("echo 20000000 > period");	// Period = 20ms
		system("echo 200000 > duty");		// Max open
		system("echo 1 > run");				// Turn on

		sprintf(row1, "VentOPEN.HeatOFF");
		sprintf(row2, "Degrees C:");
		LCD.writeRegister(0x80, 0x01);	// Clear screen
		LCD.writeRegister(0x80, 0x80);	// Row 0, Col 0 change to Home
			int i=0;
			while(row1[i]!='\0');
			{
				LCD.writeRegister(0x40, row1[i]);
				i++;
			}
			LCD.writeRegister(0x80, 0xc0);
				i=0;
				while(row2[i]!='\0');
				{
					LCD.writeRegister(0x40, row2[i]);
					i++;
				}

	}

	else
	{
		cout << "Time is" << endl;
	}


/*
		ostringstream s;				// To combine text and int data
		s.str("");
		LCD.writeRegister(0x80, 0xc0);	// Row 1, Col 0
		s << "Temp Deg C: " << temperature;
		printf(s.str());

*/



/*
	outGPIO.setDirection(OUTPUT);
    outGPIO.setValue(LOW);


   if (value >= 1900)
    {
    	system("echo 0 > run");
    	system("echo 0 > polarity");
		system("echo 20000000 > period");
		system("echo 200000 > duty");	// Max open
		system("echo 1 > run");
    }
*/
	}

	return 0;
}
