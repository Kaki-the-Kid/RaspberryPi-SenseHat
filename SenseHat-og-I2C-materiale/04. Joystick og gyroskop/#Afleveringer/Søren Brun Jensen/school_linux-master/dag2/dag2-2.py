#/bin/python3.7
import re

string = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."

x = re.search("ipsum", string)

print(x.group())
