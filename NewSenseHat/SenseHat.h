/*****************************************************************//**
 * @file	SenseHat.h
 * @date	27. August 2020
 * @version	1.0
 * @author	krs - Karsten Reitan Sørensen
 * @details	SenseHat class - updated and extended
 * 
 * @brief	
  * This is based on the original from Philippe Simier, Christophe Grillo and Jon Dellaria
* 
 * Original code:
 * Philippe Simier: https://github.com/PhilippeSimier/SenseHat
 * 
 * 
 * 
 * 
 * -file SenseHat.h
 * -date 4 mars 2018
 * -version 1.2
 * -authors Philippe SIMIER Philippe CRUCHET Christophe GRILLO
 * -details Classe SenseHat : Gestion de la carte SenseHat
 * -version 1.3
 * -date 30 July 2019
 * -authors Jon Dellaria bug fixes, method translation to English and Temperature Measurement adjustments required for the Raspberry for a true temperature reading.
*********************************************************************/



#ifndef SENSEHAT_H
#define SENSEHAT_H

#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

#define PI 3.14159265

#define NUMBER_OF_TRIES_BEFORE_FAILURE 1000


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fcntl.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <array>

//Linux bases libraries - currently working on windows machine
//todo - still needs to be tested on correct Raspberry Pi machine
//#include <dirent.h>
//#include <linux/fb.h>
//#include <sys/mman.h>
//#include <sys/ioctl.h>
//#include <unistd.h>
//#include <termios.h>
//#include <linux/input.h>

// Reference to the local url - working on a windows machine for the moment
// Part of this tast is to properly document RTIMULib for the student
#include "RTIMULib/usr/include/RTIMULib.h" //<RTIMULib.h>


class SenseHat
{
private:
	void ConvertCharacterToPattern(char c, uint16_t image[8][8], uint16_t colorTexte, uint16_t colorFond);
	bool EmptyColumn(int numcolumn, uint16_t image[8][8], uint16_t colorFond);
	void ImageContainment(int numcolumn, uint16_t image[][8][8], int taille);

	struct fb_t* fb;
	int joystick;
	RTIMUSettings* settings;
	RTIMU* imu;
	RTPressure* pressure;
	RTHumidity* humidite;
public:
	SenseHat(); // Generel constructor for class
	~SenseHat(); // General deconstrucctor for class

	// For documentation and tutorials about operators, look at:
	// todo
	SenseHat& operator<<(SenseHat& (*)(SenseHat&));
	SenseHat& operator<<(const std::string&);
	SenseHat& operator<<(const int);
	SenseHat& operator<<(const double);
	SenseHat& operator<<(const char);
	SenseHat& operator<<(const char*);
	SenseHat& operator<<(const bool);


	//--------------------------------------------------------------------
	// Functions related to the LED Matrix on the Sense Hat
	//--------------------------------------------------------------------
	#pragma region led_related

private:
	int rotation = 0; // Default value for the direction of text on display, values: 0 90 180 270
	uint16_t color;
	std::string buffer;

public:
	#define COLOR_SENSEHAT uint16_t
	#define RED 	0xF800
	#define BLUE    0x001F
	#define GREEN   0x07E0
	#define WHITE   0xFFFF
	#define BLACK   0x0000
	#define ORANGE  0xFC00
	#define CYAN    0x87FF
	#define MAGENTA 0xF81F
	#define YELLOW  0xFFE0


	struct fb_t {
		uint16_t pixel[8][8];
	};

	void InitializeLeds();
	void LightPixel(int row, int column, uint16_t color);
	void ViewPattern(uint16_t motif[][8]);
	void RotatePattern(int rotation);
	void WipeScreen(uint16_t color = BLACK);

	COLOR_SENSEHAT ConvertRGB565(uint8_t red, uint8_t green, uint8_t blue);
	COLOR_SENSEHAT ConvertRGB565(uint8_t color[]);
	COLOR_SENSEHAT ConvertRGB565(std::string color);


	// @function			set_rotation
	// @description			If you're using the Pi upside down or sideways you can use 
	//						this function to correct the orientation of the image being shown.
	// @param	integer		r 
	//						0 90 180 270 //The angle to rotate the LED matrix though. 
	//						0 is with the Raspberry Pi HDMI port facing downwards.
	// @param	Boolean		redraw 
	//						True False	
	//						Whether or not to redraw what is already being displayed on the LED matrix.Defaults to True
	//	
	//	sense.set_rotation(180) OK
	//
	//	# alternatives: sense.rotation = 180
	void set_rotation(int newRotation) { rotation = newRotation; }
	void  FixerRotation(uint16_t);
	void  SetRotation(uint16_t);

