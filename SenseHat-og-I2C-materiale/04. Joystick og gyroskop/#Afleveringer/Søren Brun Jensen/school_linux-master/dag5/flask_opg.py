from flask import Flask, render_template
from sense_hat import SenseHat as shat

sh = shat()

file = "log.csv"

def get_file_data():
	logData = 0
	logDataList = []
	with open(file, "r") as f:
		logData = f.read()
	temp = logData.split("\n")
	
	for iter in temp:
		print(iter)
		if iter != 'Temperature,Humidity,Pressure':
			if iter != '':
				logDataList.append(iter.split(","))
	return logDataList
	 


def get_sensor_data():																								#Funtion der tager data fra sensorerne
	t = sh.get_temperature()
	h = sh.get_humidity()
	p = sh.get_pressure()
	return [round(t, 1), round(h), round(p, 2)]


app = Flask(__name__)

@app.route('/')

#@app.route('/')

def index():
	fresh = get_sensor_data()
	data = get_file_data()
	return render_template('index.html', vars=data, fresh=fresh)

if __name__ == '__main__':
	app.run(debug=True, host='127.0.0.1')