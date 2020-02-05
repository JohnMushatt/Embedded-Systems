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
	unsigned char mask = 0;
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