	// @function			flip_h
	// @description			Flips the image on the LED matrix horizontally.
	//
	// @param	boolean		redraw
	//						redraw	Boolean	True False	Whether or not to redraw what is already 
	//						being displayed on the LED matrix.Defaults to True
	//	@return				List
	//						A list containing 64 smaller lists of[R, G, B] pixels(red, green, blue) 
	//						representing the flipped image.
	//	sense.flip_h()
	void flip_h(bool redraw = true)
	{
		//todo!
	}


	// @function			flip_v
	// @description			Flips the image on the LED matrix vertically.
	//
	// @param Boolean		redraw
	//						Whether or not to redraw what is already being displayed on the LED 
	//						matrix when flipped. Defaults to True
	// @return				List
	//						A list containing 64 smaller lists of[R, G, B] pixels(red, green, blue) 
	//						representing the flipped image.
	// sense.flip_v()
	void flip_v(bool redraw = true)
	{
		//todo
	}


	// @function			set_pixels
	// @description			Updates the entire LED matrix based on a 64 length list of pixel values.
	// @param				pixel_list
	//						List [[R, G, B]* 64] A list containing 64 smaller lists of[R, G, B] 
	//						pixels(red, green, blue).Each R - G - B element must be an integer between 0 and 255.
	// @return				None
	// @usage
	//						X = [255, 0, 0]  # Red@
	//						O = [255, 255, 255]  # White
	//						question_mark = [
	//							O, O, O, X, X, O, O, O,
	//							O, O, X, O, O, X, O, O,
	//							O, O, O, O, O, X, O, O,
	//							O, O, O, O, X, O, O, O,
	//							O, O, O, X, O, O, O, O,
	//							O, O, O, X, O, O, O, O,
	//							O, O, O, O, O, O, O, O,
	//							O, O, O, X, O, O, O, O
	//						]
	//						sense.set_pixels(question_mark)
	void set_pixels()
	{
		//todo - this function already exist
	}
	void Fixercolor(uint16_t);
	void SetColor(uint16_t);

	// @function		get_pixels
	// @description		The get_pixels function provides a correct representation of 
	//					how the pixels end up in frame buffer memory after you've 
	//					called set_pixels.
	//
	//					Note: You will notice that the pixel values you pass into set_pixels 
	//					sometimes change when you read them back with get_pixels.This is because 
	//					we specify each pixel element as 8 bit numbers(0 to 255) but when they're 
	//					passed into the Linux frame buffer for the LED matrix the numbers are bit 
	//					shifted down to fit into RGB 565. 5 bits for red, 6 bits for green and 5 
	//					bits for blue. The loss of binary precision when performing this conversion 
	//					(3 bits lost for red, 2 for green and 3 for blue) accounts for the 
	//					discrepancies you see.
	// @param			List
	//					A list containing 64 smaller lists of[R, G, B] pixels(red, green, blue) 
	//					representing the currently displayed image. 
	// @usage			pixel_list = sense.get_pixels()
	void get_pixels()
	{
		//todo
	}

	uint16_t GetPixel(int row, int column);


	// @function		set_pixel
	// @description		Sets an individual LED matrix pixel at the specified X - Y 
	//					coordinate to the specified colour.
	// @param			x	Integer	0 - 7	0 is on the left, 7 on the right.
	// @param			y	Integer	0 - 7	0 is at the top, 7 at the bottom.
	//
	//		Colour can either be passed as an RGB tuple :
	//	pixel	Tuple or List(r, g, b)	Each element must be an integer between 0 and 255.
	//		Or three separate values for red, greenand blue :
	//	r	Integer	0 - 255	The Red element of the pixel.
	//		g	Integer	0 - 255	The Green element of the pixel.
	//		b	Integer	0 - 255	The Blue element of the pixel.
	//		Returned type	Explanation
	//		None
	//		from sense_hat import SenseHat

	//		sense = SenseHat()
	//		# examples using (x, y, r, g, b)
	//		sense.set_pixel(0, 0, 255, 0, 0)
	//		sense.set_pixel(0, 7, 0, 255, 0)
	//		sense.set_pixel(7, 0, 0, 0, 255)
	//		sense.set_pixel(7, 7, 255, 0, 255)
	//
	//		red = (255, 0, 0)
	//		green = (0, 255, 0)
	//		blue = (0, 0, 255)
	//		# examples using (x, y, pixel)
	//		sense.set_pixel(0, 0, red)
	//		sense.set_pixel(0, 0, green)
	//		sense.set_pixel(0, 0, blue)


