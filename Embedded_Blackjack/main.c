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
#include "blackjack.h"
// Function Prototypes
void swDelay(char numLoops);

// Declare globals here

// Main
void main(void)

{
	unsigned char currKey = 0, dispSz = 3;
	unsigned char dispThree[3];

	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer. Always need to stop this!!
							  // You can then configure it properly, if desired

	// Useful code starts here
	initLeds();

	configDisplay();
	configKeypad();

	// *** Intro Screen ***
	Graphics_clearDisplay(&g_sContext); // Clear the display

	// Write some text to the display
	Graphics_drawStringCentered(&g_sContext, "MSP430 Blackjack",
	AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
	Graphics_drawStringCentered(&g_sContext, "Press * to start",
	AUTO_STRING_LENGTH, 48, 25,
	TRANSPARENT_TEXT);
	//Graphics_drawStringCentered(&g_sContext, "ECE2049-C20!", AUTO_STRING_LENGTH,
	//		48, 35, TRANSPARENT_TEXT);

	// Draw a box around everything because it looks nice
	//Graphics_Rectangle box = { .xMin = 5, .xMax = 91, .yMin = 5, .yMax = 91 };
	//Graphics_drawRectangle(&g_sContext, &box);

	// We are now done writing to the display.  However, if we stopped here, we would not
	// see any changes on the actual LCD.  This is because we need to send our changes
	// to the LCD, which then refreshes the display.
	// Since this is a slow operation, it is best to refresh (or "flush") only after
	// we are done drawing everything we need.
	Graphics_flushBuffer(&g_sContext);

	dispThree[0] = ' ';
	dispThree[2] = ' ';
	int active_game = 0;
	int new_move = 0;
	bet_made = 0;
	while (!game_over)    // Forever loop
	{
		// Check if any keys have been pressed on the 3x4 keypad
		currKey = getKey();
		if (active_game == 0 && currKey == '*') {
			active_game = 1;
			start_game();
			display_game();
		}

		//Enter main game loop
		else if (active_game) {
			if (new_move) {
				display_game();
			}
			//Make bet
			if (!bet_made) {
				swDelay(5);

				bet_made = 1;
				make_bet();
			}

			//Deal cards
			else {
				play_round();
				active_game = 0;
				// *** Intro Screen ***
				Graphics_clearDisplay(&g_sContext); // Clear the display

				// Write some text to the display
				Graphics_drawStringCentered(&g_sContext, "MSP430 Blackjack",
				AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
				Graphics_drawStringCentered(&g_sContext, "Press * to start",
				AUTO_STRING_LENGTH, 48, 25,
				TRANSPARENT_TEXT);
				Graphics_flushBuffer(&g_sContext);

			}
			/*
			 else if(currKey == '1' || currKey == '2' || currKey == '4' || currKey == '8') {
			 char arg[1];
			 arg[0]= currKey;
			 int bet = atoi(arg);
			 make_bet(bet);

			 }
			 */
		}
		/*
		 //Need to get inputs 0 - 15
		 if ((currKey >= '0') && (currKey <= '9'))
		 cut_deck(playing_deck, (int)currKey);
		 //setLeds(currKey - 0x30);

		 if (currKey) {
		 dispThree[1] = currKey;
		 // Draw the new character to the display
		 Graphics_drawStringCentered(&g_sContext, dispThree, dispSz, 48, 55,
		 OPAQUE_TEXT);

		 // Refresh the display so it shows the new data
		 Graphics_flushBuffer(&g_sContext);

		 // wait awhile before clearing LEDs
		 swDelay(1);
		 setLeds(0);
		 }
		 */

	}  // end while (1)
}

