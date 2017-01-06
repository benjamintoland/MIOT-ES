/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<unistd.h>
#include"GPIO.h"
#include"PWM.h"
#include "I2CDevice.h"
#include<fstream>
#include<string>
#include<sstream>
#include <math.h>
#include <pthread.h>
//#include<sys/ioctl.h>
#include<linux/i2c-dev.h>
//#include<fcntl.h>
#include "I2CDevice.h"
using namespace exploringBB;
using namespace std;
#define TEMP_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define SLOTS "/sys/devices/bone_capemgr.9/slots" //This line can be added in case SLOTS not set up on BBB.
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
int callbackFunction(int var){
	cout << "BBB Button Pressed!" << var << endl;
	system("/usr/sbin/ntpdate -b -s -u ie.pool.ntp.org");
		   		 	   					system("date");
				   						return var;
}

int main(){
	char buffer [16];
	char buffer2 [16];

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
		system("echo i2c1togpio > $SLOTS");
	}
	 I2CDevice lcd(1,0x3E);//messing around to figure out
	 I2CDevice rgb(1,0x62);
	 lcd.open();
	 rgb.open();
	 lcd.writeRegister(0x80,0x01);//clear screen
	 lcd.writeRegister(0x80,0x0F);//on with blinking
	 lcd.writeRegister(0x80,0x28);//two line mode
		 	 usleep (100000);
	 lcd.writeRegister(0x80,0xC0);//second row
	 lcd.writeRegister(0x40,102);//f

	 usleep(1000000);
	 	 	 usleep(1000000);
	 	lcd.writeRegister(0x80,0x08);//turn off

	 	rgb.writeRegister(0x00,0x00);
	 	rgb.writeRegister(0x08,0xFF);
	 	rgb.writeRegister(0x01,0x20);
	 	rgb.writeRegister(0x03,0x3F);
	 		 	usleep(1000000);
	 		 	rgb.writeRegister(0x03,0x00);
		PWM pwm("pwm_test_P9_22.13");
		GPIO inGPIObut(5),outGPIO(14), inGPIO(2);//gpio(2) is J6 rx (SERV0 CONTROL INPUT), gpio(14) is j2 rx (LED CONTROL OUTPUT);
													//all I need is a dto to make i2c1 into gpio5! easy ..:)
		//GPIO outGPIO(4);

		inGPIObut.setDirection(INPUT);
		inGPIObut.setEdgeType(RISING);
		outGPIO.setDirection(OUTPUT);
		outGPIO.setValue(LOW);

		//outGPIO.setDirection(OUTPUT); not needed

		cout << "GPIO(5) has value: " << inGPIObut.getValue() << "..0 means button not pressed" << endl;
		cout << "start with heater off:" << endl;

		 pwm.setPeriod(20000000);         // Set the period in ns, 20ms
   	     pwm.setDutyCycle(5.0f);       // Set the duty 5%
   	     pwm.setPolarity(PWM::ACTIVE_HIGH);
   	     cout << "start with vent closed:" << endl;// using active low PWM
   	     pwm.run();
   	   inGPIObut.setDebounceTime(2000);
   	   inGPIObut.waitForEdge(&callbackFunction);
     		 cout << "Poll Started: Press the button if you like:" << endl;
   	      while(1)
   {
	   cout << "Starting the readTHermistor program" << endl;
	  	  			   		usleep(5000000);
	  	   lcd.writeRegister(0x80,0x0F);//on with blinking
	  	   lcd.writeRegister(0x80,0x28);//two line mode
	  	   cout << "Finished sleeping for 5 seconds" << endl;
	   	  	   	  int value = readAnalog(0);
   cout << "The Thermistor value was " << value << " out of 4095." << endl;
   float R= 4095.0/((float)value)-1.0;
   R = 100000.0*R;
   float temperature =1.0/(log(R/100000.0)/B+1/298.15)-263.15;// convert an val to temp C.
          R = 100000.0*R;
   cout << "Temperature is : " << temperature << "Degrees Celcius" << endl;
   usleep(50000);
   if ((value >1150)&&(value <1250))//set range so easy to test by putting finger on thermistor to heat up, ice cube to cool.
   {
	   pwm.setDutyCycle(5.0f); //low duty cycle,9 o clock..
	   outGPIO.setValue(LOW);
	   cout << "heater still off:" << endl;
	   cout << "vent closed:" << endl;
	   sprintf(buffer,"heatoff,ventcls");
	   stringstream ss (stringstream::in | stringstream::out);
	   ss << temperature;
	   string test = ss.str();
	   sprintf(buffer2, test);
	   cout << "Test value in buffer2 :" <<test << endl;
	  //buffer2=temperature;
	  	  //	 lcd.writeRegister(0x80,0x0F);//on with blinking
	   //	 lcd.writeRegister(0x80,0x28);//two line mode
	   	 lcd.writeRegister(0x80,0x80);//row 0, col 0
	   	 int i=0;
	   	 while(buffer[i]!='\0')
	   	 {
	   		lcd.writeRegister(0x40,buffer[i]);//buffer one by one
	   		i++;
	   	 }
	   	lcd.writeRegister(0x80,0xC0);//row 1, col 0
	   		   	 i=0;
	   		   	 while(buffer2[i]!='\0')
	   		   	 {
	   		   		lcd.writeRegister(0x40,buffer2[i]);//buffer one by one
	   		   		i++;}
	 }

   else if (value <= 1150)
   {
	   outGPIO.setValue(HIGH);
	   cout << "heater ON, vent CLOSED!" << endl;
   }
   else if (value >= 1250)
   {
	   outGPIO.setValue(LOW);
	   	   pwm.setDutyCycle(13.0f);//duty 13%, I couldn't get it to work by specifying ns, should have rewatched video
	   	     cout << "vent OPEN,heater OFF" << endl;// servo at 3 o clock i hope..
   }


   /* 	  inGPIObut.setDebounceTime(200);
 	 int count= inGPIObut.waitForEdge(&callbackFunction);
 	   		count+=1;
 	   		repcount +=count;
 	   	  cout << "Poll Started: Press the button:" << endl;
 	   	   usleep(10000000);
 	   	   cout << "Finished sleeping for 10 seconds" << endl;
 	   	if(repcount%2 ==0)
 	   		{
 	   			system("/usr/sbin/ntpdate -b -s -u ie.pool.ntp.org");
 	   					system("date");
 	   					usleep(500000);
 	   					 	   		}

 	   		else
 	   		{
 	   			cout << "The temp is:??? *****************" << endl;
 	   			 	   		}
}*/
   }
   return 0;

}
