import subprocess

def get_date_string():
	s = subprocess.Popen('date', shell=None, stdout=subprocess.PIPE)
	out, err = s.communicate()
	return out[0:-1].decode()
