#/bin/python3.7

def getPrice(alder):
	if alder < 12:
		return 0.0
	elif alder < 24 and alder >= 12:
		return 0.7
	elif alder < 65 and alder >= 24:
		return 1.0
	else:
		return 0.75

print("Indtast din alder: ")

alder = input()

print("Indtast billetprisen: ")

pris = input()

print("Din pris er: " + str(float(pris) * getPrice(int(alder))) + " Kr.")
