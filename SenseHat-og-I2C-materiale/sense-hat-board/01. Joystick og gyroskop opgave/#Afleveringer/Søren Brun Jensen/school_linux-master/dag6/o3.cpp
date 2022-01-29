#include <iostream>

using namespace std;

string phrase = "Meaning of liofe the universe and everything = 42";
const string wrongWord = "liofe";

int main()
{
	size_t start = phrase.find(wrongWord);
	phrase.replace(start, wrongWord.length(), "life,");
	cout << phrase << endl;
}