	// @function		get_pixel
	//		Parameter	Type	Valid values	Explanation
	//		x	Integer	0 - 7	0 is on the left, 7 on the right.
	//		y	Integer	0 - 7	0 is at the top, 7 at the bottom.
	//		Returned type	Explanation
	//		List	Returns a list of[R, G, B] representing the colour of an individual LED matrix pixel at the specified X - Y coordinate.
	//		from sense_hat import SenseHat
	//		sense = SenseHat()
	//		top_left_pixel = sense.get_pixel(0, 0)
	//		Note: Please read the note under get_pixels
	void get_pixel()
	{
		//todo
	}


	// @function		load_image
	// @description		Loads an image file, converts it to RGB format and displays it 
	//					on the LED matrix.The image must be 8 x 8 pixels in size.
	// @param	string	file_path
	//					Any valid file path.The file system path to the image file to load.
	// @param	boolean	redraw
	//					Whether or not to redraw the loaded image file on the LED matrix. 
	//					Defaults to True
	// @return			List
	//					A list containing 64 smaller lists of[R, G, B] pixels(red, green, blue) 
	//					representing the loaded image after RGB conversion.
	// @usage			sense.load_image("space_invader.png")
	// @usage			invader_pixels = sense.load_image("space_invader.png", redraw = False)
	void load_image(std::string filename)
	{
		//todo; 
	}

	int load_image(std::string filename, bool redraw = true)
	{
		//todo
		return 0;
	}


	// @function		clear
	// @description		Sets the entire LED matrix to a single colour, defaults to blank  / off.
	// @param 			colour Tup//todole or List(r, g, b)
	//					A tuple or list containing the RGB(red, green, blue) values of the colour. 
	//					Each element must be an integer between 0 and 255. Defaults to(0, 0, 0).
	//
	//					Alternatively, the RGB values can be passed individually :
	// @param			r	Integer	
	//					0 - 255	The Red element of the colour.
	// @param			g	Integer	
	//					0 - 255	The Green element of the colour.
	// @param			b	Integer	
	//					0 - 255	The Blue element of the colour.
	// @usage			red[] = {255, 0, 0};
	//					sense.clear()               // no arguments defaults to off
	//					sense.clear(red)            // passing in an RGB tuple
	//					sense.clear(255, 255, 255)  // passing in r, g and b values of a colour
	void clear() {}
	//void clear(array values[3]) {}
	void clear(int red, int green, int blue) {}


	// @function		show_message
	// @description		Scrolls a text message from right to left across the LED 
	//					matrix and at the specified speed, in the specified colour 
	//					and background colour.
	// 
	// @param			text_string		string	Any text string.The message to scroll.
	// @param			scroll_speed	Float	Any floating point number.The speed at 
	//					which the text should scroll.This value represents the time 
	//					paused for between shifting the text to the left by one column 
	//					of pixels.Defaults to 0.1
	// @param			text_colour	List[R, G, B]	
	//					A list containing the R - G - B(red, green, blue) colour of the text.
	//					Each R - G - B element must be an integer between 0 and 255. 
	//					Defaults to[255, 255, 255] white.
	// @param			back_colour	List[R, G, B]	
	//					A list containing the R - G - B(red, green, blue) colour of the 
	//					background.Each R - G - B element must be an integer between 
	//					0 and 255. Defaults to[0, 0, 0] black / off.
	// @return			None
	// @usage			sense.show_message("One small step for Pi!", text_colour = [255, 0, 0])
	void show_message(std::string message)
	{
		//todo
	}

	//void show_message(std::string message, array colors[3])
	//{
	//	//todo
	//}


	// @function		show_letter
	// @description		Displays a single text character on the LED matrix.
	// @param			s	string	A text string of length 1.	The letter to show.
	// @param			text_colour	List[R, G, B]	
	//					A list containing the R - G - B(red, green, blue) colour of the letter.
	//					Each R - G - B element must be an integer between 0 and 255. 
	//					Defaults to[255, 255, 255] white.
	// @param			back_colour	List[R, G, B]	
	//					A list containing the R - G - B(red, green, blue) colour of the background.
	//					Each R - G - B element must be an integer between 0 and 255. 
	//					Defaults to[0, 0, 0] black / off.
	// @return			None
	// @usage			for i in reversed(range(0, 10))
	//					{
	//						sense.show_letter(str(i))
	//						time.sleep(1)
	//						low_light
	//					}
	//
	//					Toggles the LED matrix low light mode, useful if the Sense HAT is being used in a dark environment.
	//					sense.clear(255, 255, 255);
	//					sense.low_light = true;
	//					sleep(2);
	//					sense.low_light = false;
	void show_letter(char character)
	{
		//todo
	}


