# Haukur Hermannsson
# 17.04.2020
# SenseHat - DataLogger Opgave

import sys                                                                                                                                                                 
import time                                                                                                                                                                
import datetime                                                                                                                                                            
from sense_hat import SenseHat                                                                                                                                             # Importer bibliotekerne sys, time, datetime og SenseHat                                                                                                                                                        #

hat = SenseHat()                                                                                                                                                           # Giver SenseHat en variable

while True:                                                                                                                                                                # Starter et while loop der kører indtil brugeren stopper den
    t = hat.get_temperature()                                                                                                                                              # Giver SenseHat Temperaturmåleren en variable
    h = hat.get_humidity()                                                                                                                                                 # Giver SenseHat Fugtihedsmåleren en variable
    p = hat.get_pressure()                                                                                                                                                 # Giver SenseHat Trykmåleren en variable
    print("(%s) -- Temperatur: %.1f C -- Lufttryk: %.0f -- Fugtighed: %.0f" % (datetime.datetime.now().strftime("%d-%m-%Y) (%H:%M:%S"), t, p, h) )                         # Skriver Temperatur, Tryk og Fugtighed samt en Timestamp i konsolen
    myFile=open("HaukurSenseHatReadout.csv", "a+")                                                                                                                         # Åbner en fil i "appending og reading mode"
    myFile.write("(%s) -- Temperatur: %.1f C -- Lufttryk: %.0f -- Fugtighed: %.0f" % (datetime.datetime.now().strftime("%d-%m-%Y) (%H:%M:%S"), t, p, h) + "\n" + "\n")     # Skriver det samme som print fuktionen ind i filen
    myFile.close()                                                                                                                                                         # Lukker filen
    time.sleep(1)                                                                                                                                                          # Venter 1 sekund indtil den starter while loopen igen
    
sys.exit()                                                                                                                                                                 # Går ud af programmet når while loopen stopper