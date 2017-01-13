/* A Simple GPIO application
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<iostream>
#include<unistd.h>
#include"GPIO.h"
//#include"mygpio.h" tried modifying to get count to pass by ref into callback..gave up and used global instead
#include"PWM.h"// sorry about that.
#include "I2CDevice.h"
#include<fstream>
#include<string>
#include<sstream>
#include <math.h>
#include <pthread.h>
#include <iomanip>
#include <linux/i2c-dev.h>
#include "I2CDevice.h"
#include <ctime>
using namespace exploringBB;
using namespace std;
#define TEMP_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"
#define SLOTS "/sys/devices/bone_capemgr.9/slots" //This line can be added in case SLOTS not set up on BBB.
const int B =4275;
int count=0;// couldn't do this any other way..
int readAnalog(int number){
   stringstream ss;
   ss << TEMP_PATH << number << "_raw";
   fstream fs;
   fs.open(ss.str().c_str(), fstream::in);
   fs >> number;
   fs.close();
   return number;}
/*void writeLCD(float temp)//gave up on this function.have to pass in buffer vals.
   {
	char buffer [16];
	char buffer2 [16];
	//float temperature;
	I2CDevice lcd(1,0x3E);
	    	   stringstream ss (stringstream::in | stringstream::out);
 	   	   ss << temp;
 	   	   string test = ss.str();
 	   	   sprintf(buffer2,"temp is:%s ",test );
 	   lcd.writeRegister(0x80,0x01);//clear screen
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
 	  	   		   		i++;
}
   }*/
int callbackFunction(int var){
			cout << "BBB Button Pressed!" << var << endl;
			if(count%2==0){
	system("/usr/sbin/ntpdate -b -s -u ie.pool.ntp.org");
	system("date");
			}
						count++;
						cout << "count in callback is" << count<< endl;
	return var;
}

int main(){
	char buffer [16];
	char buffer2 [16];
	char buffer3 [16];
//	time_t now = time(0);
//	tm *ltm = localtime(&now);
	sprintf(buffer3,"time is : teatime");//%d";,ltm->tm_hour.c_str());
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
	 lcd.writeRegister(0x80,0x08);//turn off
	 	rgb.writeRegister(0x00,0x00);
	 	rgb.writeRegister(0x08,0xFF);
	 	rgb.writeRegister(0x01,0x20);
	 	rgb.writeRegister(0x03,0x33);


		PWM pwm("pwm_test_P9_22.13");
		GPIO inGPIObut(5),outGPIO(14), inGPIO(2);//gpio(2) is J6 rx (SERV0 CONTROL INPUT), gpio(14) is j2 rx (LED CONTROL OUTPUT);
													//all I need is a dto to make i2c1 into gpio5! easy ..:)
		inGPIObut.setDirection(INPUT);
		inGPIObut.setEdgeType(RISING);
		outGPIO.setDirection(OUTPUT);
		outGPIO.setValue(LOW);
		cout << "GPIO(5) has value: " << inGPIObut.getValue() << "..0 means button not pressed" << endl;
		cout << "start with heater off:" << endl;

		 pwm.setPeriod(20000000);         // Set the period in ns, 20ms
   	     pwm.setDutyCycle(5.0f);       // Set the duty 5%
   	     pwm.setPolarity(PWM::ACTIVE_HIGH);
   	     cout << "start with vent closed:" << endl;// using active low PWM
   	     pwm.run();
   	   inGPIObut.setDebounceTime(2000);
   	    inGPIObut.waitForEdge(&callbackFunction);
   	   cout << "count in main is ********" << count<< endl;
   	   cout << "Poll Started: Press the button if you like:" << endl;
   	      while(1)
   {
   	    	  cout << " count is " << count << endl;
   	    	  cout << "Starting the readTHermistor program" << endl;
	  	  			   		usleep(1000000);
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
   	   stringstream stream;
   	   stream << fixed<< setprecision(1) << temperature;
   	   string s = stream.str();// try this as well to get float to string into buffer
   if (((value >1150)&&(value <1250)&&(count%2==0)))
   {
	   pwm.setDutyCycle(5.0f); //low duty cycle,9 o clock..
	   outGPIO.setValue(LOW);
	   cout << "heater still off:" << endl;
	   cout << "vent closed:" << endl;
	   sprintf(buffer,"heatoff,ventcls");
	  	  	   sprintf(buffer2, "temp is: %s",s.c_str());
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
	   		   		i++;
	   		   	 }
   }

      else if ((value <= 1150)&&(count%2==0))
   {
	   outGPIO.setValue(HIGH);
	   cout << "heater ON, vent CLOSED!" << endl;
	   sprintf(buffer," heatON,ventcls");
	  // writeLCD(temperature);
	   stringstream ss (stringstream::in | stringstream::out);
	   	   ss << temperature;
	   	   string test = ss.str();
	   	   sprintf(buffer2,"temp is: %s ",test.c_str() );
	   lcd.writeRegister(0x80,0x01);//clear screen
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
   else if ((value >= 1250)&&(count%2==0))
   {
	   outGPIO.setValue(LOW);
	   	   pwm.setDutyCycle(13.0f);//duty 13%, I couldn't get it to work by specifying ns, should have rewatched video
	   	     cout << "vent OPEN,heater OFF" << endl;// servo at 3 o clock i hope..
	   	  sprintf(buffer," heatOff,v open");
	   	 	   	  	   stringstream ss (stringstream::in | stringstream::out);
	   	  	   	   ss << temperature;
	   	  	   	   string test = ss.str();
	   	  	   	   sprintf(buffer2,"temp is: %s ",test.c_str() );
	   	  	   	  cout << "value in buffer2 is " << test<< endl;
	   	  	   lcd.writeRegister(0x80,0x01);//clear screen
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
	   	  	  	   		   		i++;
	   	  	  	   		   	 }

	   	  	  	   		   	 }
   else
   {rgb.writeRegister(0x00,0x00);
	rgb.writeRegister(0x08,0xFF);
	rgb.writeRegister(0x01,0x20);
	rgb.writeRegister(0x02,0x33);
	lcd.writeRegister(0x80,0x01);//clear screen
	 	   	  	   lcd.writeRegister(0x80,0x80);//row 0, col 0
	 	   	  	  	   	 int i=0;
	 	   	  	  	   	 while(buffer3[i]!='\0')
	 	   	  	  	   	 {
	 	   	  	  	   		lcd.writeRegister(0x40,buffer3[i]);//buffer one by one
	 	   	  	  	   		i++;
	 	   	  	  	   	 }
	 	   	  	  	rgb.writeRegister(0x02,0x00);
   }
         }
   return 0;
}
