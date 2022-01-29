/*****************************************************************//**
 * @file	SenseHat.cpp
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

#include "SenseHat.h"
#include "font.h"
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


static int is_framebuffer_device(const struct dirent* dir)
{
	return strncmp(FB_DEV_NAME, dir->d_name,
		strlen(FB_DEV_NAME) - 1) == 0;
}


static int open_fbdev(const char* dev_name)
{
	struct dirent** namelist;
	int i, ndev;
	int fd = -1;
	struct fb_fix_screeninfo fix_info;

	ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
	if (ndev <= 0)
		return ndev;

	for (i = 0; i < ndev; i++)
	{
		char fname[64];

		snprintf(fname, sizeof(fname),
			"%s/%s", DEV_FB, namelist[i]->d_name);
		fd = open(fname, O_RDWR);
		if (fd < 0)
			continue;

		ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
		if (strcmp(dev_name, fix_info.id) == 0)
			break;

		close(fd);
		fd = -1;
	}

	for (i = 0; i < ndev; i++)
		free(namelist[i]);

	return fd;
}


static int is_event_device(const struct dirent* dir)
{
	return strncmp(EVENT_DEV_NAME, dir->d_name,
		strlen(EVENT_DEV_NAME) - 1) == 0;
}


static int open_evdev(const char* dev_name)
{
	int tries;
	struct dirent** namelist;
	int i, ndev;
	int fd = -1;
	int sortie = false;

	tries = 0;
	while (true)
	{
		ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
		if (ndev <= 0)
		{
			tries++;
			usleep(100);
		}
		else
		{
			break;
		}
		if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
		{
			exit(EXIT_FAILURE);
		}
	}

	i = 0;
	do
	{
		char fname[64];
		char name[256];

		snprintf(fname, sizeof(fname),
			"%s/%s", DEV_INPUT_EVENT, namelist[i++]->d_name);

		tries = 0;
		while (true)
		{
			fd = open(fname, O_RDONLY);
			if (fd < 0)
			{
				tries++;
				usleep(100);
			}
			else
			{
				break;
			}
			if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
			{
				exit(EXIT_FAILURE);
			}
		}

		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

		if (strcmp(dev_name, name) != 0)
			close(fd);
		else sortie = true;
	} while (i < ndev && sortie != true);

	for (i = 0; i < ndev; i++)
		free(namelist[i]);
	return fd;
}


uint16_t handle_events(int evfd)
{
	struct input_event ev;
	int rd;
	uint16_t retour = 0;
	int flag;

	flag = fcntl(evfd, F_GETFL, 0);
	fcntl(evfd, F_SETFL, flag | O_NONBLOCK | O_NOCTTY);

	rd = read(evfd, &ev, sizeof(struct input_event));

	if (rd > 0)
	{
		if (ev.type == EV_KEY && ev.value == 1)
			retour = ev.code;

	}

	return retour;
}


// @brief		SenseHat::SenseHat
// @details		Default constructor
//				Initialise the attributes
//				default imu, leds, Joystick and buffer.
SenseHat::SenseHat()
{
	int tries;
	settings = new RTIMUSettings("RTIMULib");

	tries = 0;
	while (true)
	{
		imu = RTIMU::createIMU(settings);
		if ((imu == NULL) || (imu->IMUType() == RTIMU_TYPE_NULL))
		{
			tries++;
			usleep(100);
		}
		else
		{
			break;
		}
		if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
		{
			exit(EXIT_FAILURE);
		}
	}

	imu->IMUInit();
	imu->setSlerpPower(0.02);
	imu->setGyroEnable(true);
	imu->setAccelEnable(true);
	imu->setCompassEnable(true);
	InitializeLeds();
	InitializeJoystick();
	InitializeHumidity();
	InitializePressure();
	buffer = " ";
	color = BLUE;
	rotation = 0;
}


// @function		deconstructor
// @brief			SenseHat::~SenseHat
// @details Destructeur de la classe
SenseHat::~SenseHat()
{
	delete settings;
}


/**
 * @brief SenseHat::operator<<
 * @details surcharge de l'opérateur << pour les modificateurs endl et flush
 */


SenseHat& SenseHat::operator<<(SenseHat& (*fp)(SenseHat&))
{
	return (*fp)(*this);
}


// @function		SenseHat::Version
// @brief			
// @details affiche la version de la classe
void SenseHat::Version()
{
	std::cout << "SenseHat PCT,PSR,CGO Version 1.2.0" << std::endl;
}


