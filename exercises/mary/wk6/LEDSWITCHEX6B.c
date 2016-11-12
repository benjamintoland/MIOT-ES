/* Modify flashing led usr3 to gpioflash teh off- board led etc.
*
* Written by Derek Molloy for the book "Exploring BeagleBone: Tools and 
* Techniques for Building with Embedded Linux" by John Wiley & Sons, 2014
* ISBN 9781118935125. Please see the file README.md in the repository root 
* directory for copyright and GNU GPLv3 license information.            */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#define GPIO_PATH  "/sys/class/gpio"
int i;
char value;
int pinvalue;
void writeGPIO(char filename[], char value[]);
int readGPIO(char filename[]);
int main(int argc, char* argv[]){
   if(argc!=1){
	printf("Usage is ./LEDSWITCHEX6B\n");
              return 2;
   }
   printf("Starting the SWITCHLED program\n");
   printf("The current gpio49 Path is: " GPIO_PATH "\n");
	writeGPIO("/unexport","49");
	writeGPIO("/unexport","115");
        writeGPIO("/export", "49");//unexport and then export the 2 pins
	writeGPIO("/export","115");
	writeGPIO("/gpio49/direction", "out");//set directions for pins
	writeGPIO("/gpio115/direction","in");	
	//infinite loop
   while(1)
{
pinvalue = readGPIO("/gpio115/value");
   if(pinvalue==1){
       	writeGPIO("/gpio49/value", "1");
   }
	else writeGPIO("/gpio49/value", "0");
usleep(200000);
   }
   return 0;
}
//func to write to files
void writeGPIO(char filename[], char value[]){
   FILE* fp;   // create a file pointer fp
   char  fullFileName[100];  // to store the path and filename
   sprintf(fullFileName,"%s%s", GPIO_PATH, filename); // write path and filename
//printf(" fullfile name is '%s%s'\n:",GPIO_PATH,filename);  
 fp = fopen(fullFileName, "w"); // open file for writing
   fprintf(fp, "%s", value);  // send the value to the file
fscanf(fp,"%s",value);
//printf("the scanned value is '%s':\n",value);
   fclose(fp);  // close the file using the file pointer

}
//function to read value at  pin 115
int readGPIO(char filename[]){
   FILE* fp;
	int val;
   char fullFileName[100];
   sprintf(fullFileName,"%s%s",GPIO_PATH,filename);
printf("file reading from is '%s%s':\n",GPIO_PATH,filename);
 if ((fp = fopen(fullFileName,"r"))==NULL)
puts("ERROR OPENING FILE");
else{puts("all good with fileopen");
   fscanf(fp,"%d",&val);
//printf("scanned value is '%d':\n",val);
fclose (fp);
return(val);
}}