	// @function		gamma
	// @description		For advanced users.Most users will just need the low_light Boolean 
	//					property above.The Sense HAT python API uses 8 bit(0 to 255) colours 
	//					for R, G, B.When these are written to the Linux frame buffer they're 
	//					bit shifted into RGB 5 6 5. The driver then converts them to RGB 5 5 5 
	//					before it passes them over to the ATTiny88 AVR for writing to the LEDs.
	//
	//					The gamma property allows you to specify a gamma lookup table for the 
	//					final 5 bits of colour used.The lookup table is a list of 32 numbers 
	//					that must be between 0 and 31. The value of the incoming 5 bit colour 
	//					is used to index the lookup table and the value found at that position 
	//					is then written to the LEDs.
	// @param						Type	Valid values	Explanation
	//			Tuple or List	Tuple or List of length 32 containing Integers between 0 and 31	Gamma lookup table for the final 5 bits of colour
	//			import time
	//			from sense_hat import SenseHat
	//			sense = SenseHat()
	//			sense.clear(255, 127, 0)
	//			print(sense.gamma)
	//			time.sleep(2)
	//			sense.gamma = reversed(sense.gamma)
	//			print(sense.gamma)
	//			time.sleep(2)
	//			sense.low_light = True
	//			print(sense.gamma)
	//			time.sleep(2)
	//			sense.low_light = False


	// @function		gamma_reset
	// @description		A function to reset the gamma lookup table to default, ideal 
	//					if you've been messing with it and want to get it back to a 
	//					default state.
	// @return			None
	// @usage			sense.clear(255, 127, 0)
	//					sleep(2)
	//					sense.gamma = [0] * 32  # Will turn the LED matrix off
	//					sleep(2)
	//					sense.gamma_reset()
	void gamma_reset() {}

	#pragma endregion


	//--------------------------------------------------------------------
	// Functions related to the sensors on the Sense Hat
	//--------------------------------------------------------------------
	#pragma region environmental_sensors

	float GetTemperature();
	float correctTemperature(float senseHatTemp, float cpuTemp);
	float getRawTemperature();
	float getCpuTemperature();

	void  InitializePressure();
	float GetPressure();

	void  InitializeHumidity();
	float GetHumidity();

	void  InitializeOrientation();
	void  GetOrientation(float &pitch, float &roll, float & yaw);

	#pragma endregion


	//--------------------------------------------------------------------
	// Functions related to the imu sensors on the Sense Hat
	// * Gyroscope
	// * Accelerometer
	// * Magnetometer(compass)
	// 
	// The IMU(inertial measurement unit) sensor is a combination of three sensors, 
	// each with an x, yand z axis.For this reason it's considered to be a 9 dof 
	// (degrees of freedom) sensor.
	// This API allows you to use these sensors in any combination to measure 
	// orientation or as individual sensors in their own right.
	//--------------------------------------------------------------------
	#pragma region imu_sensor

	void  InitializeAcceleration();
	void  GetAcceleration(float& x, float& y, float& z);

	void  GetMagnetism(float& x, float& y, float& z);
	void  GetSphericalMagnetism(float& ro, float& teta, float& delta);


	// @function		set_imu_config
	// @description		Enables and disables the gyroscope, accelerometerand /or 
	//					magnetometer contribution to the get orientation functions below.
	// @param			compass_enabled	Boolean		Whether or not the compass should be enabled.
	// @param			gyro_enabled	Boolean		Whether or not the gyroscope should be enabled.
	// @param			accel_enabled	Boolean			Whether or not the accelerometer should be enabled.
	// @return			None
	// @usage			sense.set_imu_config(false, true, false)  # gyroscope only
	void set_imu_config(bool compass_enabled, bool gyro_enabled, bool accel_enabled) 
	{
	}


	// @function		get_orientation_radians
	// @description		Gets the current orientation in radians using the aircraft 
	//					principal axes of pitch, rolland yaw.
	//
	// @return			Dictionary
	//					A dictionary object indexed by the strings pitch, rolland yaw.
	//					The values are Floats representing the angle of the axis in radians.
	//
	// @usage			orientation_rad = sense.get_orientation_radians()
	//					print("p: {pitch}, r: {roll}, y: {yaw}".format(**orientation_rad))
	//
	//					# alternatives
	//					print(sense.orientation_radians)
	int get_orientation_radians()
	{
		//todo
		return 0;
	}