void SenseHat::Fixercolor(uint16_t _color)
{
	color = _color;
}


void SenseHat::SetColor(uint16_t _color)
{
	color = _color;
}


void SenseHat::FixerRotation(uint16_t _rotation)
{
	rotation = _rotation;
}


void SenseHat::SetRotation(uint16_t _rotation)
{
	rotation = _rotation;
}



	//--------------------------------------------------------------------
	// Functions related to the LED Matrix on the Sense Hat
	//--------------------------------------------------------------------
	#pragma region led_related

	// @function		SenseHat::InitialiserLeds
	// @detail initialise de framebuffer
	void SenseHat::InitializeLeds()
	{
		int fbfd;
		int tries;
		int tries2;

		tries = 0;
		tries2 = 0;
		while (true)
		{
			fbfd = open_fbdev("RPi-Sense FB");
			if (fbfd > 0)
			{
				while (true)
				{
					fb = (struct fb_t*)mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
					if (!fb)
					{
						tries2++;
						usleep(100);
						if (tries2 > NUMBER_OF_TRIES_BEFORE_FAILURE)
						{
							printf("Failed to mmap.\n");
							exit(EXIT_FAILURE);
						}
					}
					else
					{

						memset(fb, 0, 128);
						return;
					}

				}
			}
			else
			{
				close(fbfd);
				tries++;
				usleep(100);
				if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
				{
					printf("Error: cannot open framebuffer device.\n");
					exit(EXIT_FAILURE);
				}
			}

		}

	}


	// @function	SenseHat::AllumerPixel
	// @brief 
	// @param		row		int		indicate row
	// @param		column	int		indicate column
	// @param		color   uint16_t a color in the format 565
	// @details fixe la color d'un pixel désigné par ses coordonnées
	void SenseHat::LightPixel(int row, int column, uint16_t color)
	{
		if (row < 0)
			row = 0;
		if (column < 0)
			column = 0;

		fb->pixel[row % 8][column % 8] = color;
	}


	// @function		SenseHat::ViewPatterns
	// @param			tableau 8*8 de uint16_t
	// @details			Displays the pixel table on the display taking into account 
	//					the angle of rotation
	void SenseHat::ViewPattern(uint16_t motif[][8])
	{
		for (int row = 0; row < 8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				switch (this->rotation)
				{
				case   90:
				case -270:
					fb->pixel[7 - column][row] = motif[row][column];
					break;
				case  180:
				case -180:
					fb->pixel[7 - row][7 - column] = motif[row][column];
					break;
				case  270:
				case  -90:
					fb->pixel[column][7 - row] = motif[row][column];
					break;
				default:
					fb->pixel[row][column] = motif[row][column];
				}
			}
		}
	}


	// @function		SenseHat::PivoterMotif
	// @param			angle int	Display rotation 90, 180, 270, -90, -180, -270
	// @details			Rotates the display pattern around the middle of a rotation angle.
	void SenseHat::RotatePattern(int angle)
	{
		uint16_t tabAux[8][8];

		for (int row = 0; row < 8; row++)
		{
			for (int column = 0; column < 8; column++)
			{
				switch (angle)
				{
				case   90:
				case -270:
					tabAux[7 - column][row] = fb->pixel[row][column];
					break;
				case  180:
				case -180:
					tabAux[7 - row][7 - column] = fb->pixel[row][column];
					break;
				case  270:
				case  -90:
					tabAux[column][7 - row] = fb->pixel[row][column];
					break;
				default:
					tabAux[row][column] = fb->pixel[row][column];
				}
			}
		}

		//    AfficherMotif(tabAux);
		ViewPattern(tabAux);
	}


	// @function		SenseHat::Effacer
	// @param			color	uint16_t a color in the format 565
	// @details			Displays the color on the entire led display 
	//					the color BLACK turns off the screen
	void SenseHat::WipeScreen(uint16_t color = BLACK)
	{
		memset(fb, color, 128);
	}


	// @function		SenseHat::ConvertirRGB565
	// @param			red		uint8_t composante rouge
	// @param			green	uint8_t composante verte
	// @param			blue	uint8_t composante BLUE
	// @return			uint16_t a color encoded as RGB565
	// @details			allows you to convert a color express in the form of three 
	//					unsigned 8-bit integers into an integer representing the color 
	//					encoded in RGB565
	COLOR_SENSEHAT SenseHat::ConvertRGB565(uint8_t red, uint8_t green, uint8_t blue)
	{

		blue	&= 0xF8;
		green	&= 0xFC;
		red		&= 0xF8;

		return ((red << 8) + (green << 3) + (blue >> 3));
	}


	// @function		SenseHat::ConvertirRGB565 (overload)
	// @param			array of of three colors uint8_t
	// @return			uint16_t a color encoded as RGB565
	// @details			allows you to convert a color express in the form of three 
	//					unsigned 8-bit integers into an integer representing the color 
	//					encoded in RGB565
	COLOR_SENSEHAT SenseHat::ConvertRGB565(uint8_t color[])
	{
		return ConvertRGB565(color[1], color[2], color[3]);
	}


	// @function		SenseHat::ConvertirRGB565 (overload)
	// @param			array of of three colors uint8_t
	// @return			uint16_t a color encoded as RGB565
	// @details			allows you to convert a color express in the form of three 
	//					unsigned 8-bit integers into an integer representing the color 
	//					encoded in RGB565
	COLOR_SENSEHAT SenseHat::ConvertRGB565(std::string hexCode)
	{
		int r, g, b;

		// Retire le hashtag ...
		if (hexCode.at(0) == '#') {
			hexCode = hexCode.erase(0, 1);
		}
		// puis extraction des valeurs r g b.
		std::istringstream(hexCode.substr(0, 2)) >> std::hex >> r;
		std::istringstream(hexCode.substr(2, 2)) >> std::hex >> g;
		std::istringstream(hexCode.substr(4, 2)) >> std::hex >> b;

		return ConvertRGB565(r, g, b);
	}





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








