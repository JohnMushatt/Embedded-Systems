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
	current_note_index = 0;
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

	//play_note(song[current_note_index]);
	timer_on = 1;
	//play_notes=1;
	runtimerA2();
	play_note_now = 1;

}
void play_note(Note note) {

	// Initialize PWM output on P3.5, which corresponds to TB0.5
	P3SEL |= BIT5; // Select peripheral output mode for P3.5
	P3DIR |= BIT5;

	TB0CTL = (TBSSEL__ACLK | ID__1 | MC__UP); // Configure Timer B0 to use ACLK, divide by 1, up mode
	TB0CTL &= ~TBIE;           // Explicitly Disable timer interrupts for safety

	// Now configure the timer period, which controls the PWM period
	// Doing this with a hard coded values is NOT the best method
	// We do it here only as an example. You will fix this in Lab 2.
	TB0CCR0 = note.pitch;                    // Set the PWM period in ACLK ticks
	TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts

	// Configure CC register 5, which is connected to our PWM pin TB0.5
	TB0CCTL5 = OUTMOD_7;                   // Set/reset mode for PWM
	TB0CCTL5 &= ~CCIE;                     // Disable capture/compare interrupts
	TB0CCR5 = TB0CCR0 / 2;                  // Configure a 50% duty cycle
	/*
	 timer_on = 1;
	 runtimerA2();
	 while (timer <= note.duration) {

	 }
	 timer_on = 0;
	 stoptimerA2(1);
	 */
	//current_note_index++;
	configLabBoardLED((char) note.led);
	play_note_now = 0;
}

