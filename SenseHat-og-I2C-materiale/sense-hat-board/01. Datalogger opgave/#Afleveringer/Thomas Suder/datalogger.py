import os # gør så du kan slette en fil
import sys 
import time # tid i tilfælde af sleep så går den ud fra en bestemt oscillering som kaldes time.
import datetime # nuværende dato og tid importeret til logning af målinger.
import csv # comma seperated variables
from sense_hat import SenseHat # for at kunne bruge sensehat skal denne linie kode står der

sense = SenseHat() # gør det nemmere at indføre funktionen i koden. 

f = open("datalogger.csv", "a") # open funktionen tager to parametre, filnavn og mode. er her mode "a" valgt som laver en fil, hvis den ikke eksisterer i forvejen eller åbner den og gør klar til tilføjelse ligesom .append
f.write("    date      time,temp,humidity,pressure\n") # efter jeg har åbnet filen og gjort klar til tilføjelse, skriver jeg data ind i csv fil som er kan sammenlignes med excel. bemærk kommaerne som seperarer det jeg skriver ind i forskellige kolonner.derefter laver jeg en ny linie \n
f.close() # lukker filen.

while True:
    t = sense.get_temperature() #her laves variabler som funktioner der defineres t, p h  temperatur, tryk og fugtighed. funktionerne ber Hat om den nuværende måling
    p = sense.get_pressure()
    h = sense.get_humidity()
    x = datetime.datetime.now() # det samme som ovenstående
    
    
    f = open("datalogger.csv", "a")# se linie 10.
    
    f.write("\n") #ny linie 
    
    f.write(x.strftime(" %Y/%m/%d - %H:%M:%S ")) #her skriver jeg til en fil. write funktionen har to parametre, write (overskriver i tilfælde eksisterende data) append vil tilføje data ved slutningen af en fil. bemærk at jeg har lavet en ny linie ovenfor for ikke at overskrive gammel data.
                                                 #x = datetime.datetime.now() funktion så først skriver jeg dato og tid. strftime() metoden laver en formateret string fra en given datetime. %Y, %m, %d, %H. %M, %S er format koder. strftime() metoden kan tage op til flere format koder og retunere en formateret string baseret på det. %Y er fx format koden for nuværende år med årti som decimalnummer.  
                                                 
    f.write(",%.2f°C" %t  + ",%.0frh" %h + ",%.0fhPa" %p) # %t er fx temperatur som jeg vile have med to decimaler = "%.2f" . % er en operator som du kan bruge til at formatere strings %.2f formaterer en f string med 2 decimaler. det minder om str.format()
    
    print(x.strftime("%Y" + "/%m" + "/%d" + ",%X")) # her printer jeg værdierne ud så jeg kan få en ide om hvordan dataen skrives ind i csv dokumentet.
          
    print(",%.2f°C" %t + ",%.0frh" %h + ",%.0fhPa" %p) 
    
    f.close() # og her lukker jeg filen igen,
    
    
    time.sleep(2) 
   # time.sleep(2 - time.time() % 1)