	// @function		get_orientation_degrees
	// @description		Gets the current orientation in degrees using the aircraft 
	//					principal axes of pitch, rolland yaw.
	//
	// @return			Dictionary	
	//					A dictionary object indexed by the strings pitch, rolland yaw.
	//					The values are Floats representing the angle of the axis in degrees.
	// @usage			orientation = sense.get_orientation_degrees()
	//					print("p: {pitch}, r: {roll}, y: {yaw}".format(**orientation))
	int get_orientation_degrees()
	{
		//todo
		return 0;
	}


	// @function		get_orientation
	// @description		Calls get_orientation_degrees above.
	// @usgae			orientation = sense.get_orientation()
	//					print("p: {pitch}, r: {roll}, y: {yaw}".format(**orientation))
	//
	//					# alternatives
	//					print(sense.orientation)
	void get_orientation() 
	{
		//todo
	}


	// @function		get_compass
	// @description		Calls set_imu_config to disable the gyroscope and accelerometer then gets the direction of North from the magnetometer in degrees.
	// @return			Float	The direction of North.
	// @usage			north = sense.get_compass()
	//					prifnt("North: %s" % north)
	//
	//					# alternatives
	//					print(sense.compass)
	float get_compass()
	{
		//todo
		return 0;
	}


	// @function		get_compass_raw
	// @description		Gets the raw x, y and z axis magnetometer data.
	// @return			Dictionary
	//					A dictionary object indexed by the strings x, y and z.
	//					The values are Floats representing the magnetic intensity 
	//					of the axis in microteslas(µT).
	// @usage			raw = sense.get_compass_raw()
	//					print("x: {x}, y: {y}, z: {z}".format(**raw))
	//
	//					# alternatives
	//					print(sense.compass_raw)
	int get_compass_raw()
	{
		//todo
		return 0;
	}


	// @function		get_gyroscope
	// @description		Calls set_imu_config to disable the magnetometer and accelerometer
	//					then gets the current orientation from the gyroscope only.
	// @return			Dictionary
	//					A dictionary object indexed by the strings pitch, roll and yaw.
	//					The values are Floats representing the angle of the axis in degrees.
	// @usage			gyro_only = sense.get_gyroscope()
	//					print("p: {pitch}, r: {roll}, y: {yaw}".format(**gyro_only))
	//
	//					# alternatives
	//					print(sense.gyro)
	//					print(sense.gyroscope)
	void get_gyroscope()
	{
		//todo
	}


	// @function		get_gyroscope_raw
	// @description		Gets the raw x, y and z axis gyroscope data.
	// @return			Dictionary
	//					A dictionary object indexed by the strings x, y and z.
	//					The values are Floats representing the rotational 
	//					intensity of the axis in radians per second.
	// @usage			raw = sense.get_gyroscope_raw()
	//					print("x: {x}, y: {y}, z: {z}".format(**raw))
	//
	//					# alternatives
	//					print(sense.gyro_raw)
	//					print(sense.gyroscope_raw)
	int get_gyroscope_raw()
	{
		//todo
		return 0;
	}


	// @function		get_accelerometer
	// @description		Calls set_imu_config to disable the magnetometer and 
	//					gyroscope then gets the current orientation from the 
	//					accelerometer only.
	// @return			Dictionary
	//					A dictionary object indexed by the strings pitch, roll 
	//					and yaw.The values are Floats representing the angle of 
	//					the axis in degrees.
	// @usage			accel_only = sense.get_accelerometer()
	//					print("p: {pitch}, r: {roll}, y: {yaw}".format(**accel_only))
	//
	//					# alternatives
	//					print(sense.accel)
	//					print(sense.accelerometer)
	void get_accelerometer()
	{
		//todo
	}


	// @function		get_accelerometer_raw
	// @description		Gets the raw x, y and z axis accelerometer data.
	// @return			Dictionary
	//					A dictionary object indexed by the strings x, y and z.
	//					The values are Floats representing the acceleration 
	//					intensity of the axis in Gs.
	// @usage			raw = sense.get_accelerometer_raw()
	//					print("x: {x}, y: {y}, z: {z}".format(**raw))
	//
	//					# alternatives
	//					print(sense.accel_raw)
	//					print(sense.accelerometer_raw)
	void get_accelerometer_raw()
	{
		//todo
	}

