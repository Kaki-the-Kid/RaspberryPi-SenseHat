#Marco Elbek
#17/4/2020
#Opgave 1
#Temperatur, fugt og tryk

#Importer det moduler der skal bruges til at kunne lave csv filer, timestamp, sensor læsning etc.
import csv
from sense_hat import SenseHat
from datetime import datetime
from time import sleep

#Sense sættes til sense hatten så den ved det er den som skal bruges
sense = SenseHat()
sense.clear()

class Logger:
    
#Initialisere det hele med en "ordbog" af ting jeg kommer til at exportere til filerne
    def __init__(self):
        self.data_dict = {}

#Indsamling af de forskellige sensors data
    def collect_data(self):
#self.data_dict laver en fil med et label som er givet i ['']
#og dataen som kommer i den fil er så det information den henter efter = tegnet
#så dataen bliver datetime.now() som giver dato og klok, og den information som kan læses fra givne sensor temp, fugt, tryk....
        self.data_dict['temperatur'] = (datetime.now(), sense.get_temperature())
        self.data_dict['fugtighed'] = (datetime.now(), sense.get_humidity())
        self.data_dict['trykhed'] = (datetime.now(), sense.get_pressure())

#Konsol print af data i formateret strings så det er let læsligt
    def print_data(self):
        print("-"*120)
        print("~~ {0:%d-%m-%Y, %H:%M:%S} ~~" .format(*self.data_dict['temperatur']))
        print("Temperatur: {1:,.2f}" .format(*self.data_dict['temperatur']))
        print("Fugtighed: {1:,.2f}" .format(*self.data_dict['fugtighed']))
        print("Trykhed: {1:,.2f}" .format(*self.data_dict['trykhed']))

#Skriver dataen i csv filer
#filen åbnes i append mode så den kan tilføje data til det data der allerede er der
#hvis den skulle lukkes og startes op igen
    def log_data(self):
        for file, data in self.data_dict.items():
            with open('data/' + file + '.csv', 'a+', newline='') as f:
                writer = csv.writer(f)
                writer.writerow(data)

#Køre et loop som læser data, og skriver det på konsol og i fil hvert 5 sekund
def main():
    while True:
        logger = Logger()
        logger.collect_data()
        logger.log_data()
        logger.print_data()
        sleep(5)

#Kør main aka alt eftersom det hele er i min def main
main()