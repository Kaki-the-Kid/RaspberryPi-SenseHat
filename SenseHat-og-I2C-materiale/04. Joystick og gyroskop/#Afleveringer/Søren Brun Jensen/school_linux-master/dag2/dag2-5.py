#/bin/python3.7

def yourRating(alder):
	if int(alder) < 13:
		return "PG"

	elif int(alder) < 18 and int(alder) >= 13:
		return "PG-13"

	else:
		return "R"

print("Hvor gammel er du? ")
alder = input()
print("Du kan se: " + yourRating(alder) + " rated film.")