	#pragma endregion


	//--------------------------------------------------------------------
	// Functions related to the joystack on the Sense Hat
	//--------------------------------------------------------------------
	#pragma region joystick_related

public:
	char ScannerJoystick();
	char ScanJoystick();
private:
	void  InitialiserJoystik();
	void  InitializeJoystick();


	// InputEvent
	// A tuple describing a joystick event.Contains three named parameters :
	//
	// * timestamp - The time at which the event occurred, as a fractional number of seconds(the same format as the built - in time function)
	// * direction - The direction the joystick was moved, as a string("up", "down", "left", "right", "middle")
	// * action - The action that occurred, as a string("pressed", "released", "held")
	//
	// This tuple type is used by several joystick methods either as the return type or the type of a parameter.


	// @function		wait_for_event
	// @description		Blocks execution until a joystick event occurs, then returns an InputEvent representing the event that occurred.

	//	from sense_hat import SenseHat
	//	from time import sleep

	//	sense = SenseHat()
	//	event = sense.stick.wait_for_event()
	//	print("The joystick was {} {}".format(event.action, event.direction))
	//	sleep(0.1)
	//	eve @nt = sense.stick.wait_for_event()
	//	print("The joystick was {} {}".format(event.action, event.direction))
	//	In the above example, if you briefly push the joystick in a single direction you should see two events output : a pressed actionand a released action.The optional emptybuffer can be used to flush any pending events before waiting for new events.Try the following script to see the difference :

	//from sense_hat import SenseHat
	//	from time import sleep

	//	sense = SenseHat()
	//	event = sense.stick.wait_for_event()
	//	print("The joystick was {} {}".format(event.action, event.direction))
	//	sleep(0.1)
	//	event = sense.stick.wait_for_event(emptybuffer = True)
	//	print("The joystick was {} {}".format(event.action, event.direction))




	// @function		get_events
	// @description		Returns a list of InputEvent tuples representing all events 
	//					that have occurred since the last call to get_events or 
	//					wait_for_event.
	// @usage			while(true)
	//					{
	//						for event in sense.stick.get_events()
	//						{
	//							print("The joystick was {} {}".format(event.action, event.direction))
	//						}
	//					}
	void get_events()
	{
		//todo
	}


	//	direction_up, direction_left, direction_right, direction_down, direction_middle, direction_any
	//	These attributes can be assigned a function which will be called whenever the joystick is pushed in the associated direction(or in any direction in the case of direction_any).The function assigned must either take no parameters or must take a single parameter which will be passed the associated InputEvent.
	//
	//	from sense_hat import SenseHat, ACTION_PRESSED, ACTION_HELD, ACTION_RELEASED
	//	from signal import pause
	//
	//	x = 3
	//	y = 3
	//	sense = SenseHat()
	//
	//	def clamp(value, min_value = 0, max_value = 7) :
	//	return min(max_value, max(min_value, value))
	//
	//	def pushed_up(event) :
	//	global y
	//	if event.action != ACTION_RELEASED :
	//		y = clamp(y - 1)
	//
	//	def pushed_down(event) :
	//		global y
	//		if event.action != ACTION_RELEASED :
	//			y = clamp(y + 1)
	//
	//	def pushed_left(event) :
	//		global x
	//		if event.action != ACTION_RELEASED :
	//			x = clamp(x - 1)
	//
	//	def pushed_right(event) :
	//		global x
	//		if event.action != ACTION_RELEASED :
	//			x = clamp(x + 1)
	//
	//	def refresh() :
	//		sense.clear()
	//		sense.set_pixel(x, y, 255, 255, 255)
	//
	//	sense.stick.direction_up = pushed_up
	//	sense.stick.direction_down = pushed_down
	//	sense.stick.direction_left = pushed_left
	//	sense.stick.direction_right = pushed_right
	//	sense.stick.direction_any = refresh
	//	refresh()
	//	pause()
	//
	//	Note that the direction_any event is always called after all other events making it an ideal hook for things like display refreshing(as in the example above).


	#pragma endregion




	void  Version();
	void  Flush();


};

// overload manipulators
SenseHat& endl(SenseHat& os);
SenseHat& flush(SenseHat& os);

// surcharge du manipulateur setcolor
struct _Setcolor { int val; };
_Setcolor setcolor(int n);
SenseHat&  operator<<(SenseHat& os, _Setcolor color);


#endif // SENSEHAT_H
