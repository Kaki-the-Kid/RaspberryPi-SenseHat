#Marco Elbek
#17/4/2020
#Opgave 3
#Joystick Extended

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

#Loop til at check joystick og hvis pil på display
while True:
    for x in sense.stick.get_events():
        if x.direction == 'up':
            sense.set_pixels(arrow)
        
        elif x.direction == 'down':
            sense.set_rotation(180)
            sense.set_pixels(arrow)

        elif x.direction == 'left':
            sense.set_rotation(270)
            sense.set_pixels(arrow)

        elif x.direction == 'right':
            sense.set_rotation(90)
            sense.set_pixels(arrow)

        elif x.direction == 'middle':
            sense.set_rotation(0)
            sense.set_pixels(arrow)
            sleep(1)
            sense.set_rotation(90)
            sense.set_pixels(arrow)
            sleep(1)
            sense.set_rotation(180)
            sense.set_pixels(arrow)
            sleep(1)
            sense.set_rotation(270)
            sense.set_pixels(arrow)
            sleep(1)