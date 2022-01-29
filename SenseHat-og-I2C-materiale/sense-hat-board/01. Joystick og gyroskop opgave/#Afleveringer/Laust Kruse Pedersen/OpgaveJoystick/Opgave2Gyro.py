#Navn: Laust Kruse Pedersen
#Data: 17/04/2020
#Opgave: Gyroskop

from sense_hat import SenseHat

hat = SenseHat()
hat.set_emu_config(True,True,True)
                   
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
    
    gyro_data = hat.get_gyroscope_raw()
    x = round(gyro_data['x'],0)
    y = round(gyro_data['y'],0)
    
    if y == -1:
        hat.set_pixels(leftArrow)
        
    elif y == 1:
        hat.set_pixels(rightArrow)
        
    elif x == -1:
        hat.set_pixels(downArrow)
        
    elif x == 1:
        hat.set_pixels(upArrow)
        
    
            