#Marco Elbek
#17/4/2020
#Opgave 4
#Gyro

#Importer det bibliotek der skal bruges til sensehatten, og til sleep funktion
from sense_hat import SenseHat
from time import sleep

#Sense sættes til sense hatten så den ved det er den som skal bruges
sense = SenseHat()

#Hvid sættes op
w = 200, 200, 200

#Blank sættes op
e = 0, 0, 0

#Pil sættes op
arrow = [
    e,e,e,w,w,e,e,e,
    e,e,w,w,w,w,e,e,
    e,w,e,w,w,e,w,e,
    w,e,e,w,w,e,e,w,
    e,e,e,w,w,e,e,e,
    e,e,e,w,w,e,e,e,
    e,e,e,w,w,e,e,e,
    e,e,e,w,w,e,e,e
    ]

#Viser en pil på displayed
sense.set_pixels(arrow)

#Loop som læser gyroscope og ændre pilens retning
while True:
#For loop bruges til at undgå den flipper ud
    for Q in range(0,10):
        x, y ,z = sense.get_accelerometer_raw().values()

    x = round (x,0)
    z = round (z,0)

#Pil retning   
    if x == -1:
        sense.set_rotation(0)
    elif z == 1:
        sense.set_rotation(270)
    elif z == -1:
        sense.set_rotation(90)
    else:
        sense.set_rotation(180)