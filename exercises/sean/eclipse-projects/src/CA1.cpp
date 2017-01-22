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
#include<fstream>
#include<string>
#include<sstream>
#include"I2CDevice.h"
#include <math.h>

using namespace std;
using namespace exploringBB;

#define Temp_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

int buttonCount = 0;

int callbackFunction(int var){

	cout << "BBB Button Pressed!" << var << endl;
	buttonCount = buttonCount + 1;

	if(buttonCount == 2){
		//display the date and time
		system("date");
		buttonCount =0;
	}


	return var;
}

int readAnalog(int number) { // returns the input as an int
	stringstream ss;
	ss << Temp_PATH << number << "_raw";
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> number;
	fs.close();
	return number;
}


int main() {

	I2CDevice back(1, 0x62);			// Enable Backlight
	back.open();
	back.writeRegister(0x00, 0x00);	// Initialise registers
	back.writeRegister(0x08, 0xff);
	back.writeRegister(0x01, 0x20);
	back.writeRegister(0x02, 0x60);	// Backlight Blue
	back.writeRegister(0x03, 0x60);	// Backlight Green
	back.writeRegister(0x04, 0x60);	// Backlight Red

	I2CDevice lcd(1,0x3e);			// Enable LCD
	lcd.open();
	lcd.writeRegister(0x80, 0x01);	// Clear screen
	lcd.writeRegister(0x80, 0x28);	// 2 line mode
	lcd.writeRegister(0x80, 0x0f);	// Switch on with blinking
	lcd.writeRegister(0x80, 0x02);	// Cursor Home

	int analogueIn = 0;
	//load the ADC Device tree overlay
	system("echo BB-ADC > $SLOTS");

	//set up GPIO pins.
	GPIO inGPIO(14); //Formerly 48 - SET to 14 to take in RX signal of grove cape.
	GPIO outGPIO(2); //Formerly 60  SET to 2 to take in RX signal of grove cape.


	inGPIO.setDirection(INPUT);
	inGPIO.setEdgeType(RISING);

	outGPIO.setDirection(OUTPUT);
	outGPIO.setValue(LOW);
	usleep(500000);


	inGPIO.setDebounceTime(200);
	inGPIO.waitForEdge(&callbackFunction);

	while(1)
	{
		usleep(50000);
		int value = readAnalog(analogueIn);
			cout << "The ADC value is: " << value << endl;
			float R = 1023.0/((float)value)-1.0;
			 R = 100000.0*R;
			 //convert to temperature via formula given with by supplier EG. http://wiki.seeed.cc/Grove-Temperature_Sensor_V1.2/
			float temperature=1.0/(log(R/100000.0)/4275+1/298.15)-273.15;
			cout << "The temperature is :" << temperature << endl;

			if(value<=415){
				cout << "Alarm. Temperature below minimum threshold turning heater ON";
				outGPIO.setValue(HIGH);
			}

			if(value>415){
				outGPIO.setValue(LOW);
			}

			if(value>420){
				cout << "Alarm. Temperature above maximum threshold turning vent ON";
			}
	}




}