/**
 * @brief SenseHat::AfficherLettre
 * @param lettre char
 * @param colorTexte  uint16_t une color au format 565
 * @param colorFond   uint16_t une color au format 565
 * @details affiche un caractère (lettre ponctuation signe) sur l'afficheur
 */
void SenseHat::ViewLetter(char lettre, uint16_t colorTexte, uint16_t colorFond)
{
	uint16_t chr[8][8];
	//	ConvertirCaractereEnMotif(lettre,chr,colorTexte,colorFond);
	ConvertCharacterToPattern(lettre, chr, colorTexte, colorFond);
	//	AfficherMotif(chr);
	ViewPattern(chr);
}


/**
 * @brief SenseHat::ObtenirPixel
 * @param int indice de la row
 * @param int indice de la column
 * @return uint16_t une color au format 565
 * @details  retourne, sous la forme d'un entier sur 16 bits non signé, la color du pixel
 *           dont les coordonnées sont passées à la fonction.
 */
uint16_t SenseHat::GetPixel(int row, int column)
{
	if (row < 0)
		row = 0;
	if (column < 0)
		column = 0;

	return fb->pixel[row % 8][column % 8];
}


/**
 * @brief SenseHat::ScannerJoystick
 * @return le code équivalent aux touches du clavier enter,
 * fleche droite, gauche, haut et bas.
 */
char SenseHat::ScannerJoystick()
{
	return handle_events(joystick);
}

char SenseHat::ScanJoystick()
{
	return handle_events(joystick);
}


/**
 * @brief SenseHat::ObtenirTemperature
 * @return float la valeur de la température exprimée en °C,
 */
float SenseHat::GetTemperature()
{
	float cpuTemp;
	float correctedTemp;
	float senseHatTemp;

	senseHatTemp = getRawTemperature();
	cpuTemp = getCpuTemperature();

	//temp_calibrated = temp - ((cpu_temp - temp)/FACTOR)
	correctedTemp = correctTemperature(senseHatTemp, cpuTemp);
	return (correctedTemp);
}


float SenseHat::correctTemperature(float senseHatTemp, float cpuTemp)
{
	float correctedTemp;
	float TEMPERATUREFACTOR = 1.2;

	//temp_calibrated = temp - ((cpu_temp - temp)/FACTOR)
	correctedTemp = senseHatTemp - ((cpuTemp - senseHatTemp) / TEMPERATUREFACTOR);
	return (correctedTemp);
}


/**
 * @brief SenseHat::getRawTemperature
 * @return float la valeur de la température exprimée en °C,
 */


float SenseHat::getRawTemperature()
{
	RTIMU_DATA data;
	float senseHatTemp;

	pressure->pressureRead(data);
	senseHatTemp = data.temperature;
	return (senseHatTemp);
}


/**
 * @brief SenseHat::getCoreTemperature
 * @return float la valeur de la température exprimée en °C,
 */


