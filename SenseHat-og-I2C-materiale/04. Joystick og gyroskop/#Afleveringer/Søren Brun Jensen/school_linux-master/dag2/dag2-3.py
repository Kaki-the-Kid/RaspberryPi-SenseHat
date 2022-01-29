#/bin/python3.7
import re

string = "Larsten er underviser på AARHUS TECH"

x = re.sub("Larsten", "Karsten", string)

print(x)
