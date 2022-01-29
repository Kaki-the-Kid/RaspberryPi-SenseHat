#Navn: Laust Kruse Pedersen
#Data: 17/04/2020
#Opgave: Data logging

from sense_hat import SenseHat
import datetime
import csv
import time

hat = SenseHat()
run = 1
x = datetime.datetime.now()

#Open or create a log file, named with todays date.
with open(x.strftime("%Y-%m-%d")+".csv", "a", newline="") as file:
    #Setting Names for the diffrent columns.
    colNames = ["Date:","Time:","Temp(C):","Pressure(mbar):","rh(%):"]
    #Define a writer with the right column names.
    writer = csv.DictWriter(file, fieldnames=colNames) 
    writer.writeheader()#Writing the headder with column names.
    
    while run==1:
        #Read the diffrent data i want to log to a file.
        x = datetime.datetime.now()
        temp = hat.get_temperature()
        temp = ro