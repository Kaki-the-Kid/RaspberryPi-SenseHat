#/bin/python3.7
def plusMoms(netto):
	return float(netto) * 1.25

print("Hvad er nettoprisen på varen? : ")
pris = input()
print("Prisen inklussiv moms er: " + str(plusMoms(pris)) + " kr.")
