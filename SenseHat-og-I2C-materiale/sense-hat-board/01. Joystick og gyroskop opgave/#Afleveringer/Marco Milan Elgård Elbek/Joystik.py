#Marco Elbek
#17/4/2020
#Opgave 2
#Joystick

#Importer det bibliotek der skal bruges til sensehatten
from sense_hat import SenseHat

#Sense sættes til sense hatten så den ved det er den som skal bruges
sense = SenseHat()

#Loop til at check joystick og print retning
while True:
    for x in sense.stick.get_events():
        if x.direction == 'up':
            print("UP")
        elif x.direction == 'down':
            print("DOWN")
        elif x.direction == 'left':
            print("LEFT")
        elif x.direction == 'right':
            print("RIGHT")
        elif x.direction == 'middle':
            print("KLIK")