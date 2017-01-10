#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv)
{
	if(system("echo bone_pwm_P9_22 > $SLOTS"))
	{
		printf("P9_22 PWM Already loaded!\n");
	}
	else
	{
		printf("P9_22 PWM Loaded!\n");
		system("echo am33xx_pwm > $SLOTS");
	}

	return 0;
}

