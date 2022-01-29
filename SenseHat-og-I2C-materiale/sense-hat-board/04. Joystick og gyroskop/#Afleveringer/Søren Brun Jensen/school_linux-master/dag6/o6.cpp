#include <iostream>

using namespace std;

int main(){
	int year;
	cout << "Enter year: ";
	cin >> year;
	if(year == 0) cout << "Year 0 is theoreticaly not a leapyear." << endl;
	else cout << "\nThe year you entered is:" << (year % 4 == 0 ? "" : " not") << " a leapyear." << endl;
}