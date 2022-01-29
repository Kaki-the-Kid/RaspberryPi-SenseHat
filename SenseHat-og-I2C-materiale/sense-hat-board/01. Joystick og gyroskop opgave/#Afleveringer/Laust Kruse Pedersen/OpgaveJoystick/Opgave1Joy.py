#Navn: Laust Kruse Pedersen
#Data: 17/04/2020
#Opgave: Joystick 1

from sense_hat import SenseHat

hat = SenseHat()


X = (0, 255, 0) 
O= (0, 0, 0)

upArrow = [
    O, O, O, X, X, O, O, O,
    O, O, X, X, X, X, O, O,
    O, X, X, O, O, X, X, O,
    X, X, O, O, O, O, X, X,
    X, O, O, O, O, O, O, X,
    O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O
]
downArrow = [
    O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O,
    O, O, O, O, O, O, O, O,
    X, O, O, O, O, O, O, X,
    X, X, O, O, O, O, X, X,
    O, X, X, O, O, X, X, O,
    O, O, X, X, X, X, O, O,
    O, O, O, X, X, O, O, O
]
rightArrow = [
    O, O, O, X, X, O, O, O,
    O, O, O, O, X, X, O, O,
    O, O, O, O, O, X, X, O,
    O, O, O, O, O, O, X, X,
    O, O, O, O, O, O, X, X,
    O, O, O, O, O, X, X, O,
    O, O, O, O, X, X, O, O,
    O, O, O, X, X, O, O, O
]
leftArrow = [
    O, O, O, X, X, O, O, O,
    O, O, X, X, O, O, O, O,
    O, X, X, O, O, O, O, O,
    X, X, O, O, O, O, O, O,
    X, X, O, O, O, O, O, O,
    O, X, X, O, O, O, O, O,
    O, O, X, X, O, O, O, O,
    O, O, O, X, X, O, O, O
]

while True:
    for event in hat.stick.get_events():        
        if event.action == 'pressed' and event.direction == 'up':
            hat.set_pixels(upArrow)
        if event.action == 'pressed' and event.direction == 'down':
            hat.set_pixels(downArrow)
        if event.action == 'pressed' and event.direction == 'right':
            hat.set_pixels(rightArrow)
        if event.action == 'pressed' and event.direction == 'left':
            hat.set_pixels(leftArrow)
        if event.action == 'pressed' and event.direction == 'middle':
            hat.clear()
            exit()
            
            
            
