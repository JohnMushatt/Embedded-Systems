#include "msp430.h"
#include <math.h>
#include <stdlib.h>
#define MAX_PTS 300
// John Mushatt ECE box 648
/**
 * For the efficiency question:
 * Floating point storay is 2x as much space as floats are 32-bit while ints are 16-bit
 * Floating point instruction time is I believe ~13 assembly instructions while integer math is ~1
 * thus Integer instructions only take ~5 instruction cycles while floating point instructions take ~65 instruction cycles
 */
void main(void) {
	/*
	 // Give total size (in bytes) of these variables as declared.

	 //in[MAX_PTS] creates an statically allocated array of size MAX_PTS which is 300, same with parabola[MAX_PTS];
	 //Total size of both arrays should be 2 bytes (2 bytes per int) * MAX_PTS (300) = 600 bytes;
	 int in[MAX_PTS], parabola[MAX_PTS]; //
	 // At what address is alfa stored?
	 //alfa is stored 0x0043F6
	 //Create an integer alfa and assign a value of 4 to it
	 int alfa = 4; //

	 // Create an integer half_max and i, no value specified,
	 //so whatever value is already stored there will be stored in half_max and i

	 int half_max, i; //
	 //Disable the watchdog timer to prevent system restart
	 WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	 //Assign half_max the value of 300/2 = 150
	 half_max = MAX_PTS / 2;
	 //Loop for 300 times (value of MAX_PTS)
	 for (i = 0; i < MAX_PTS; i++) {
	 //the value in array in at index i is set to i-half_max
	 in[i] = i - half_max;
	 //The value in array parabola at index i is set to in[i] - alfa) * (in[i] - alfa)
	 parabola[i] = (in[i] - alfa) * (in[i] - alfa); //
	 }
	 */

	// Give total size (in bytes) of these variables as declared
	//Creates 2 floating point arrays of size 1200
	// 4 bytes per float so 4 * 300 = 1200 bytes per array
	float in_x[MAX_PTS], parab[MAX_PTS]; //
	// At what address is beta stored?
	//Float beta is stored at 0x0043F6
	//Also declares a floating point variable beta
	float beta; //
	int i = 0; //
	int a = 2 + 4;
	//Stops watchdog timer to prevent system reset
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	//Assign berta a floating point value of 4.0
	beta = 4.0;
	//Loop for 300 times
	while (i < MAX_PTS) {
		//Assigned the value at in_x[i] to be the result of the index-150, then cast that value to a float
		in_x[i] = (float) (i - MAX_PTS / 2); //
		//Assign the value at parab[i] to be the result of taking the value at in_x[i[ - beta, then squaring that value by 2
		//The version the compiler uses should be the version taking two double floating point arguments
		//And it outputs a 64 bit-float as a double
		parab[i] = pow((in_x[i] - beta), 2.0); //
		//Increment the index by 1
		i++; //
	}

}

