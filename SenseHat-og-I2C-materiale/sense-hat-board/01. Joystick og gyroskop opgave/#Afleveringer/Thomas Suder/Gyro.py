from sense_hat import SenseHat
sense = SenseHat()
sense.clear()
import time
import decimal
W = (255,255,255)
N = (0,0,0)

sense.low_light = True

nothing = [N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N,
          N,N,N,N,N,N,N,N, 
          ]

pilop = [N,N,N,W,W,N,N,N,
        N,N,W,W,W,W,N,N,
        N,W,W,W,W,W,W,N,
        W,W,W,W,W,W,W,W,
        N,N,N,W,W,N,N,N,
        N,N,N,W,W,N,N,N,
        N,N,N,W,W,N,N,N,
        N,N,N,W,W,N,N,N,
        ]
pilned = [N,N,N,W,W,N,N,N,
          N,N,N,W,W,N,N,N,
          N,N,N,W,W,N,N,N,
          N,N,N,W,W,N,N,N,
          W,W,W,W,W,W,W,W,
          N,W,W,W,W,W,W,N,
          N,N,W,W,W,W,N,N,
          N,N,N,W,W,N,N,N,
          ]

pilvenstre = [N,N,N,W,N,N,N,N,
             N,N,W,W,N,N,N,N,
             N,W,W,W,N,N,N,N,
             W,W,W,W,W,W,W,W,
             W,W,W,W,W,W,W,W,
             N,W,W,W,N,N,N,N,
             N,N,W,W,N,N,N,N,
             N,N,N,W,N,N,N,N,
             ]
pilhøjre =  [N,N,N,N,W,N,N,N,
             N,N,N,N,W,W,N,N,
             N,N,N,N,W,W,W,N,
             W,W,W,W,W,W,W,W,
             W,W,W,W,W,W,W,W,
             N,N,N,N,W,W,W,N,
             N,N,N,N,W,W,N,N,
             N,N,N,N,W,N,N,N,
             ]

højrehjørnen = [N,N,N,W,W,W,W,W,
             N,N,N,N,W,W,W,W,
             N,N,N,N,N,W,W,W,
             N,N,N,N,W,N,W,W,
             N,N,N,W,N,N,N,W,
             N,N,W,N,N,N,N,N,
             N,W,N,N,N,N,N,N,
             W,N,N,N,N,N,N,N,
             ]
             
højrehjørneo = [W,W,W,W,W,N,N,N,
              W,W,W,W,N,N,N,N,
              W,W,W,N,N,N,N,N,
              W,W,N,W,N,N,N,N,
              W,N,N,N,W,N,N,N,
              N,N,N,N,N,W,N,N,
              N,N,N,N,N,N,W,N,
              N,N,N,N,N,N,N,W,
              ]
venstrehjørnen = [W,N,N,N,N,N,N,N,
              N,W,N,N,N,N,N,N,
              N,N,W,N,N,N,N,N,
              N,N,N,W,N,N,N,W,
              N,N,N,N,W,N,W,W,
              N,N,N,N,N,W,W,W,
              N,N,N,N,W,W,W,W,
              N,N,N,W,W,W,W,W,
              ]
venstrehjørneo = [N,N,N,N,N,N,N,W,
              N,N,N,N,N,N,W,N,
              N,N,N,N,N,W,N,N,
              W,N,N,N,W,N,N,N,
              W,W,N,W,N,N,N,N,
              W,W,W,N,N,N,N,N,
              W,W,W,W,N,N,N,N,
              W,W,W,W,W,N,N,N,
              ]

while True:
    
   # edge = [0, 1, 2, 3, 4, 5, 6, 7, 15, 23, 31, 39, 47, 55, 63, 62, 61, 60, 59, 58, 57, 56, 48, 40, 32, 24, 16,8]
    #length = len(edge)
    #ratio = length / 360
  
    #y = number // 8
    #x = number % 8

    
    
    o = sense.get_orientation()
    
    pitch = o["pitch"]
    roll = o["roll"]
    yaw = o["yaw"]
    
   # yaw_list_position = int(yaw * ratio)
    
   # yaw_pixel_number = edge[yaw_list_position]
    
   # y = yaw_pixel_number // 8
   # x = yaw_pixel_number % 8
    
   # sense.set_pixel(x, y, 255,255,255)
    
    
    pitch = round(pitch,2)
    roll = round(roll,2)
    yaw = round(yaw,2)
    
    print("pitch{0} roll {1} yaw {2}".format(pitch, roll, yaw)) #format er en metode ligesom str. som gør at man kan skrive tekst og tal sammen.
    time.sleep(0.1)                                             # {0}{1}{2} sørger for at værdierne bliver hvor de er når de printes ud.
 

    sens = 1
    
     
         
    
    if pitch > 1 and pitch <90 and roll < 2 or roll >358:     
        sense.set_pixels(pilvenstre)
   
    
    if pitch <358 and pitch > 270: # and roll < 2-sens or roll >358:
        sense.set_pixels(pilhøjre)
        
  
  
    if pitch < 2 and pitch < 358 and roll >2 and roll <100:
        sense.set_pixels(pilned)
      
    if pitch < 2 and pitch < 358 and roll >266 and roll <359:
        sense.set_pixels(pilop)
    
    
    if pitch >0+sens and pitch <90-sens and roll > 0+sens and roll <=90:
        sense.set_pixels(venstrehjørneo)
    
    if pitch > 0+sens and pitch <90-sens and roll >266 and roll < 360-sens:       
        sense.set_pixels(højrehjørneo)
     
    if pitch < 358 and pitch > 270 and roll > 1 and roll <100:
        sense.set_pixels(venstrehjørnen)
    
    if pitch < 358 and pitch > 270 and roll > 260 and roll < 358:
        sense.set_pixels(højrehjørnen)
    
    #if pitch >358 and pitch < 2 and roll >358 and roll < 2:
     #   sense.set_pixels(nothing)
   
    
   # if pitch >= 358 and pitch <= 2 and roll <= 2 and roll >= 358:
   #     sense.set_pixels(nothing)
   #     sense.set_rotation(270)

        
    

  
 #in range teknikken er også godt, men kan kun bruges med int. dvis ikke float.

#if pitch in range(1,90):
#    sense.set_pixels(pilvenstre)
#else:
#        sense.set_pixels(nothing)
#if pitch in range (290,359):
#        sense.set_pixels(pilhøjre)
#if roll in range(180,359):
#        sense.set_pixels(pilop)
#if roll in range(1, 179):
#        sense.set_pixels(pilned)
 
  
    #if roll in range 



