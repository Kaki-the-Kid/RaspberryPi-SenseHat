#/bin/python3.7

def replaceNumbers(n):
	if (n % 3) == 0:
		if (n % 5) == 0:
			return "vaniljebudding"
		else:
			return "vanilje"
	elif (n % 5) == 0 and (n % 3) > 0:
		return "budding"

	else:
		return n

for x in range(0,25):
	print(replaceNumbers(x))