int score_note(unsigned int button) {
	unsigned int note_led = song[current_note_index].led;
	if (note_led == button) {
		score++;
	} else {
		score--;
	}
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "Current Score",
	AUTO_STRING_LENGTH, 50, 30,
	TRANSPARENT_TEXT);
	char score_string[3];
	sprintf(score_string, "%d", score);
	Graphics_drawStringCentered(&g_sContext, score_string, AUTO_STRING_LENGTH,
			50, 50,
			TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
	return score;
}
unsigned int convert_freq(float freq) {
	float period = 1 / freq;
	return (unsigned int) (period * 32768);
}
void init_song() {

	Note Pause = { 0, 100 };

	Note A_3_eighth = { convert_freq(220.00), 33, BIT3 };
	Note A_4_eighth = { convert_freq(440.00), 33, BIT3 };
	Note A_5_eighth = { convert_freq(880.00), 33, BIT3 };
	Note A_5_eighth_bpm80 = { A_5_eighth.pitch, 25, BIT3 };
	Note F_4_eighth = { convert_freq(349.23), 33, BIT3 };

	Note G_3_eighth = { convert_freq(196.0), 33, BIT0 };
	Note G_3_sharp_eighth = { convert_freq(207.65), 33, BIT0 };
	Note G_4_eighth = { convert_freq(392.00	), 33, BIT0 };
	Note G_5_eighth = { convert_freq(783.99	),5.5555555555,BIT0};
	Note C_4_eighth = { convert_freq(261.63), 33, BIT1 };
	Note C_5_eighth = { convert_freq(523.25), 33, BIT1 };

	Note C_5_eighth_bpm80 = { C_5_eighth.pitch, 25, BIT1 };
	Note D_4_sharp_eighth = { convert_freq(311.13), 33, BIT2 };
	Note D_4_eighth = { convert_freq(293.66), 33, BIT2 };

	Note B_3_eighth = { convert_freq(246.94), 33, BIT1 };

	Note E_5_eighth = { convert_freq(659.25), 33, BIT2 };
	Note E_5_eighth_bpm80 = { E_5_eighth.pitch, 25, BIT2 };
	Note E_4_eighth = { convert_freq(329.63), 33, BIT2 };
	Note E_3_eighth = { convert_freq(164.81), 33, BIT2 };
	/*
	 * Measure 1
	 */
	song[0] = G_3_eighth;
	song[1] = C_4_eighth;
	song[2] = D_4_sharp_eighth;

	song[3] = G_3_eighth;
	song[4] = C_4_eighth;
	song[5] = D_4_sharp_eighth;

	song[6] = G_3_eighth;
	song[7] = C_4_eighth;
	song[8] = D_4_sharp_eighth;

	song[9] = G_3_eighth;
	song[10] = C_4_eighth;
	song[11] = D_4_sharp_eighth;
	/*
	 * Measure 2
	 */
	song[12] = G_3_eighth;
	song[13] = C_4_eighth;
	song[14] = D_4_sharp_eighth;

	song[15] = G_3_eighth;
	song[16] = C_4_eighth;
	song[17] = D_4_sharp_eighth;

	song[18] = G_3_eighth;
	song[19] = C_4_eighth;
	song[20] = D_4_sharp_eighth;

	song[21] = G_3_eighth;
	song[22] = C_4_eighth;
	song[23] = D_4_sharp_eighth;
	/*
	 * Measure 3
	 */
	song[24] = G_3_sharp_eighth;
	song[25] = C_4_eighth;
	song[26] = D_4_sharp_eighth;

	song[27] = G_3_sharp_eighth;
	song[28] = C_4_eighth;
	song[29] = D_4_sharp_eighth;

	song[30] = G_3_sharp_eighth;
	song[31] = C_4_eighth;
	song[32] = F_4_eighth;

	song[33] = G_3_sharp_eighth;
	song[34] = C_4_eighth;
	song[35] = F_4_eighth;

	/*
	 * Measure 4
	 */
	song[36] = G_3_sharp_eighth;
	song[37] = B_3_eighth;
	song[38] = F_4_eighth;

	song[39] = G_3_sharp_eighth;
	song[40] = B_3_eighth;
	song[41] = E_4_eighth;

	song[42] = G_3_sharp_eighth;
	song[43] = B_3_eighth;
	song[44] = D_4_eighth;

	song[45] = E_3_eighth;
	song[46] = G_3_sharp_eighth;
	song[47] = D_4_eighth;

	/*
	 * Measure 5
	 */
	song[48] = A_3_eighth;
	song[49] = C_4_eighth;
	song[50] = E_4_eighth;

	song[51] = C_4_eighth;
	song[52] = E_4_eighth;
	song[53] = A_4_eighth;

	song[54] = E_4_eighth;
	song[55] = A_4_eighth;
	song[56] = C_5_eighth;

	song[57] = A_4_eighth;
	song[58] = C_5_eighth;
	song[59] = E_5_eighth;

	/*
	 * Measure 6
	 */

	song[60] = C_5_eighth;
	song[61] = E_5_eighth;
	song[62] = A_5_eighth;
	/*
	 * Song boosts up from 60bpm to 80bpm
	 */
	song[63] = C_5_eighth_bpm80;
	song[64] = E_5_eighth_bpm80;
	song[65] = A_5_eighth_bpm80;

	song[66] = C_5_eighth_bpm80;
	song[67] = E_5_eighth_bpm80;
	song[68] = A_5_eighth_bpm80;

	/*
	 * Song boosts up from 80bpm to 100bpm
	 */
	C_5_eighth_bpm80.duration = 20;
	E_5_eighth_bpm80.duration = 20;
	A_5_eighth_bpm80.duration = 20;
	song[69] = C_5_eighth_bpm80;
	song[70] = E_5_eighth_bpm80;
	song[71] = A_5_eighth_bpm80;
	/*
	 * Song boosts up from 100bpm to 120bpm
	 */
	C_5_eighth_bpm80.duration = 16.666666666;
	E_5_eighth_bpm80.duration = 16.666666666;
	A_5_eighth_bpm80.duration = 16.666666666;
	song[72] = C_5_eighth_bpm80;
	song[73] = E_5_eighth_bpm80;
	song[74] = A_5_eighth_bpm80;

	song[75] = C_5_eighth_bpm80;
	song[76] = E_5_eighth_bpm80;
	song[77] = A_5_eighth_bpm80;
	/*
	 * Song boosts up from 120bpm to 150bpm
	 */
	C_5_eighth_bpm80.duration = 13.333333333;
	E_5_eighth_bpm80.duration = 13.333333333;
	A_5_eighth_bpm80.duration = 13.333333333;
	song[78] = C_5_eighth_bpm80;
	song[79] = E_5_eighth_bpm80;
	song[80] = A_5_eighth_bpm80;

	song[81] = C_5_eighth_bpm80;
	song[82] = E_5_eighth_bpm80;
	song[83] = A_5_eighth_bpm80;

	/*
	 * Song boosts up from 150bpm to 180bpm
	 */
	C_5_eighth_bpm80.duration = 11.111111111;
	E_5_eighth_bpm80.duration = 11.111111111;
	A_5_eighth_bpm80.duration = 11.111111111;

	song[84] = C_5_eighth_bpm80;
	song[85] = E_5_eighth_bpm80;
	song[86] = A_5_eighth_bpm80;

	song[87] = C_5_eighth_bpm80;
	song[88] = E_5_eighth_bpm80;
	song[89] = A_5_eighth_bpm80;

	/*
	 * Song boosts up from 180bpm to 210bpm
	 */

	C_5_eighth_bpm80.duration = 9.523809523;
	E_5_eighth_bpm80.duration = 9.523809523;
	A_5_eighth_bpm80.duration = 9.523809523;

	song[90] = C_5_eighth_bpm80;
	song[91] = E_5_eighth_bpm80;
	song[92] = A_5_eighth_bpm80;

	song[93] = C_5_eighth_bpm80;
	song[94] = E_5_eighth_bpm80;
	song[95] = A_5_eighth_bpm80;

	/*
	 * Song boosts up from 210bpm to 240bpm
	 */
	C_5_eighth_bpm80.duration = 8.333333333;
	E_5_eighth_bpm80.duration = 8.333333333;
	A_5_eighth_bpm80.duration = 8.333333333;

	song[96] = C_5_eighth_bpm80;
	song[97] = E_5_eighth_bpm80;
	song[98] = A_5_eighth_bpm80;

	song[99] = C_5_eighth_bpm80;
	song[100] = E_5_eighth_bpm80;
	song[101] = A_5_eighth_bpm80;

	/*
	 * Song boosts up from 240bpm to 280bpm
	 */
	C_5_eighth_bpm80.duration = 7.142857142;
	E_5_eighth_bpm80.duration = 7.142857142;
	A_5_eighth_bpm80.duration = 7.142857142;

	song[102] = C_5_eighth_bpm80;
	song[103] = E_5_eighth_bpm80;
	song[104] = A_5_eighth_bpm80;

	song[105] = C_5_eighth_bpm80;
	song[106] = E_5_eighth_bpm80;
	song[107] = A_5_eighth_bpm80;

	song[108] = C_5_eighth_bpm80;
	song[109] = E_5_eighth_bpm80;
	song[110] = A_5_eighth_bpm80;
	/*
	 * Song slows down from 280bpm to 180bpm
	 */
	C_5_eighth_bpm80.duration = 5.5555555555;
	E_5_eighth_bpm80.duration = 5.5555555555;
	A_5_eighth_bpm80.duration = 5.5555555555;
	Pause.duration = 11.111111111;

	song[111] = Pause;
	song[112] = A_5_eighth_bpm80;
	song[113] = E_5_eighth_bpm80;
	song[114] = C_5_eighth_bpm80;

	song[115] = Pause;
	song[116] = A_5_eighth_bpm80;
	song[117] = E_5_eighth_bpm80;
	song[118] = C_5_eighth_bpm80;

	song[119] = Pause;
	song[120] = A_5_eighth_bpm80;
	song[121] = E_5_eighth_bpm80;
	song[122] = C_5_eighth_bpm80;

	song[123] = Pause;
	song[124] = A_5_eighth_bpm80;
	song[125] = E_5_eighth_bpm80;
	song[126] = C_5_eighth_bpm80;

	song[127] = Pause;
	song[128] = A_5_eighth_bpm80;
	song[129] = E_5_eighth_bpm80;
	song[130] = C_5_eighth_bpm80;

	song[131] = Pause;
	song[132] = A_5_eighth_bpm80;
	song[133] = E_5_eighth_bpm80;
	song[134] = C_5_eighth_bpm80;

	song[135] = Pause;
	song[136] = A_5_eighth_bpm80;
	song[137] = E_5_eighth_bpm80;
	song[138] = C_5_eighth_bpm80;

	song[139] = Pause;
	song[140] = A_5_eighth_bpm80;
	song[141] = E_5_eighth_bpm80;
	song[142] = C_5_eighth_bpm80;
	/*
	 * Lower some
	 */
	B_3_eighth.duration = 5.5555555555;
	song[143] = Pause;
	song[144] = A_5_eighth_bpm80;
	song[145] = E_5_eighth_bpm80;
	song[146] = B_3_eighth;

	song[147] = Pause;
	song[148] = A_5_eighth_bpm80;
	song[149] = E_5_eighth_bpm80;
	song[150] = B_3_eighth;

	song[151] = Pause;
	song[152] = A_5_eighth_bpm80;
	song[153] = E_5_eighth_bpm80;
	song[154] = B_3_eighth;

	song[151] = Pause;
	song[152] = G_5_eighth;
	song[153] = E_5_eighth_bpm80;
	song[154] = B_3_eighth;




	song[155] = Pause;
	song[156] = G_5_eighth;
	song[157] = E_5_eighth_bpm80;
	song[158] = B_3_eighth;

	song[159] = Pause;
	song[160] = G_5_eighth;
	song[161] = E_5_eighth_bpm80;
	song[162] = B_3_eighth;

	song[163] = Pause;
	song[164] = G_5_eighth;
	song[165] = E_5_eighth_bpm80;
	song[166] = B_3_eighth;

	song[167] = Pause;
	song[168] = G_5_eighth;
	song[169] = E_5_eighth_bpm80;
	song[170] = B_3_eighth;
	/*
	 * Back to normal
	 */

	song[171] = Pause;
	song[172] = A_5_eighth_bpm80;
	song[173] = E_5_eighth_bpm80;
	song[174] = C_5_eighth_bpm80;

	song[175] = Pause;
	song[176] = A_5_eighth_bpm80;
	song[177] = E_5_eighth_bpm80;
	song[178] = C_5_eighth_bpm80;

	song[179] = Pause;
	song[180] = A_5_eighth_bpm80;
	song[181] = E_5_eighth_bpm80;
	song[182] = C_5_eighth_bpm80;

	song[183] = Pause;
	song[184] = A_5_eighth_bpm80;
	song[185] = E_5_eighth_bpm80;
	song[186] = C_5_eighth_bpm80;

	song[187] = Pause;
	song[188] = A_5_eighth_bpm80;
	song[189] = E_5_eighth_bpm80;
	song[190] = C_5_eighth_bpm80;
	/*
	 song[113] = Pause;
	 song[114] = A_5_eighth_bpm80;

	 song[115] = Pause;
	 song[116] = A_5_eighth_bpm80;

	 song[117] = Pause;
	 song[118] = A_5_eighth_bpm80;

	 song[119] = Pause;
	 song[120] = A_5_eighth_bpm80;

	 song[121] = Pause;
	 song[122] = A_5_eighth_bpm80;

	 song[123] = Pause;
	 song[124] = A_5_eighth_bpm80;

	 song[125] = Pause;
	 song[126] = A_5_eighth_bpm80;
	 */
}
#pragma vector=TIMER2_A0_VECTOR
__interrupt void Timer_A2_ISR(void) {
	if (timer_on) {

		if (leap_cnt < 1024) {
			timer++;
			leap_cnt++;
			elasped_time++;
		} else {
			timer += 2;
			elasped_time += 2;
			leap_cnt = 0;
		}
		if (play_notes && current_note_index <= 190) {
			if (play_note_now) {
				BuzzerOn();
				play_note(song[current_note_index]);
			}

			else if (elasped_time > song[current_note_index].duration) {
				current_note_index++;
				elasped_time = 0;
				BuzzerOff();
				play_note_now = 1;
				configLabBoardLED(0);

			}
		}
		else {
			current_note_index=0;
		}

	}

}
