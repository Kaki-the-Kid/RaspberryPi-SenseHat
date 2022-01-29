"""
Steffen Østerby
Opg 2-4
Temp.
"""

import time
import datetime
from sense_hat import SenseHat


sense = SenseHat ()

file = open ("Datalogger.csv", "w")#opretter og åbner en fil
file.write("Dato,Klokken,Temp (C),Fugtighed (%),Tryk (mBar)\n")#indsætter Dato ... i øverstelinje af dukomentet
file.close#og lukker filen igen

while True:#loop der køre
    dateandtime = datetime.datetime.now ()#henter dato og klokkesæt ligger det i variablen dateandtime
    temp = sense.get_temperature()#henter temp og ligger det i mit temp variable
    temp = round(temp,2)#runder temp ned med to decimaler


    fugt= sense.get_humidity()#det samme som ved temp
    fugt= round(fugt,2)

    tryk= sense.get_pressure()#det samme som ved temp
    tryk= round (tryk,2)

    #print(dateandtime.strftime ("%Y-%m-%d T%H:%M:%S%z"))
    #print("Temperatur: " + str(temp), "Fugtighed: " + str(fugt), "Tryk: " + str(tryk))
    
    file = open ("Datalogger.csv", "a")#opretter og åbner en fil
    file.write(dateandtime.strftime ("%d-%m-%Y,%H:%M:%S"))# i filen skriver jeg dato og tid for min udlæsning
    file.write("," + str(temp) + "," + str(fugt) + "," + str(tryk) + "\n")#på samme linje, men i næste kolone skriver jeg mine udlæsninger
    
    file.close()#lukker filen
    
    file = open ("Datalogger.csv", "r")#åbner den igen
    print (file.read())#og printer data i terminalprogrammet 
    
    time.sleep(5)#venter 5 sek og kører while lykken igen.

