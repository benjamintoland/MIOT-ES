
/** Simple On-board LED flashing program - written in C by Derek Molloy
*    simple functional struture for the Exploring BeagleBone book
*
*    This program uses USR LED 3 and can be executed in three ways:
*         makeLED on
*         makeLED off
*         makeLED flash  (flash at 100ms intervals - on 50ms/off 50ms)
*         makeLED status (get the trigger status)
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define REDLED_PATH "/sys/class/leds/beaglebone:green:usr3"
#define LED_NUMBER 49
#define LED_DIRECTORY "gpio49"

void writeGPIO(char filename[], char value[]);
void setUpLedpin(void);

int main(int argc, char* argv[]){
   if(argc!=2){
	printf("Usage is makeLEDC and one of:\n");
        printf("   on, off, flash or status\n");
	printf(" e.g. makeLED flash\n");
        return 2;
   }
   printf("Starting the makeLED program\n");
   printf("The current LED Path is: " REDLED_PATH "\n");
	writeGPIO("/unexport","49");
	writeGPIO("/export","49");
	writeGPIO("/gpio49/direction","out");
   printf("gpio direction is out");

   // select whether command is on, off, flash or status
   if(strcmp(argv[1],"on")==0){
        printf("Turning the LED on\n");
        writeGPIO("/gpio49/value", "1");
   }
   else if (strcmp(argv[1],"off")==0){
        printf("Turning the LED off\n");

        writeGPIO("/gpio49/value", "0");
   }
   else if (strcmp(argv[1],"flash")==0){
        printf("Flashing the LED\n");
        writeGPIO("/trigger", "timer");
        writeGPIO("/delay_on", "50");
        writeGPIO("/delay_off", "50");
   }
   else if (strcmp(argv[1],"status")==0){
      FILE* fp;   // see writeLED function below for description
      char  fullFileName[100];  
      char line[80];
      sprintf(fullFileName, REDLED_PATH "/trigger"); 
      fp = fopen(fullFileName, "r"); //reading text this time
      while (fgets(line, 80, fp) != NULL){
         printf("%s", line);
      }
      fclose(fp);  
   }  
   else{
	printf("Invalid command!\n");
   }
   printf("Finished the makeLED Program\n");
   return 0;
}

void writeGPIO(char filename[], char value[])
{
   FILE* fp;   // create a file pointer fp
   char  fullFileName[100];  // to store the path and filename

   sprintf(fullFileName,"%s/%s/value", REDLED_PATH, LED_DIRECTORY); // write path and filename
   printf("%s\n", fullFileName);
   fp = fopen(fullFileName, "w"); // open file for writing
   fprintf(fp, "%i", value);  // send the value to the file
   fclose(fp);  // close the file using the file pointer
}

void setUpLedPin(void)
{
	FILE *fp;
	char fullFileName[100];

	sprintf(fullFileName, "%s/export", REDLED_PATH);
	printf("%s\n", fullFileName);

	fp = fopen(fullFileName, "w");
	fprintf(fp, "%i", LED_NUMBER);
	fclose(fp);

	sprintf(fullFileName, "%s/%s/direction", REDLED_PATH, LED_DIRECTORY);
	printf("%s\n", fullFileName);
	
	fp = fopen(fullFileName, "w");
	fprintf(fp, "%s", "out");
	fclose(fp);

}
