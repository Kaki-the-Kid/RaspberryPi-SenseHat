#include <stdio.h>

#define NUM_SENSORS 3
#define NUM_LOGS 3

int data[NUM_LOGS][NUM_SENSORS];

int main()
{
	data[0][0] = 25;
	data[0][1] = 63;
	data[0][2] = 807;

	data[1][0] = 26;
	data[1][1] = 67;
	data[1][2] = 1030;

	data[2][0] = 24;
	data[2][1] = 45;
	data[2][2] = 998;

	for(int i = 0; i < NUM_LOGS; i++)
	{
		for(int j = 0; j < NUM_SENSORS; j++)
		{
			printf("Sensor %d: %d", j, data[i][j]);
			if(j < 2) printf("\t");
			if(j == 2) printf("\n");
		}
	}
	
}