float SenseHat::getCpuTemperature()
{
	FILE* temperatureFile;
	double T;
	temperatureFile = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
	if (temperatureFile == NULL)
	{
		//      printf ("Error getting Core Temperature!")  ; //print some message
		return (0);
	}
	fscanf(temperatureFile, "%lf", &T);
	T /= 1000;
	//    printf ("The temperature is %6.3f C.\n", T);
	fclose(temperatureFile);
	return(T);
}


/**
 * @brief SenseHat::ObtenirPression
 * @return float la valeur de la Pression exprimée en hPa,
 * @detail la valeur correspond à la valeur brute mesurée à l'altitude du capteur
 *         elle doit donc être convertie pour être ramenée au niveau de la mer
 */
float SenseHat::GetPressure()
{
	RTIMU_DATA data;
	float pression = nan("");  // initialise la valeur à Not-A-Number
	if (pressure->pressureRead(data)) {
		if (data.pressureValid) {
			pression = data.pressure;
		}
	}
	return pression;
}

/**
 * @brief SenseHat::ObtenirHumidite
 * @return float la valeur de l'humidité relative exprimée en %,
 */
float SenseHat::GetHumidity()
{
	RTIMU_DATA data;
	float humidi = nan("");  // initialise la valeur à Not-A-Number
	if (humidite->humidityRead(data)) {
		if (data.humidityValid) {
			humidi = data.humidity;
		}
	}
	return humidi;
}

/**
 * @brief SenseHat::ObtenirOrientation
 * @return float la valeur de l'accélération angulaire suivant pitch roll et yaw
 * @detail la valeur est exprimée en rd/s
 *
 */
void SenseHat::GetOrientation(float& pitch, float& roll, float& yaw)

{
	while (imu->IMURead()) {
		RTIMU_DATA imuData = imu->getIMUData();
		pitch = imuData.gyro.x();
		roll = imuData.gyro.y();
		yaw = imuData.gyro.z();
	}
}


/**
 * @brief SenseHat::ObtenirAcceleration
 * @return float la valeur de l'accélération linéaire suivant X,Y,Z
 * @detail la valeur est exprimée en m/s*s
 *
 */
void SenseHat::GetAcceleration(float& x, float& y, float& z)
{
	while (imu->IMURead()) {
		RTIMU_DATA imuData = imu->getIMUData();
		x = imuData.accel.x();
		y = imuData.accel.y();
		z = imuData.accel.z();
	}
}


/**
 * @brief SenseHat::ObtenirMagnétisme
 * @return float la valeur du champ magnétique terrestre suivant X,Y,Z
 * @detail la valeur est exprimée en micro Tesla
 *
 */
void SenseHat::GetMagnetism(float& x, float& y, float& z)
{
	while (imu->IMURead()) {
		RTIMU_DATA imuData = imu->getIMUData();
		x = imuData.compass.x();
		y = imuData.compass.y();
		z = imuData.compass.z();
	}
}

/**
 * @brief SenseHat::ObtenirMagnetismeSpherique
 * @return la valeur du vecteur champ magnétique en coordonnées sphérique
 * @detail teta l'angle mesuré depuis l'axe des X
 *      si teta = 0 alors l'axe X est dirigé vers le nord magnétique
 *	si teta = 180 ou -180 alors l'axe des X est dirigé vers le sud
 */
void SenseHat::GetSphericalMagnetism(float& ro, float& teta, float& delta)

{
	float x, y, z;

	//   ObtenirMagnetisme(x,y,z);
	GetMagnetism(x, y, z);
	teta = atan2(y, x) * 180 / PI;
	ro = sqrt(x * x + y * y + z * z);
	delta = atan2(z, sqrt(x * x + y * y)) * 180 / PI;
}




void SenseHat::InitializeLeds()
{
	int fbfd;
	int tries;
	int tries2;

	tries = 0;
	tries2 = 0;
	while (true)
	{
		fbfd = open_fbdev("RPi-Sense FB");
		if (fbfd > 0)
		{
			while (true)
			{
				fb = (struct fb_t*)mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
				if (!fb)
				{
					tries2++;
					usleep(100);
					if (tries2 > NUMBER_OF_TRIES_BEFORE_FAILURE)
					{
						printf("Failed to mmap.\n");
						exit(EXIT_FAILURE);
					}
				}
				else
				{

					memset(fb, 0, 128);
					return;
				}

			}
		}
		else
		{
			close(fbfd);
			tries++;
			usleep(100);
			if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
			{
				printf("Error: cannot open framebuffer device.\n");
				exit(EXIT_FAILURE);
			}
		}

	}

}

