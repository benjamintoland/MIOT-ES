/* A Simple GPIO application
 * Written by Derek Molloy for the book "Exploring BeagleBone: Tools and
 * Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
 * ISBN 9781118935125. Please see the file README.md in the repository root
 * directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<unistd.h> //for usleep
#include"GPIO.h"
#include"I2CDevice.h"
#include"PWM.h"
using namespace exploringBB;
using namespace std;

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>
using namespace std;

#define Temp_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

GPIO outGPIO(14), inGPIO(2); // temp sensor connected to GPIO14, Servo GPIO2

int readAnalog(int number) { // returns the input as an int
	stringstream ss;
	ss << Temp_PATH << number << "_raw";
	fstream fs;
	fs.open(ss.str().c_str(), fstream::in);
	fs >> number;
	fs.close();
	return number;
}

float getTemperature(int adc_value) {
	float cur_voltage = adc_value * (1.80f / 4096.0f);
	float diff_degreesC = (cur_voltage - 0.75f) / 0.01f;
	return (25.0f + diff_degreesC);
}

int main(int argc, char* argv[]) {
	int ain = 0;

	//load the ADC DTO
	system("echo BB-ADC > $SLOTS");


	outGPIO.setDirection(OUTPUT);

	cout << "Starting the TMP36 temperature sensor program" << endl;

	//if (argc>1) ain = atoi(argv[1]);

	//Next turn on light if < given value this simulates turning on a heater

	for (int i = 0; i < 10; i++) {
		int value = readAnalog(ain);
		cout << "The ADC value is: " << value << endl;
		//float temperature = getTemperature(value);
		//cout << "The temperatures is: " << temperature << " degrees Celsius." << endl;


		if (value < 431) { //light LED if Temp < 431
			outGPIO.setValue(HIGH);
		} else {
			outGPIO.setValue(LOW);

		}
//next part servo to open vent
		system("echo bone_pwm_P9_22 > $SLOTS");
		system("echo am33xx_pwm > $SLOTS");


		for (int i = 0; i < 10; i++) { //run 10 times
				int value = readAnalog(ain);
				cout << "The Vent is open: " << value << endl;

		PWM pwm("pwm_test_P9_22.15");
		pwm.setPeriod(2000000);
		pwm.setDutyCycle(50.0f);
		pwm.setPolarity(PWM::ACTIVE_LOW);
		pwm.run();

// output data on LCD

		system("echo i2c1togpio > $SLOTS"); //DTO where did this come from?
		}
		 I2CDevice lcd(1,0x3E);//messing around to figure out fix
		 I2CDevice rgb(1,0x62);
		 lcd.open();
		 rgb.open();
		 lcd.writeRegister(0x80,0x01);//clear screen
		 lcd.writeRegister(0x80,0x0F);//on with blinking
		 lcd.writeRegister(0x80,0x28);//two line mode
		 	 usleep (100000);
		 lcd.writeRegister(0x80,0x08);//turn off
		 	rgb.writeRegister(0x00,0x00);
		 	rgb.writeRegister(0x08,0xFF);
		 	rgb.writeRegister(0x01,0x20);
		 	rgb.writeRegister(0x03,0x33);

		}
		sleep(1);
	}
	return 0;
}

