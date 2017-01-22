/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include <iostream>
#include <unistd.h>
#include <fstream>		// tmp36
#include <string>		// tmp36
#include <sstream>		// tmp36
#include <stdlib.h>		// tmp36
#include <math.h>		// Grove Temperature sensor
#include <pthread.h>
#include <iomanip>
#include "GPIO.h"		// GPIO functions
#include "PWM.h"		// For PWM
#include "I2CDevice.h"	// I2C devices
#include "util.h"		// Needed for PWM

using namespace exploringBB;
using namespace std;

#define TEMPSENS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"	// tmp36 Path changed from LDR
#define SLOTS "/sys/devices/bone_capemgr.9/slots"					// Our BBB has this loaded into .profile but other systems may not

// Declaring the variables
const int B = 4275;		// B value of thermistor from Grove sensor code
int count = 0;			// Set to read value to change state


/* Code from A TMP36 temperature sensor application
 * Conversion values from Grove temperature Sensor code*/

int readAnalog(int number){ 					// returns the input as an int
	stringstream ss;
	ss << TEMPSENS_PATH << number << "_raw";
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> number;
	fs.close();
	return number;
}


float getTemperature(int adc_value){
    float R = 4095.0/((float)adc_value)-1.0;	// Vcc = 5V for Temperature Sensor, Range = 0-4095
    R = 100000.0*R;
    float temperature = 1.0/(log(R/100000.0)/B+1/298.15)-273.15;
    return (temperature);
}


// Button Press with current time displayed, Need to get date to show***

int callbackFunction(int var){
	cout << "BBB Button Pressed!" << var << endl;
		if(count%2==0)
		{
			system("date");		// Add script at boot to get current time
		}
				count++;
				return var;
}

// Program runs from here
int main(int argc, char* argv[])
{

	if(system("echo BB-ADC > $SLOTS"))			// Check or Load DTO to enable Analog input for Temperature Sensor
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
		system("echo am33xx_pwm > $SLOTS");		// Enable hardware on SOC
	}


	// Enable Backlight and LCD
	// Declaring Character array variables for the LCD
	char row0 [16];					// Row 0 with 16 character array for LCD
	char row1 [16];					// Row 1 with 16 character array for LCD
	char tdc [5];					// TDC array to put Temperature Degrees C on end of Row 1

	I2CDevice BL(1, 0x62);			// Enable Backlight on I2C
	BL.open();
	BL.writeRegister(0x00, 0x00);	// Initialise registers x 3
	BL.writeRegister(0x08, 0xFF);
	BL.writeRegister(0x01, 0x20);
//	BL.writeRegister(0x02, 0x55);	// Backlight Blue medium
//	BL.writeRegister(0x03, 0x55);	// Backlight Green medium
//	BL.writeRegister(0x04, 0x55);	// Backlight Red medium		RGB together gives White Backlight, not used

	I2CDevice LCD(1,0x3E);			// Enable LCD on I2C
	LCD.open();
	LCD.writeRegister(0x80, 0x01);	// Clear screen
	LCD.writeRegister(0x80, 0x28);	// 2 line mode
	LCD.writeRegister(0x80, 0x0C);	// Switch on, No blinking
	LCD.writeRegister(0x80, 0x02);	// Cursor Home


	/* Rest of temperature code, added DTO BB-ADC to SLOTS code
	 * Outputs ADC and Temperature values to console */

	int ain = 0;
	cout << "Starting the Temperature Sensor program" << endl;
	if (argc>1) ain = atoi(argv[1]);
	int value = readAnalog(ain);
	cout << "The ADC value is: " << value << endl;
	float temperature = getTemperature(value);				// Get initial Temperature to see what state to enter
	cout << "The temperatures is: " << temperature << " degrees Celsius." << endl;


	// Enable GPIO for LED and Servo

	PWM pwm("pwm_test_P9_22.13");		// Creating PWM object for servo control

	GPIO outGPIO1(14), outGPIO2(2);		// 14 is LED and 2 is Servo

	outGPIO1.setDirection(OUTPUT);		// Output to LED
