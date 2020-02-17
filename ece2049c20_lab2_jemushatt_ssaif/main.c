/************** ECE2049 DEMO CODE ******************/
/**************  13 March 2019   ******************/
/***************************************************/

#include <msp430.h>

/* Peripherals.c and .h are where the functions that implement
 * the LEDs and keypad, etc are. It is often useful to organize
 * your code by putting like functions together in files.
 * You include the header associated with that file(s)
 * into the main file of your project. */
#include "peripherals.h"
#include "guitarhero.h"
// Function Prototypes
void swDelay(char numLoops);

// Declare globals here

// Main
void main(void)

{
	//BuzzerOn();

	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer. Always need to stop this!!
							  // You can then configure it properly, if desired
	_BIS_SR(GIE);
	// Useful code starts here
	initLeds();
	init_selector_buttons();

	configDisplay();
	configKeypad();
	// *** Intro Screen ***
	Graphics_clearDisplay(&g_sContext); // Clear the display

	Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48,
			15, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);

	unsigned char button;

	init_song();
	char current_score[3];
	unsigned int selector_buttons;
	while (1)    // Forever loop
	{

		button = getKey();
		selector_buttons = get_pressed_button_hex();
		if (button == '*') {
			sprintf(current_score, "%d",0);
			start_game();

			 //dispThree[1] = button;

			 Graphics_clearDisplay(&g_sContext);
			 Graphics_drawStringCentered(&g_sContext, "Current Score",AUTO_STRING_LENGTH, 50, 30,
			 TRANSPARENT_TEXT);
			 Graphics_drawStringCentered(&g_sContext,current_score ,AUTO_STRING_LENGTH, 50, 50,
						 TRANSPARENT_TEXT);
			 Graphics_flushBuffer(&g_sContext);

		}
		if (game_started) {

			play_notes = 1;
			if (selector_buttons != 0) {
				score_note(selector_buttons);
			}
		}

		//unsigned char val = get_pressed_button_hex();
		//configLabBoardLED((char) selector_buttons);
		//configUserLED(val);
	}  // end while (1)
}

void swDelay(char numLoops) {
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i, j;	// volatile to prevent removal in optimization
								// by compiler. Functionally this is useless code

	for (j = 0; j < numLoops; j++) {
		i = 50000;					// SW Delay
		while (i > 0)				// could also have used while (i)
			i--;
	}
}
