#include <SenseHat.h>
#include <string>
#include <iostream>
//#include <ncurses.h>


int Main()
{
	char unit[] = "stick";
	
	SenseHat sense
	
	SenseHat.operator <<	if(open_fbdev("stick"))
		printf("Fandt enheden %s", unit);
	else
		printf("Ingen enheder");
}