//	outGPIO1.setValue(LOW);				// LED Off			Does not need to be here

	outGPIO2.setDirection(OUTPUT);		// Output to Servo
//	outGPIO2.setValue(LOW);				// 					Does not need to be here

	pwm.setPeriod(20000000);			// Period in ns
	pwm.setPolarity(PWM::ACTIVE_HIGH);	// Set Polarity to 0


/*	Could not get it to work by this method, used PWM from Exploring BB
    system("echo 0 > run");				// Turn off PWM
    system("echo 0 > polarity");		// Set Pulse polarity
	system("echo 20000000 > period");	// Set Period = 20ms
*/

	// Had to put these in a while loop as when used in each state, got stuck in that state
	while(1)
	{
		LCD.writeRegister(0x80, 0x28);				// 2 line mode
		LCD.writeRegister(0x80, 0x0C);				// Switch on, No blinking
		LCD.writeRegister(0x80, 0x02);				// Cursor Home

		int value = readAnalog(ain);
		float temperature = getTemperature(value);	// Did not work within individual States


	// State 1 - COLD
	// LED Heat On and Vent Shut

	if ((value <= 1800)&&(count%2==0))				// 1800ADC = 20C. Change this value if you need to.
	{
		outGPIO1.setValue(HIGH);					// Turn LED On

		outGPIO2.setValue(LOW);						// My servo is fried, so used LED and trial and error to figure out Duty Cycles
		pwm.setDutyCycle(2.5f);						// 500000ns		LED Dim		Servo Min
		pwm.run();									// Start the output

/*
	    system("echo 0 > run");						// Turn off
//	    system("echo 0 > polarity");				// Already declared in GPIO Enable
//		system("echo 20000000 > period");			// Already declared in GPIO Enable
		system("echo 500000 > duty");				// Duty cycle to Close Vent
		system("echo 1 > run");						// Turn on
*/

/*
		int value = readAnalog(ain);
		float temperature = getTemperature(value);	// Does not work here, moved to while loop
*/

		// Output these to console
		cout << "STATE 1 - COLD" << endl;
		cout << "Heat On, Vent Shut" << endl;
		cout << "The temperatures is: " << temperature << " degrees Celsius." << endl;
		cout << "The ADC value is: " << value << endl;

//		LCD.writeRegister(0x80, 0x01);				// Clear screen
//		LCD.writeRegister(0x80, 0x02);				// Cursor Home.		Moved to while loop, does not work here
		BL.writeRegister(0x02, 0x99);				// Backlight Blue medium - Cold
		BL.writeRegister(0x03, 0x00);				// Backlight Green Off
		BL.writeRegister(0x04, 0x00);				// Backlight Red Off

		sprintf(row0, "HeatON .VentSHUT");			// Row 0 character array
		sprintf(row1, "Degrees C:");				// Row 1 character array text
		sprintf(tdc, "%0.1f", temperature);			// Convert and pass the float variable temperature to 1 decimal place to character array tdc

		int i=0;									// This loop passes text to the character array
		while(row0[i]!='\0')
			{
			LCD.writeRegister(0x40, row0[i]);		// Write text to Row 0 from character array in increments
			i++;
			}
		LCD.writeRegister(0x80, 0xC0);				// Set courser position to Row 1
		i=0;
		while(row1[i]!='\0')
			{
			LCD.writeRegister(0x40, row1[i]);		// Write text to Row 1 from character array in increments
			i++;
			}
		LCD.writeRegister(0x80, 0xCB);				// Set courser position to Row 1 cursor 13 to display temperature
		i=0;
		while(tdc[i]!='\0')
			{
			LCD.writeRegister(0x40, tdc[i]);		// Write temperature to Row 1 cursor 13 from character array in increments
			i++;
			}
	}


	// State 2 - GRAND
	// LED Heat Off and Vent Shut

	else if ((value >1800)&&(value <2050)&&(count%2==0))	// 1800-2050ADC = 20-25C. Change these values if you need to.
	{
		outGPIO1.setValue(LOW);						// LED Off

		outGPIO2.setValue(LOW);
		pwm.setDutyCycle(2.5f);						// 500000ns		LED Dim		Servo Min
		pwm.run();									// Start the output


		// Output these to console
		cout << "STATE 2 - GRAND" << endl;
		cout << "Heat Off, Vent Shut" << endl;
		cout << "The temperatures is: " << temperature << " degrees Celsius." << endl;
		cout << "The ADC value is: " << value << endl;

		// Output to LCD
//		LCD.writeRegister(0x80, 0x01);				// Clear screen
//		LCD.writeRegister(0x80, 0x02);				// Cursor Home.		Moved to while loop, does not work here
		BL.writeRegister(0x02, 0x00);				// Backlight Blue Off
		BL.writeRegister(0x03, 0x55);				// Backlight Green On
		BL.writeRegister(0x04, 0x00);				// Backlight Red Off

		sprintf(row0, "HeatOFF.VentSHUT");			// Row 0 character array
		sprintf(row1, "Degrees C:");				// Row 1 character array text
		sprintf(tdc, "%0.1f", temperature);			// Convert and pass the float variable temperature to 1 decimal place to character array tdc

		int i=0;									// This loop passes text to the character array
		while(row0[i]!='\0')
			{
			LCD.writeRegister(0x40, row0[i]);		// Write text to Row 0 from character array in increments
			i++;
			}
		LCD.writeRegister(0x80, 0xC0);				// Set courser position to Row 1
		i=0;
		while(row1[i]!='\0')
			{
			LCD.writeRegister(0x40, row1[i]);		// Write text to Row 1 from character array in increments
			i++;
			}
		LCD.writeRegister(0x80, 0xCB);				// Set courser position to Row 1 cursor 13 to display temperature
		i=0;
		while(tdc[i]!='\0')
			{
			LCD.writeRegister(0x40, tdc[i]);		// Write temperature to Row 1 cursor 13 from character array in increments
			i++;
			}
	}


		// State 3 - HOT
		// LED Heat Off and Vent Open

	else if ((value >=2050)&&(count%2==0))			// 2050ADC = 25C.  Change this value if you need to.
	{
		outGPIO1.setValue(LOW);						// LED Off

		outGPIO2.setValue(LOW);						// Vent Open
		pwm.setDutyCycle(10.5f);					// 2100000ns	LED Brighter	Servo Max
		pwm.run();									// Start the output


		// Output these to console
		cout << "STATE 3 - HOT" << endl;
		cout << "Heater Off, Vent Open" << endl;
		cout << "The temperatures is: " << temperature << " degrees Celsius." << endl;
		cout << "The ADC value is: " << value << endl;

		// Output to LCD
//		LCD.writeRegister(0x80, 0x01);				// Clear screen
//		LCD.writeRegister(0x80, 0x02);				// Cursor Home.		Moved to while loop, does not work here
		BL.writeRegister(0x02, 0x00);				// Backlight Blue Off
		BL.writeRegister(0x03, 0x00);				// Backlight Green Off
		BL.writeRegister(0x04, 0x55);				// Backlight Red On - HOT

		sprintf(row0, "HeatOFF.VentOPEN");			// Row 0 character array
		sprintf(row1, "Degrees C:");				// Row 1 character array
		sprintf(tdc, "%0.1f", temperature);			// Convert and pass the float variable temperature to 1 decimal place to character array tdc

		int i=0;									// This loop passes text to the character array
		while(row0[i]!='\0')
			{
			LCD.writeRegister(0x40, row0[i]);		// Write text to Row 0 from character array in increments
			i++;
			}
		LCD.writeRegister(0x80, 0xC0);				// Set courser position to Row 1
		i=0;
		while(row1[i]!='\0')
			{
			LCD.writeRegister(0x40, row1[i]);		// Write text to Row 1 from character array in increments
			i++;
			}
		LCD.writeRegister(0x80, 0xCB);				// Set courser position to Row 1 cursor 13 to display temperature
		i=0;
		while(tdc[i]!='\0')
			{
			LCD.writeRegister(0x40, tdc[i]);		// Write temperature to Row 1 cursor 13 from character array in increments
			i++;
			}

	}


	}
	return 0;

}
