"""
Steffen Østerby
Opg gyroskop
Opgave 2 - Du skal lave den grundlæggende del af styringen af en drone. I denne opgave skal du emulere, at dronen holder sig lige i luften.
Det betyder, at den hele tiden skal kompensere modsat. Du kan i første omgang
   - Lav en aflæsning af gyroskop
   - Lav en angivelse af kompenserende retning på displayet vha. en pil
"""
import time
from sense_hat import SenseHat
sense = SenseHat()

s = (0, 0, 0)       # Sort - defination sortfarve i displayet
h = (255, 0, 0)     # Rød - LED'erne lyser Rød
g = (0, 255, 0)     # Grøn


l_arrow = [
    s, s, s, s, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, h, h, s, s, s, s,
    s, h, h, h, h, h, s, s,
    s, s, h, h, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, s, s, s, s, s,
    s, s, s, s, s, s, s, s,
]

r_arrow = [
    s, s, s, s, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, h, h, s, s, s,
    s, h, h, h, h, h, s, s,
    s, s, s, h, h, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, s, s, s, s, s,
    s, s, s, s, s, s, s, s,
]

u_arrow = [
    s, s, s, s, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, h, h, h, s, s, s,
    s, h, h, h, h, h, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, s, s, s, s, s,
    s, s, s, s, s, s, s, s,
]

d_arrow = [
    s, s, s, s, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, h, s, s, s, s,
    s, h, h, h, h, h, s, s,
    s, s, h, h, h, s, s, s,
    s, s, s, h, s, s, s, s,
    s, s, s, s, s, s, s, s,
    s, s, s, s, s, s, s, s,
]

ok = [
    s, s, s, s, s, s, s, s,
    s, s, s, s, s, s, s, s,
    s, s, s, g, g, s, s, s,
    s, s, g, g, g, g, s, s,
    s, s, g, g, g, g, s, s,
    s, s, s, g, g, s, s, s,
    s, s, s, s, s, s, s, s,
    s, s, s, s, s, s, s, s,
    
]


while True:
    o = sense.get_orientation_degrees()#jeg vælger at at hente data fra orientation sensore og får dataen grader 0-360
    pitch = o["pitch"] #gemmer dataen i min variabe
    roll = o["roll"]   #samme
    

    pitch =round(pitch, 0)# af runder op til hele tal
    roll=round(roll, 0)#
    print("P: " + str(pitch), "R: " + str(roll))# printer værdien i variablerne i Shell

    if pitch >= 5 and pitch <=180:#hvis pitch er mellem 5 og 180 grader skal PI'en rettes op mod hjøre
        sense.set_pixels(r_arrow)
    elif pitch >= 181 and pitch <=355:# hvis pitch er mellem 181 og 355 grader skal PI'en rettes op mod venstre
        sense.set_pixels(l_arrow)
    elif roll >= 181 and roll <=355:#hvis roll er mellem 181 og 355 grader skal PI'en løfte bunden op 
        sense.set_pixels(d_arrow)
    elif roll >= 5 and roll <=180:#hvis roll er mellem 5 og 180 grader skal PI'en løfte toppen op
        sense.set_pixels(u_arrow)
    else:
        sense.set_pixels (ok)#er lige i luften svinger den melle 5 og -5 grader der den på nulpunktet.
        
        
    #time.sleep (2)
