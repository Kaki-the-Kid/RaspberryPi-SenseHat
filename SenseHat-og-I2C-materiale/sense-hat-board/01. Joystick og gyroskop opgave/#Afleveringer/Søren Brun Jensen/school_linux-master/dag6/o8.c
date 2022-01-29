#include <stdio.h>

char *navne[10] = {"Stan Marsh", 
				   "Eric Cartman", 
				   "Kenny McKormik", 
				   "Randy Marsh", 
				   "Kyle Broflovski", 
				   "Mr. Garrison", 
				   "Butters Stotch", 
				   "Mr. Slave", 
				   "Lemingwinks", 
				   "Mr. Hat"};

int main()
{
	printf("Index 4 in names is: %s\n", navne[3]);
}