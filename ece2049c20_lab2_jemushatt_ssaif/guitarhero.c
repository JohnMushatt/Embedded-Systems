/*
 * guitarhero.c
 *
 *  Created on: Feb 5, 2020
 *      Author: johnm
 */

#include "guitarhero.h"

void init_selector_buttons() {

	P7SEL &= ~(BIT0 | BIT4);
	P3SEL &= ~BIT6;
	P2SEL &= ~BIT2;

	P7DIR |= ~(BIT0 | BIT4);
	P3DIR |= ~BIT6;
	P2DIR |= ~BIT2;

	P7REN |= BIT0 | BIT4;
	P3REN |= BIT6;
	P2REN |= BIT2;

	P7OUT |= BIT0 | BIT4;
	P3OUT |= BIT6;
	P2OUT |= BIT2;

}
unsigned char get_pressed_button() {

	if (!(P7IN & BIT4)) {
		return '4';
	} else if (!(P2IN & BIT2)) {
		return '3';
	} else if (!(P3IN & BIT6)) {
		return '2';
	} else if (!(P7IN & BIT0)) {
		return '1';
	} else {
		return '0';
	}
}

unsigned char get_pressed_button_hex() {
	unsigned char mask = 0;
	//If far right button
	if (!(P7IN & BIT4)) {
		mask |= BIT3;
	}
	if (!(P2IN & BIT2)) {
		mask |= BIT2;
	}
	if (!(P3IN & BIT6)) {
		mask |= BIT1;
	}
	if (!(P7IN & BIT0)) {
		mask |= BIT0;
	}

	return mask;
}
void configUserLED(char inbits) {

	P1SEL &= ~(BIT0);
	P1DIR |= (BIT0);

	P4SEL &= ~(BIT7);
	P4DIR |= (BIT7);

	P4OUT &= ~(BIT7);
	P1OUT &= ~(BIT0);
	//unsigned char mask = 0;
	if (inbits & BIT0) {
		P1OUT |= BIT0;
	}
	if (inbits & BIT1) {
		P4OUT |= BIT7;
	}

}
void configLabBoardLED(char inbits) {
	unsigned char mask = 0;
	if (inbits & BIT0) {
		mask |= BIT3;
	}
	if (inbits & BIT1) {
		mask |= BIT2;
	}
	if (inbits & BIT2) {
		mask |= BIT1;
	}
	if (inbits & BIT3) {
		mask |= BIT0;
	}
	setLeds(mask);

}

void runtimerA2(void) {
// This function configures and starts Timer A2
// Timer is counting ~0.01 seconds
//
// Input: none, Output: none
//
// smj, ECE2049, 17 Sep 2013
//
// Use ACLK, 16 Bit, up mode, 1 divider
	TA2CTL = TASSEL_1 + MC_1 + ID_0;
	TA2CCR0 = 327; // 327+1 = 328 ACLK tics = ~1/100 seconds
	TA2CCTL0 = CCIE; // TA2CCR0 interrupt enabled
}
void stoptimerA2(int reset) {
// This function stops Timer A2 andresets the global time variable
// if input reset = 1
//
// Input: reset, Output: none
//
// smj, ECE2049, 17 Sep 2013
//
	TA2CTL = MC_0; // stop timer
	TA2CCTL0 &= ~CCIE; // TA2CCR0 interrupt disabled
	if (reset)
		timer = 0;
}

void start_game() {
	Graphics_clearDisplay(&g_sContext);
	current_note_index=0;
	timer_on = 1;
	runtimerA2();
	char timer_as_string[2];
	int seconds = 0;
	game_started = 1;
	while (timer <= 300) {
		if (timer % 100 == 0) {
			seconds = (300 - timer) / 100;
			sprintf(timer_as_string, "%d", seconds);
			Graphics_clearDisplay(&g_sContext);
			Graphics_drawString(&g_sContext, timer_as_string,
			AUTO_STRING_LENGTH, 30, 30, OPAQUE_TEXT);
			Graphics_flushBuffer(&g_sContext);
			if (seconds == 3) {
				configUserLED(BIT0);
			} else if (seconds == 2) {
				configUserLED(BIT1);
			} else if (seconds == 1) {
				configUserLED(BIT0);
			} else if (seconds == 0) {
				configUserLED(BIT0 | BIT1);
			}
		}
	}
	timer_on = 0;
	stoptimerA2(1);
	configUserLED(0);

}
void play_note(Note note) {

	// Initialize PWM output on P3.5, which corresponds to TB0.5
	    P3SEL |= BIT5; // Select peripheral output mode for P3.5
	    P3DIR |= BIT5;

	    TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
	    TB0CTL  &= ~TBIE;                       // Explicitly Disable timer interrupts for safety

	    // Now configure the timer period, which controls the PWM period
	    // Doing this with a hard coded values is NOT the best method
	    // We do it here only as an example. You will fix this in Lab 2.
	    TB0CCR0   = note.pitch;                    // Set the PWM period in ACLK ticks
	    TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts

	    // Configure CC register 5, which is connected to our PWM pin TB0.5
	    TB0CCTL5  = OUTMOD_7;                   // Set/reset mode for PWM
	    TB0CCTL5 &= ~CCIE;                      // Disable capture/compare interrupts
	    TB0CCR5   = TB0CCR0/2;                  // Configure a 50% duty cycle

	    timer_on = 1;
	    runtimerA2();
	    while(timer <= note.duration*100) {

	    }
	    timer_on = 0;
	    stoptimerA2(1);
	    BuzzerOff();
	    current_note_index++;


}
void init_song() {
	float f_por = 1 / 698.0;
	unsigned int f_tick = (unsigned int)(f_por* 32768);
	Note E = { f_tick,1};
	Note D = {587,1};
	Note C_note = {523,1};
	Note Pause = {0,1};
	Note F = {698,1};

	song[0] = E;
	song[1] = D;
	song[2] = C_note;
	song[3] = Pause;
	song[4] = F;
	song[5] = E;
	song[6] = D;
}
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void) {
	if (timer_on) {
		if (leap_cnt < 1024) {
			timer++;
			leap_cnt++;
		} else {
			timer += 2;
			leap_cnt = 0;
		}
	}

}
