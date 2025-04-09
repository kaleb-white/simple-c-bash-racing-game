/*

Written by Montek Singh
Copyright and all rights reserved by Montek Singh
Last Updated:  April 4, 2025

Permission granted to use this only for students, teaching/learning assistants
and instructors of the COMP 541 course at UNC Chapel Hill.
For any other use, contact Montek Singh first.

*/

/*

This is a C template for initial development
of your demo app for COMP541 find projects!

You must compile and run this code in an ANSI
compatible terminal.  You can use the terminal app
in the course VM.  For macOS and Linux users, the
standard terminal/shell on your laptop is also ANSI
compatible.

Open a terminal and compile and run the code as follows:

	gcc code.c
	./a.out

*/

#include <stdio.h>

/* Specify the keys here that get_key() will look for,
returning 1 if the first key was found, 2, for the second key, etc.,
and returning 0 if none of these keys was found.
In the actual board-level implementation, you will define
scancodes instead of characters, and you can use specify
key releases as well.
*/

int key_array[] = {'a', 'q', 'w', 'e', 'd'}; // define as many as you need

/* Let us define our sprites.  These will be text approximations
of the actual sprites used in the board implementation.
Here, each sprite is specified by:
	{ text character, foreground color, background color }

For example, specifying a sprite as
	{'.', white, red},
means it is drawn as a white dot over a red background.

Specify the number of sprites first (Nchars), and then the
attributes of each sprite.
*/

// type definition for emulating sprites (see below)
typedef struct
{
	char char_to_display;
	int color;
} sprite_attr;

#define Nchars 9

enum colors
{
	black,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white
};

enum BackgroundColor
{
	bgBLACK = 40,
	bgRED = 41,
	bgGREEN = 42,
	bgORANGE = 43,
	bgBLUE = 44,
	bgPURPLE = 45,
	bgCYAN = 46,
	bgGREY = 47,
	bgGRAY = 47
}; /*https://azrael.digipen.edu/~mmead/www/mg/ansicolors/index.html*/

sprite_attr sprite_attributes[Nchars] = {
		{' ', bgBLACK},
		{' ', bgRED},
		{' ', bgGREEN},
		{' ', bgORANGE},
		{' ', bgBLUE},
		{' ', bgPURPLE},
		{' ', bgCYAN},
		{' ', bgGREY},
		{' ', bgGRAY}};

//===============================================================
// Here are the functions available for I/O.  These correspond
// one-to-one to functions available in MIPS assembly in the
// helper files provided.
//
// NOTE:  There is one function specific to the C implementation
// that is not needed in the assembly implementation:
//     void initialize_IO(char* smem_initfile);
//===============================================================

void my_pause(int N); // N is hundredths of a second

void putChar_atXY(int charcode, int col, int row);
// puts a character at screen location (X, Y)

int getChar_atXY(int col, int row);
// gets the character from screen location (X, Y)

int get_key();
// if a key has been pressed and it matches one of the
// characters specified in key_array[], return the
// index of the key in that array (starting with 1),
// else return 0 if no valid key was pressed.

int get_key2();
// similar to get_key(), but looks for key in
// key_array2[].

int pause_and_getkey(int N);
// RECOMMENDED!
// combines pause(N) with get_key() to produce a
// *responsive* version of reading keyboard input

void pause_and_getkey_2player(int N, int *key1, int *key2);
// 2-player version of pause_and_getkey().

int get_accel();
// returns the accelerometer value:  accelX in bits [31:16], accelY in bits [15:0]
// to emulate accelerometer, use the four arrow keys

int get_accelX();
// returns X tilt value (increases back-to-front)

int get_accelY();
// returns Y tilt value (increases right-to-left)

void put_sound(int period);
// visually shows approximate sound tone generated
// you will not hear a sound, but see the tone highlighted on a sound bar

void sound_off();
// turns sound off

void put_leds(int pattern);
// put_leds: set the LED lights to a specified pattern
//   displays on row #31 (below the screen display)

void initialize_IO(char *smem_initfile);

void initialize_smem(char *initfilename);

//===============================================================
// This is the code for your demo app!
//===============================================================

char __current_smem[30][40];

void read_smem(char *initfilename)
{
	FILE *initfile = fopen(initfilename, "r");
	if (!initfile)
	{
		fprintf(stderr, "Could not open screen memory initialization file: %s\n", initfilename);
		fprintf(stderr, "Continuing in 5 seconds without screen initialization.\n");
		my_pause(500);
	}
	else
	{
		int charcode;
		for (int row = 0; row < 30; row++)
		{
			for (int col = 0; col < 40; col++)
			{
				if (fscanf(initfile, "%d", &charcode) != 0)
				{
					__current_smem[row][col] = charcode;
				}
			}
		}
	}
}

int main()
{
	int current_frame = 1;
	char frame_location[27];
	sprintf(frame_location, "../smemframes/frame%d.mem", current_frame);

	initialize_IO(frame_location);

	// Read smem (copy of initialize_smem, just doesn't use same array)
	read_smem(frame_location);

	int oldrow, oldcol;
	int row = 27, col = 21, key = 0, keynew;
	int playing = 1;
	int accelX = 0;
	long period;

	while (playing)
	{
		oldrow = row;
		oldcol = col;
		keynew = pause_and_getkey(50 - (current_frame << 1) - ((accelX >> 5) + (accelX >> 7)));
		accelX = get_accelX();

		// accelX * approx 438941 + 156128, or normalized_acceleration * avg(periods) + min(periods)
		period = (accelX << 18) + (accelX << 17) + (accelX << 15) + (accelX << 13) + (accelX << 12) + (accelX << 9) + (accelX << 7) + (accelX << 5) + 156128;
		// Normalize based on max val of accelX (32*15)
		period = (period >> 9) + (period >> 13) + (period >> 17) + (period >> 21);
		put_sound(period);

		if (keynew != 0)
			key = keynew;
		switch (key)
		{
		case 1: // a
			col--;
			break;
		case 2: // q
			col--;
			row--;
			break;
		case 3: // w
			row--;
			break;
		case 4: // e
			col++;
			row--;
			break;
		case 5: // d
			col++;
			break;
		}

		// Adjust locations and rerender if necessary; check for win (row = 29 & current frame is 9)
		if (row > 29)
			row = 29;
		if (row < 0)
		{
			row = 29;
			if (current_frame < 9)
				current_frame++;
			else
			{
				playing = 0;
				continue;
			}
			sprintf(frame_location, "../smemframes/frame%d.mem", current_frame);
			initialize_smem(frame_location);
			read_smem(frame_location);
		}
		if (col > 39)
			col = 39;
		if (col < 0)
			col = 0;

		// Check for collision and death
		if (getChar_atXY(col, row) == 1) // Red
		{
			initialize_smem("../smemframes/death.mem");
			playing = 0;
			continue;
		}

		put_leds(1 << (current_frame - 1));
		putChar_atXY(3, col, row);
		putChar_atXY(__current_smem[oldrow][oldcol], oldcol, oldrow);
	}
}

// The file below has the implementation of all of the helper functions
#include "no_procs.c"
