# Anders Chr Rungborg
# 17-04-2020
# datalogger

import sys                                                                                 
import time                                                                                # timmer 
import datetime                                                                            # dato
from sense_hat import SenseHat

Temp = input("lok-data" + "\n" + "...")
sense = SenseHat()
hat = SenseHat()
test = "test.csv"                                                                           # opset text fil som .com


if (Temp) == "data":                                                                        # siger at data atver log
    timer = 10                                                                              # hantehed på timer = 10cek
    while timer > 0:                                                                        # tiller ned til 0 i lube 
    
        t = hat.get_temperature()                                                           # tempertur data
        h = hat.get_humidity()                                                              # luftfut data 
        p = hat.get_pressure()                                                              # pressure data

        print ((datetime.datetime.utcnow().strftime("%d-%m-%Y %H:%M:%S")),
        'Temp C:{:.2f} Humd:{:.0f} Pres:{:.0f}' .format(t, h, p ))                          # skiver data ude
        
        with open(test, 'a+') as f:                                                         # openr text fil  
            f.write((datetime.datetime.utcnow().strftime("%d-%m-%Y %H:%M:%S")) + "\n"       # og laver et skive funktion hvor jeg for date 
            + "temperature: " + str(round(t,2)) + "C" + "\n" + "luftfut: " +                # vadieren for SenseHat
            str(round(h)) + "%" + "\n" + "pressure: " + str(round(p)) + "\n" + "\n")        # og skiver ude i test.csv
    
        time.sleep(1)                                                                       # sitter spit time op
        timer -=1                                                                           # og siger at den skal tag 1 fra være gean den måller 

sys.exit()                                                                                  # exter stop ikke nedvente 