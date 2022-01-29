#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>

using namespace std;

float do_comma_stuff(string s)
{
	size_t found = s.find(',');
	string period = ".";
	s.replace(found, 1, period);
	return atof(s.c_str());
}

string stringify_float(float f)
{
	int integerPart = static_cast<int>(f);
	int fractionPart = static_cast<int>((f - integerPart) * 100.0f);
	string s = to_string(integerPart);
	s += ",";
	s += to_string(fractionPart);
	return s;
}

int main()
{
	vector<pair<string,float>> item;
	string lastItem = "", sPrice = "";
	float lastPrice, totalPrice = 0.00f;
	cout << "Indtast varer og pris. Skriv stop når du ikke vil have flere varer\n" << endl;
	while(lastItem != "stop")
	{
		cout << "Indtast en vare: ";
		cin >> lastItem;
		if(lastItem == "stop") break;
		cout << "Indtast varens pris: ";
		cin >> sPrice;
		item.push_back(make_pair(lastItem, do_comma_stuff(sPrice)));
		cout << endl;
	}

	cout << "\n\nBon:\n" << endl;

	for(auto x : item)
	{
		cout << x.first << "\t\t\t" << stringify_float(x.second) << endl;
		totalPrice += x.second;
	}

	cout << "\nTotal:\t\t\t" << stringify_float(totalPrice) << "Kr." << endl;
	cout << "\tHeraf moms:\t" << stringify_float(totalPrice * 0.2) << "Kr." << endl; 
}