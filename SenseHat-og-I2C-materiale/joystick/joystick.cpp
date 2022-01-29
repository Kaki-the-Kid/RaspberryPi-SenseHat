#include <SenseHat.h>
#include <iostream>
#include <string>
#include "stdio.h"
#include <vector>
//#include "conio.h"

using namespace std;

int main()
{
    SenseHat sense;

    int temperature =  sense.GetTemperature();
    int dir=0;
    string keyIn;
    cout << temperature << endl;
    string first;
    //char escapeButton = 0x13;
    while(1)
    {
        dir = sense.ScannerJoystick();
        //dir = sense.stick.event();
        //scanf("%c", &first);
        cin >> first;
        if(keyIn == "keyIn"){;}
        
        printf("**%d**\n", std::stoi(first));
        //cout <<  << (std::) <<"**" <<endl;
        
        //sleep(1);
    }


}
