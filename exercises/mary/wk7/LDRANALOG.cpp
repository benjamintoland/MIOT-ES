/*C++ to use analog input 0 with ldr attached to control LED ON PIN 49
*
*d 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<sstream>
#include<string>
#include<unistd.h>
#include<cstring>
#include<fstream>
using namespace std;
#define GPIO_PATH  "/sys/class/gpio"
#define SLOTS "/sys/devices/bone_capemgr.9/slots"
#define ANALOG_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
void writeGPIO(string filename,string value)
{

	fstream fs;
	string path(GPIO_PATH);
	fs.open((path + filename).c_str(),fstream::out);
	fs<< value;
cout << "written value is : " << value<< endl; 
	fs.close();
}
void writeADC(string value)
{
	fstream fs;
	string path(SLOTS);
	fs.open((path).c_str(),fstream::out);
	fs << value;
cout << "BB-ADC sent to SLOTS" << endl;
	fs.close();
}
//function to read value at  Analog pin 0
int readANALOG(string filename)
{

	int val;
//	fstream fs;
	string path(ANALOG_PATH);

ifstream fs((path).c_str(),ifstream::in);
cout << "read filename is: " << path << "\n"; 
	if(!fs) {
	cout<< "Cannot open file.\n";
	return 1;}
	fs >> val;
	cout<< "read value is : " << val<< endl;	
	return (val);
	fs.close();
}
int main(int argc, char* argv[]){
   if(argc!=1){
	cout << "Usage is ./LDRANALOG " << endl;;
              return 2;
   }
   cout << "Starting the SWITCHLED program" << endl;
   printf("The current gpio49 Path is: " GPIO_PATH "\n");
		writeADC("BB-ADC");//write BB-ADC to SLOTS
	writeGPIO("/unexport","49");
	writeGPIO("/unexport","115");
        writeGPIO("/export", "49");//unexport and then export the 2 pins
	writeGPIO("/export","115");
	writeGPIO("/gpio49/direction", "out");//set directions for pins
	writeGPIO("/gpio115/direction","in");	
cout << "all good file writing" << endl;
	//infinite loop
   while(1)
{
int anvalue;
anvalue = readANALOG(ANALOG_PATH);
cout << "read  ldr  value in main is : " << anvalue << endl;
   if(anvalue < 600){
       	writeGPIO("/gpio49/value", "1");
   }
	else writeGPIO("/gpio49/value", "0");
usleep(200000);
   }
   return 0;
}
