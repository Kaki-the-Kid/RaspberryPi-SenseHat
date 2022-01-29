# Mikkel Rahbek
# 20-04-2020
# Opgave Sense Hat

# Fra bibloteket sense_hat importeres funktionerne til SenseHat
from sense_hat import SenseHat

# Giver overordnet adgang til tid. Så som sleep og datetime
import time

# Giver adgang til forskellige funktioner og værdier i forhold til kørsels af programmet
import sys

# Giver adgang til sleep funktionerne
from time import sleep

# Giver adgang til dato funktionen
import datetime


# Laver SenseHat om til hat
hat = SenseHat() 
# Rotere pixelne 270 grader
hat.set_rotation(270) 
# Rød. Farven kan indeles i 256(0-255) forskellige styrker. Hvilket vil sige at den kan justere spændingen med en opløsning på en byte for hver diode.
red = (255, 0, 0)
# Grøn
green = (0, 255, 0)
# Blå
blue = (0, 0, 255)
# Gul. Blander farverne grøn og rød
yellow = (255, 200, 0)
# Sætter farven sort
black = (0, 0, 0)
# Sætter baggrunds farven(pink) Kun hvis den kaldes. Standard sort
back = (255, 0, 255)

# Sætter den målte temperatur fra hatten ind i strengen t
t = hat.get_temperature()
# Sætter den målte fugtighed fra hatten ind i strengen h
h = hat.get_humidity()
# Sætter det målte tryk fra hatten ind i strengen b
b = hat.get_pressure()

# Skriver tekst på hatten med sort, med en given visnings hastighed og baggrunds farve
hat.show_message("Hej Skat!", text_colour=black, scroll_speed=0.05, back_colour=back)
# Holder 0,5 selunders pause
sleep(.5)
# Clear displayet
hat.clear()
# Sætter log til det navn og fil type vi vil have
log = "log.csv"
# Laver et input hvor man kan indsætte nogle valg i terminalen
valg = (input("Tryk 't' for temp. 'h' for humid. 'b' for bar. 'l' for log Efterfulgt af enter ")) 


# Hvis l valgt som input
if (valg) == "l":
    # Timer får en værdi på 10
    timer = 10
    # Så lønge timer er over 0
    while timer > 0:
        # Skal køres i hvert loop, da vi ellers får et øjebliks billed, da værdierne ellers ikke bliver opdateret
        t = hat.get_temperature() 
        h = hat.get_humidity() 
        b = hat.get_pressure()
        # Dette er en 'Context Manager' funktion. Åbner/kreere filen i appending & reading mode(a+). Kan ikke slette i filen
        with open(log, 'a+') as f:
            # Skriver data i filen. datetime.now henter tiden fra raspi. str(round(x,x) gør opmærksom på at det er en streng og round af runder værdien med de givne decimaler. Slutter af med new line
            f.write(datetime.datetime.now().strftime("%d-%m-%Y - %H:%M:%S - ") + "Temp: " + str(round(t,2)) + "C" + " - " + "Humid: " + str(round(h,)) + "%" + " - " + "Pressur: " + str(round(b,)) + "mB" + "\n")
        # venter 1 sekund    
        sleep(1)
         # Tæller værdien af timer ned med 1 for hvert kørt cyklus
        timer -= 1
        
# Hvis t er valgt som input
if (valg) == "t":
    # .format tager strengen t og flytter ind i mellem {}. Den kigger samtidig på hvad der står inde i {}, hvilket i det her tilfælde har noget med afrunding af decimalerne at gøre.
    print("Temp C: {:.2f} C".format(t))
    # Denne gang er det dog hatten og ikke terminalen der viser værdien i en speciferet farve
    hat.show_message("T= {:.0f}C" .format(t), text_colour=red)
    
# Som beskrevet ovenover    
if (valg) == "h":
    print("Luftfugtighed: {:.0f}% ".format(h)) 
    hat.show_message("H= {:.0f}%" .format(h), text_colour=blue)
    
# Som beskrevet ovenover +1    
if (valg) == "b":
    print("Hektorpaskal: {:.0f} mB ".format(b)) 
    hat.show_message("B= {:.0f}mB" .format(b), text_colour=green) 
    
   