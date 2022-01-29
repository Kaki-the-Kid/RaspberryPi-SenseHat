# Anders Chr Rungborg
# 19-04-2020
# LEDPild
from sense_hat import SenseHat

hat = SenseHat()

b = (0, 0, 255)                                  # Blå
g = (0, 255, 0)                                  # Grøn
x = (0, 0, 0)                                    # slåg

image_Op = [                                     # pil op
    x, x, x, x, x, x, x, x,
    x, x, x, g, g, x, x, x,
    x, x, g, g, g, g, x, x,
    x, g, g, g, g, g, g, x,
    x, x, x, g, g, x, x, x,
    x, x, x, g, g, x, x, x,
    x, x, x, g, g, x, x, x,
    x, x, x, x, x, x, x, x
]

image_Ned = [                                     # pil ned
    x, x, x, x, x, x, x, x,
    x, x, x, g, g, x, x, x,
    x, x, x, g, g, x, x, x,
    x, x, x, g, g, x, x, x,
    x, g, g, g, g, g, g, x,
    x, x, g, g, g, g, x, x,
    x, x, x, g, g, x, x, x,
    x, x, x, x, x, x, x, x
]

image_Left = [                                     # pil ledt
    x, x, x, x, x, x, x, x,
    x, x, x, x, g, x, x, x,
    x, x, x, x, g, g, x, x,
    x, g, g, g, g, g, g, x,
    x, g, g, g, g, g, g, x,
    x, x, x, x, g, g, x, x,
    x, x, x, x, g, x, x, x,
    x, x, x, x, x, x, x, x
]

image_Rigt = [                                      # pil rigt
    x, x, x, x, x, x, x, x,
    x, x, x, g, x, x, x, x,
    x, x, g, g, x, x, x, x,
    x, g, g, g, g, g, g, x,
    x, g, g, g, g, g, g, x,
    x, x, g, g, x, x, x, x,
    x, x, x, g, x, x, x, x,
    x, x, x, x, x, x, x, x
]

creeper = [                                         # creeper
    g,g,g,g,g,g,g,g,
    g,g,g,g,g,g,g,g,
    g,b,b,g,g,b,b,g,
    g,b,b,g,g,b,b,g,
    g,g,g,b,b,g,g,g,
    g,g,b,b,b,b,g,g,
    g,g,b,b,b,b,g,g,
    g,g,b,g,g,b,g,g
]

while True:
    acceleration = hat.get_accelerometer_raw()      # setter qyrogyroskop på 
    x = acceleration["x"]                           # op og ned
    y = acceleration["y"]                           # venster og høred
    
    x=round(x, 0)                                   # rutasjon på op og ned 
    y=round(y, 0)                                   # rutasjon venster og høred
    

    
    if x == -1:                                     # her siger vis der er -1 
        hat.set_pixels(image_Left)                  # viser den pil til venster
        
    elif x == 1:                                    # her siger vis der er 1 
        hat.set_pixels(image_Rigt)                  # viser den pil til høred
        
    elif y == 1:                                    #
        hat.set_pixels(image_Op)                    #
        
    elif y == -1:                                   #
        hat.set_pixels(image_Ned)                   #
        
    else:                                           # standbymode
        hat.set_pixels(creeper)
        
