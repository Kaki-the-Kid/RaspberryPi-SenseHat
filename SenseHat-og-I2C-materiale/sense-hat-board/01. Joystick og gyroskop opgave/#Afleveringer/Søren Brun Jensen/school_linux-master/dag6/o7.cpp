#include <iostream>
#include <ctime>

using namespace std;

int main()
{
	srand(time(0));
	int guessed = 12, random;
	random = rand() % 11;
	cout << "Enter a number between 0 and 10: ";
	cin >> guessed;
	while(guessed != random)
	{
		if(guessed < 0 || guessed > 10)	cout << "Number is out of range!\nEnter a number between 0 and 10: ";
		else if(guessed < random) cout << "Try a higher number: ";
		else cout << "Try a lower number :";
		cin >> guessed;
	}
	cout << "You got it! The number was: " << random << endl;
}