/**
 * @brief  SenseHat::InitialiserJoystik
 * @detail initialise le Joystick
 */
void SenseHat::InitialiserJoystik()
{
	InitializeJoystick();
}

void SenseHat::InitializeJoystick()
{
	joystick = open_evdev("Raspberry Pi Sense HAT Joystick");
}

/**
 * @brief  SenseHat::InitialiserPression
 * @detail initialise le capteur de pression
 */
void SenseHat::InitializePressure()
{
	int tries;

	tries = 0;
	while (true)
	{
		pressure = RTPressure::createPressure(settings);
		if (pressure == NULL)
		{
			tries++;
			usleep(100);
		}
		else
		{
			break;
		}
		if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
		{
			printf("Pas de mesure de pression/température \n");
			exit(EXIT_FAILURE);
		}
	}
	pressure->pressureInit();
}


/**
 * @brief  SenseHat::InitialiserHumidite
 * @detail initialise le capteur d'humidité
 */
void SenseHat::InitializeHumidity()
{
	int tries;

	tries = 0;
	while (true)
	{
		humidite = RTHumidity::createHumidity(settings);

		if (humidite == NULL)
		{
			tries++;
			usleep(100);
		}
		else
		{
			break;
		}

		if (tries > NUMBER_OF_TRIES_BEFORE_FAILURE)
		{
			printf("Pas de mesure de pression/température \n");
			exit(EXIT_FAILURE);
		}
	}

	humidite->humidityInit();
}


void SenseHat::InitializeOrientation()
{
	InitializeOrientation();

}


void SenseHat::InitializeAcceleration()
{
	imu->setAccelEnable(true);
}


/**
 * @brief  SenseHat::ConvertirCaractereEnMotif
 * @detail Converti un caractère en Motif affichable sur la matrice de leds
 * - Fait par Grilo Christophe
 */
void SenseHat::ConvertCharacterToPattern(char c, uint16_t image[8][8], uint16_t colorTexte, uint16_t colorFond)
{
	int i = 0;
	int j, k;
	int tailleTableDeConvertion = sizeof(font) / sizeof(Tfont);

	// Recherche si le caractere existe dans la table de convertion (cf font.h)
	while (c != font[i].caractere && i < tailleTableDeConvertion)
		i++;

	// Si le caractere est dans la table on le converti
	if (i < tailleTableDeConvertion)
	{
		for (j = 0; j < 8; j++)
		{
			for (k = 0; k < 8; k++)
			{
				if (font[i].motifbinaire[j][k])
					image[j][k] = colorTexte;
				else
					image[j][k] = colorFond;
			}
		}
	}
	else // caractère inexistant on le remplace par un glyphe inconnu
//	ConvertirCaractereEnMotif(255,image,colorTexte,colorFond);
ConvertCharacterToPattern(255, image, colorTexte, colorFond);
}


bool SenseHat::EmptyColumn(int numcolumn, uint16_t image[8][8], uint16_t colorFond)
{
	int i = 0;
	for (i = 0; i < 8; i++)
		if (image[i][numcolumn] != colorFond)
			return false;
	return true;
}


void SenseHat::ImageContainment(int numcolumn, uint16_t image[][8][8], int taille)
{
	int i = 0, j = 0, k = 0, l = 0, isuivant, ksuivant;
	int nombredecolumns = taille * 8; //8 columns par motif

	for (l = numcolumn; l < nombredecolumns - 1; l++)
	{
		i = l / 8;
		k = l % 8;
		isuivant = (l + 1) / 8;
		ksuivant = (l + 1) % 8;
		for (j = 0; j < 8; j++)
			image[i][j][k] = image[isuivant][j][ksuivant];
	}
}

void SenseHat::AfficherMessage(const std::string message, int vitesseDefilement, uint16_t colorTexte, uint16_t colorFond)
{
	ViewMessage(message, vitesseDefilement, colorTexte, colorFond);
}

