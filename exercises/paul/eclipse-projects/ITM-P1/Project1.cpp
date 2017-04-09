/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.
* Modified April 2017 by Paul McArdle
* Application to read temperature, hostname and time stamp and output to Couch DB*/

#include <iostream>
#include <unistd.h>
#include <fstream>			// tmp36
#include <string>			// tmp36
#include <sstream>			// tmp36
#include <stdlib.h>			// tmp36 and curl
#include <stdio.h>			// curl
#include <math.h>			// Grove Temperature sensor
#include <pthread.h>
#include <iomanip>
#include "GPIO.h"			// GPIO functions
#include <string>
using namespace exploringBB;
using namespace std;

#define TEMPSENS_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"	// TMP36 Path changed from LDR
#define SLOTS "/sys/devices/bone_capemgr.9/slots"					// Our BBB has this loaded into .profile but other systems may not

// Declaring the variables

const int B = 4275;								// B value of thermistor from Grove sensor code
char datas [1000];								// Power On status and Unit ID
char datatt [1000];								// Temperature, time stamp and Unit ID
char statuscode[] = "On";						// Power On status code
//char id[] = "BBBPMA";							// Unit ID with fixed value - no longer needed - code added to read device hostname
char hostname [1024];							// Unit ID now reads devices hostname
char timeinfo [sizeof "2017-04-02T06:07:08Z"];	// To set time format and size to ISO8601


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


// Main Program runs from here

int main(int argc, char* argv[])
{

	// Check or Load DTO to enable Analog input for Temperature Sensor

	if(system("echo BB-ADC > $SLOTS"))
	{
		cout << "DTO BB-ADC for Analog inputs already loaded\n" << endl;
	}
	else
	{
		cout << "DTO BB-ADC for Analog inputs loaded\n" << endl;
	}


	// Get Hostname of Device

	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	cout << "Device On with Unit ID: " << hostname << endl;		// Output to console - hostname of device


	// To get time in ISO8601 format, UTC

    time_t now;
    time(&now);
    strftime(timeinfo, sizeof timeinfo, "%FT%TZ", gmtime(&now));
	cout << "UTC Time is: " << timeinfo << endl;				// Output to console Time and Date
//	system("date");												// Check Time and Date using system function - removed after testing


	// Post to database - Unit ID and Power up status code

	sprintf(datas, "curl -X POST -H \"Content-Type: application/json\" http://178.62.29.184/db/paul_db_a1 -d'{\"Unit_ID\":\"\%s\", \"Time_Stamp\":\"\%s\", \"Status\":\"\%s\"}'", hostname, timeinfo, statuscode);
	system(datas);
	usleep(30000000);								// Usleep to pause program for 30 seconds to allow time between initial Post and subsequent temperature Post


	/* 5 minute interval loop to read Time Stamp and Temperature then
	 * Using curl Post to database - Unit ID, Time Stamp and Temperature */

	while(1)
	{


	 /*Rest of temperature code, added DTO BB-ADC to SLOTS code
	 * Outputs ADC and Temperature values to console */

	int ain = 0;
	cout << "Starting the Temperature Sensor program" << endl;
	if (argc>1) ain = atoi(argv[1]);
	int value = readAnalog(ain);
	cout << "The ADC value is: " << value << endl;														// Output to console - ADC value
	float temperature = getTemperature(value);
	cout << "The temperature is: " << setprecision(3) << temperature << " degrees Celsius." << endl;	// Output to console - Temperature to 1 decimal place


	// To get time in ISO8601 format, UTC

    time_t now;
    time(&now);
    strftime(timeinfo, sizeof timeinfo, "%FT%TZ", gmtime(&now));
	cout << "UTC Time is: " << timeinfo << endl;				// Output to console Time and Date

	// Using curl check connection to CouchDB database

//	cout << "\r\nGoing to call curl\r\n" << endl;
//	system("curl http://178.62.29.184/db/");		// Removed after initial connection test


	// Using curl Post to database - Unit ID, Time Stamp and Temperature

	sprintf(datatt, "curl -X POST -H \"Content-Type: application/json\" http://178.62.29.184/db/paul_db_a1 -d'{\"Unit_ID\":\"\%s\", \"Time_Stamp\":\"\%s\", \"Temperature_C\":\"\%0.1f\"}'", hostname, timeinfo, temperature);
	system(datatt);

	usleep(300000000);								// Usleep to loop program every 5 minutes

	}


	return 0;

}
