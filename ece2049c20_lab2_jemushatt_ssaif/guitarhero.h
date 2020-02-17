/*
 * guitarhero.h
 *
 *  Created on: Feb 5, 2020
 *      Author: johnm
 */

#ifndef GUITARHERO_H_
#define GUITARHERO_H_
#include <msp430.h>
#include <stdio.h>
#include <stdlib.h>
#include "peripherals.h"

/**
 * 6 byte structure
 * 2 bytes for pitch b/c 16-bit unsigned int is 16/8 =2  bytes
 * 4 bytes for duration b/c 32-bit is 4 bytes
 * maybe some additional padding
 */
typedef struct _Note {
	unsigned int pitch; // 2 bytes

	float duration; // 4 bytes

	unsigned int led; //2 BytesBIT0
} Note;
Note song[200];
unsigned int current_note_index;
unsigned int timer;
unsigned int elasped_time;
int play_notes;
int play_note_now;
int timer_on;
unsigned int leap_cnt;
int score;
int game_started;


/*
 * Game logic
 */

int score_note(unsigned int button);

void start_game();
void init_song();
unsigned int convert_freq(float freq);
void play_note(Note note);
void runtimerA2(void);
void stoptimerA2(int reset);
void init_selector_buttons();
unsigned char get_pressed_button();
unsigned char get_pressed_button_hex();
void configUserLED(char inbits);
void configLabBoardLED(char inbits);
#endif /* GUITARHERO_H_ */