void SenseHat::ViewMessage(const std::string message, int vitesseDefilement, uint16_t colorTexte, uint16_t colorFond)
{
	int taille = message.length();
	uint16_t chaine[taille][8][8]; /* Le tableau de motif (image/caractère) à afficher */
	int i = 0, j = 0, k = 0, l = 0, nombreDecolumnVide = 0;
	int isuivant = 0, ksuivant = 0, nombreDecolumns = 0;

	/* Convertion de tout le message en tableau de motifs
	 * format caractère : 1 column vide + 5 columns réellement utilisées
	 * + 2 columns vides */
	for (i = 0, j = 0; i < taille; i++, j++)
	{

		if (message[i] == 195)  // les lettres accentuées sont codées sur deux octets  (195 167 pour ç)
		{
			i++;
			k++;
		}
		// row suivante à décommenter pour obtenir le code des caractères UTF8
		// std::cout << "code : " << (int)message[i] << std::endl;
	//	ConvertirCaractereEnMotif(message[i],chaine[j],colorTexte,colorFond);
		ConvertCharacterToPattern(message[i], chaine[j], colorTexte, colorFond);
	}
	taille = taille - k;
	nombreDecolumns = (taille) * 8 - 2;
	k = 0;
	// Parcours de toutes les columns de tous les motifs qui compose
	// la chaine de caractères à afficher pour supprimer les columns vides sauf celle
	// qui sépare les motifs (caractères). + gestion du caractère espace.
	for (l = 0; l < nombreDecolumns; l++)
	{
		i = l / 8;
		k = l % 8;

		//		if(columnVide(k,chaine[i],colorFond)) // Une column Vide avant chaque caractère à ne pas supprimer
		if (EmptyColumn(k, chaine[i], colorFond)) // Une column Vide avant chaque caractère à ne pas supprimer

		{
			isuivant = (++l) / 8;
			ksuivant = (l) % 8;
			nombreDecolumnVide = 1;

			// compter les columns vide après la première afin de les supprimer
			// si plus de 4 c'est le caractère espace que l'on doit garder
//			while(columnVide(ksuivant,chaine[isuivant],colorFond) && nombreDecolumnVide++ < 6)
			while (EmptyColumn(ksuivant, chaine[isuivant], colorFond) && nombreDecolumnVide++ < 6)
			{
				//				TassementDeLimage(l,chaine,taille);
				ImageContainment(l, chaine, taille);
				nombreDecolumns--;
			}

		}
	}

	// Parcours de toutes les columns de tous les motifs qui composent
	// la chaine de caractères à afficher (sans les columns vides superflues).
	for (l = 0; l < nombreDecolumns; l++)
	{
		// Decalage des columns vers la gauche sur l'image Numero 0 (celle qu'on affiche sur la matrice de LED
		for (i = 0; i < taille; i++)
		{
			// Cas Normal, les columns sont sur le même motif
			for (j = 0; j < 8; j++)
			{
				for (k = 0; k < 7; k++)
					chaine[i][j][k] = chaine[i][j][k + 1];

			}
			// Cas où l'on doit changer de motif
			for (j = 0; j < 8; j++)
				chaine[i][j][7] = chaine[i + 1][j][0];
		}
		usleep(1000 * vitesseDefilement);
		//		AfficherMotif(chaine[0]);
		ViewPattern(chaine[0]);
	}

}


SenseHat& SenseHat::operator<<(const std::string& message)
{
	buffer += message;
	return *this;
}

SenseHat& SenseHat::operator<<(const int valeur)
{
	buffer += std::to_string(valeur);
	return *this;
}

SenseHat& SenseHat::operator<<(const double valeur)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << valeur;
	buffer += ss.str();
	return *this;
}

SenseHat& SenseHat::operator<<(const char caractere)
{
	buffer += std::string(1, caractere);
	return *this;
}

SenseHat& SenseHat::operator<<(const char* message)
{
	buffer += std::string(message);
	return *this;
}

SenseHat& SenseHat::operator<<(const bool valeur)
{
	buffer += std::to_string(valeur);
	return *this;
}
// Méthode Flush() Affiche le buffer puis le vide
void SenseHat::Flush()
{
	buffer += "  ";
	//   AfficherMessage(buffer, 80, color);
	ViewMessage(buffer, 80, color);
	buffer = " ";
}

// Modificator endl
// (endl manipulator) effectue un flush du buffer
SenseHat& endl(SenseHat& os)
{
	os.Flush();
	return os;
}


SenseHat& flush(SenseHat& os)
{
	os.Flush();
	return os;
}


_Setcolor setcolor(int n)
{
	return { n };
}


SenseHat& operator<<(SenseHat& os, _Setcolor color)
{
	//    os.Fixercolor(color.val);
	os.SetColor(color.val);
	return os;
}

_SetRotation setrotation(int n)
{
	return { n };
}


SenseHat& operator<<(SenseHat& os, _SetRotation rotation)
{
	//    os.FixerRotation(rotation.val);
	os.SetRotation(rotation.val);
	return os;
}
