from sense_hat import SenseHat as shat
import time

sh = shat()

sh.set_rotation(0)
sh.clear()

sh.set_imu_config(True, True, True)

pixels = [ [0, 0, 0] for i in range(64)]

#sh.set_pixel()

ball = [ [0,0], [1,0], [0,1], [1,1] ]

imuStartVal = sh.get_orientation_degrees()

ballX = 3.0
ballY = 3.0

def init_imu():
	input("Put the pi on flat surface, and press any key")

def show_ball(x, y, ball):
	for i in range(len(ball)):
		sh.set_pixel(ball[i][0] + int(x), ball[i][1] + int(y), 255, 0, 0)

def convert_imu_vals(o):
	p = (o['pitch'] / 360) * 2 - 1
	if p > -0.5 and p < 0.5:
		p = 0.0

	r = -((o['roll'] / 360) * 2 - 1)
	if r > -0.5 and r < 0.5:
		r = 0.0

	y = (o['yaw'] / 360) * 2 - 1
	return {'pitch' : p, 'roll' : r, 'yaw' : y}

def doPhysics(x, y, o):
	x += o['pitch']
	if x < 0:
		x = 0
	elif x > 6:
		x = 6

	y += o['roll']
	if y < 0:
		y = 0
	elif y > 6:
		y = 6

	return x, y

show_ball(3,3,ball)

init_imu()

while(True):
	ori = convert_imu_vals(sh.get_orientation_degrees())

	ballX,ballY = doPhysics(ballX, ballY, ori)
	sh.clear()
	show_ball(ballX,ballY,ball)

	#t = sh.get_orientation_degrees()

	#print("pitch: {pitch}, roll: {roll}, yaw: {yaw}".format(**t))
	time.sleep(0.1)