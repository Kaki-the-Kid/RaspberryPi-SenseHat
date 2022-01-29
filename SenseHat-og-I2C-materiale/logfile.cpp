#include "SenseHat.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	FILE *f;
	char dateTime[] = "28/01/2020 - 13:35\n";
	string dateTimeString = "28/01/2020 - 13:35\n";
	char temperature[] = "26.7";
	char humidity[] = "45";
	char barometer[] = "450";
	int count = 0;
	string line;
	string dataTxt;


	ifstream file("x.log");
	while (getline(file, line))
		count++;
		
	cout << "Eksisterende linier i filen: " << count << endl;
	count=0;
	
	int i=10;
	f = fopen("x.log", "a+");
	while(i > 0)
	{
		if(f == NULL)
		{
			cout << "Somthing went terribly wrong";
		}
		else
		{
			//fprintf(f, "%s,\t%s,\t%s,\t%s\n", dateTimeString, temperature, humidity, barometer);
			
			dateTimeString.pop_back();
			cout << dateTimeString  << ",\t "<< temperature << ",\t "<<  humidity << ",\t "<<  barometer << "\n";
		}
		
		
		i--;
		
	}
	
	fclose(f);
	
	
	//ifstream myFileData("x.log");
	//while(getline(myFileData, dataTxt))
	//{
	//	cout << dataTxt << endl;
	//}
	
	//myFileData.close();
	
	return(0);
}
