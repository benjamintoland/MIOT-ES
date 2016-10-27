#include <stdio.h>
#include <unistd.h>

#define GPIO_PATH "/sys/class/gpio"
#define LED_NUMBER 49
#define LED_DIRECTORY "gpio49"

void setUpLedPin(void);
void writeValue(int value);

int main(int argc, char* argv[])
{
	setUpLedPin();

	if(argc < 2) {
		printf("One argument required.  One of:\n");
		printf("   on, off, flash\n");
		return 2;
	}

	if(strcmp(argv[1], "on") == 0){
		printf("Turning on LED\n");
		writeValue(1);
	}
	else if(strcmp(argv[1], "off") == 0){
		printf("Turning off LED\n");
		writeValue(0);
	}
	else{
		printf("Invalid command\n");
	}

	return 0;
}

void writeValue(int value)
{
	FILE *fp;
	char fullFileName[100];

	sprintf(fullFileName,"%s/%s/value", GPIO_PATH, LED_DIRECTORY);
	printf("%s\n", fullFileName);	
	fp = fopen(fullFileName, "w");
	fprintf(fp, "%i", value);
	fclose(fp);
}

void setUpLedPin(void)
{
	FILE *fp;
	char fullFileName[100];

	sprintf(fullFileName, "%s/export", GPIO_PATH);
	printf("%s\n", fullFileName);

	fp = fopen(fullFileName, "w");
	fprintf(fp, "%i", LED_NUMBER);
	fclose(fp);

	sprintf(fullFileName, "%s/%s/direction", GPIO_PATH, LED_DIRECTORY);
	printf("%s\n", fullFileName);
	
	fp = fopen(fullFileName, "w");
	fprintf(fp, "%s", "out");
	fclose(fp);

}

