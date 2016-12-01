#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<linux/i2c-dev.h>


int main(int argc, char* argv[]){

   int file;


   if(argc < 3)
   {
	printf("Need to pass Reg AND Cmd parameters\n");
	return(1);
   }

   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }

   if(ioctl(file, I2C_SLAVE, 0x3E) < 0){
      perror("Failed to connect to the LCD\n");
      return 1;
   }

   char writeBuffer[2] = {strtol(argv[1], NULL, 0), strtol(argv[2], NULL, 0)};

   printf("Reg and Cmd written: %s %s\n", argv[1], argv[2]);

   if(write(file, writeBuffer, 2)!=2){
      perror("Failed to write to CMD address\n");
      return 1;
   }


   return 0;
}
