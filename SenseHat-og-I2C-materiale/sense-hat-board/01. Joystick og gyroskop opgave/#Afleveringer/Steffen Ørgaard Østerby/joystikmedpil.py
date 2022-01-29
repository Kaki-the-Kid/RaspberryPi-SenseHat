"""
Steffen Østerby
Opg ex
joystik
"""
"""
Opgave 1 - Lav en udlæsning af joystick retning og få Python til at skrive retningen.

Opgave 1a - Udvid opgave 1 med at vise en pil på displayet alt efter retning på joysticket.
"""
from sense_hat import SenseHat
sense = SenseHat()

s = (0, 0, 0)       # Sort - defination sortfarve i displayet
h = (255, 255, 255) # Hvid - Lederne lyser hvid

"""
I disse "arrow" variabler har jeg diffienret hver pixel så display tegner en pil.
"""
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

m_arrow = [
    s, s, s, s, s, s, s, s,
    s, h, s, s, s, s, s, s,
    s, h, s, s, h, s, s, s,
    s, h, s, s, h, h, s, s,
    s, h, h, h, h, h, h, s,
    s, s, s, s, h, h, s, s,
    s, s, s, s, h, s, s, s,
    s, s, s, s, s, s, s, s,
    
]

while True:
    for event in sense.stick.get_events():#en func der styre Joystikket - denne func får et input fra joystikke 
       
        if event.action == 'pressed' and event.direction == 'up':#hvis joystikket er "trykket ned" opad skal den
            print(event.direction)#printer event.direction i shell
            sense.set_pixels(u_arrow)#og her sætte pixels til variablen u_arrow som er deffineret længere oppe.
        
        if event.action == 'pressed' and event.direction == 'down':
            print(event.direction)
            sense.set_pixels(d_arrow)
            
        if event.action == 'pressed' and event.direction == 'right':
            print(event.direction)
            sense.set_pixels(r_arrow)
            
        if event.action == 'pressed' and event.direction == 'left':
            print(event.direction)
            sense.set_pixels(l_arrow)
            
        if event.action == 'pressed' and event.direction == 'middle':
            print(event.direction)
            sense.set_pixels(m_arrow)
            
            
        
            