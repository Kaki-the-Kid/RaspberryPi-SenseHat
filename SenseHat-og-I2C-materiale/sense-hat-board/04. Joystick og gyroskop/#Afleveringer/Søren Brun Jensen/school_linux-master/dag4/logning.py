from sense_hat import SenseHat as shat 																				#Library til sensehat
import time																											#Library til bl. a. slepp funktion
import _thread																										#Library til at lave tråde

global flag																											#Global variabel til at signalerer mellem tråde
global terminate																									#Global variabel til at signalerer mellem tråde

flag = 0																											#Global variabel initialiseres
terminate = 0																										#Global variabel initialiseres

sh = shat()																											#Opretter et sense hat objekt

file = "log.csv"																									#Opretter string-variabel med navnet på filen der logges til 

def write_labels():																									#Funktion der skriver "labels" til log-filen
	with open(file, 'a+') as f:
		f.write('Temperature' + ',' + 'Humidity' + ',' + 'Pressure' + '\n')											#Sammensætter linje i csv-filen

def write_empty_line():																								#Funktion der skriver en tom linje i csv-filen
	with open(file, 'a+') as f:
		f.write('\n')

def append_to_file(data):																							#Funktion der skriver data til csv-filen data er en dictionary
	with open(file, 'a+') as f:																						#Åbner filen med with, som selv sørger for at lukke file handle-
																													#når scope er slut
		entry = str(round(data['t'],2)) + ',' + str(round(data['h'])) + ',' + str(round(data['p'], 2)) + '\n'	#Laver string sammensat af data. Adskilt af kommaer og slutter med new line 
		f.write(entry)																								#Skriver til filen

def get_sensor_data():																								#Funtion der tager data fra sensorerne
	t = sh.get_temperature()
	h = sh.get_humidity()
	p = sh.get_pressure()
	return {'t' : t, 'h' : h, 'p' : p}																				#Returnerer en dictionary med sensorværdierne

def logger_function():																								#Funktion der køres i seperat tråd. Logger data til csv-filen hvert sekund
	print("Logging... Press enter to stop")																			
	global flag																										#Gør global variabel tilgængelig for funktion
	while flag == 0:																								#Mens flag er nul 
		append_to_file(get_sensor_data())																			#Skriv sensor-data til fil
		time.sleep(1)																								#Vent 1 sekund
	print("Logging stopped!")																						
	global terminate																								#Gør variabel tilgængelig for funktion
	terminate = 1																									#Sætter terminate variablen til 1
	return 																											#returnér fra funktion. (Nødvendigt for .join() for at stoppe tråden)

def keyboard_function():																							#Funktion der køres i seperat tråd. Venter på der trykkes enter på keyboarded
	input()																											#Venter på enter
	global flag																										#Gør global variabel tilgængelig for tråd
	flag = 1																										
	return 																											#returnér fra funktion. (Nødvendigt for .join() for at stoppe tråden)

write_labels()																										#Programmet starter med at skrive "labels" i csv-filen

try:																												#Try er en slags conditional konstruktion der "prøver at køre kode"
																													#Men springer til except hvis der opstår en fejl
	_thread.start_new_thread(logger_function, ())																	#Starter ny tråd som kører funktionen logger_function med 0 inputparametre
	_thread.start_new_thread(keyboard_function, ())																	#Starter ny tråd som kører funktionen keyboard_function med 0 inputparametre

except Exception as e:																								#Hvis koden i try fejler
	print(str(e))																									#Printer fejlen som string 

while terminate == 0:																								#Mens terminate = 0. (mens enter ikke trykket på endnu)
	pass																											#Gør ingen ting da seperate tråde laver arbejdet.

write_empty_line()																									#Når programmet afsluttes skrives en tom linje til csv-filen
