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

	// We are now done writing to the display.  However, if we stopped here, we would not
	// see any changes on the actual LCD.  This is because we need to send our changes
	// to the LCD, which then refreshes the display.
	// Since this is a slow operation, it is best to refresh (or "flush") only after
	// we are done drawing everything we need.
	Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48,
			15, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
	//Graphics_clearDisplay(&g_sContext);
	/*
	 Graphics_drawStringCentered(&g_sContext, "Time start", AUTO_STRING_LENGTH,
	 30, 30,
	 TRANSPARENT_TEXT);
	 Graphics_flushBuffer(&g_sContext);

	 timer_on = 1;
	 runtimerA2();
	 char timer_as_string[10];
	 while (timer <= 200) {
	 if (timer % 100 == 0) {
	 sprintf(timer_as_string, "%d", timer / 100);
	 Graphics_clearDisplay(&g_sContext);
	 Graphics_drawString(&g_sContext, timer_as_string,
	 AUTO_STRING_LENGTH, 30, 30,
	 OPAQUE_TEXT);
	 Graphics_flushBuffer(&g_sContext);
	 }
	 }

	 stoptimerA2(1);
	 Graphics_clearDisplay(&g_sContext);
	 Graphics_drawString(&g_sContext, "Timer Over!",

	 AUTO_STRING_LENGTH, 30, 30,
	 OPAQUE_TEXT);
	 Graphics_flushBuffer(&g_sContext);
	 unsigned char dispThree[3];
	 dispThree[0] = ' ';
	 dispThree[2] = ' ';
	 char test = BIT0 | BIT1;
	 configUserLED(test);
	 swDelay(2);
	 test = BIT1;
	 configUserLED(test);
	 swDelay(2);

	 test = BIT0;
	 configUserLED(test);
	 swDelay(2);
	 setLeds(0);
	 */
	unsigned char button;
	Note test_note_1;
	test_note_1.duration = 2;
	test_note_1.pitch = 200;

	Note test_note_2;
	test_note_2.duration = 1;
	test_note_2.pitch = 50;
	init_song();
	while (1)    // Forever loop
	{

		button = getKey();
		if (button == '*') {
			start_game();
			/*
			 dispThree[1] = button;

			 Graphics_clearDisplay(&g_sContext);
			 Graphics_drawStringCentered(&g_sContext, dispThree, 3, 30, 30,
			 TRANSPARENT_TEXT);
			 Graphics_flushBuffer(&g_sContext);
			 */
		}
		if (game_started) {
			if (current_note_index <= 6) {
				play_note(song[current_note_index]);
			}
		}
		//unsigned char val = get_pressed_button_hex();
		//configLabBoardLED((char) val);
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