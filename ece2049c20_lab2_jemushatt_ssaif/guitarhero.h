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
 * 4 byte structure
 * 2 bytes for pitch b/c 16-bit unsigned int is 16/8 =2  bytes
 * 2 bytes for duration b/c 16-bit unsigned int is 16/8 =2  bytes
 * maybe some additional padding
 */
typedef struct _Note {
	unsigned int pitch; // 2 bytes
	unsigned int duration; // 2 bytes
} Note;
Note song[100];
unsigned int current_note_index;
unsigned int timer;
int timer_on;
unsigned int leap_cnt;

int game_started;
/**
 *  8 + N*5 byte structure
 *  8 Bytes for pointer to array of notes
 *  N*5 bytes for N notes in the array
 */
/*
typedef struct _Song {
	Note *Notes;

} Song;
*/
/*
 * Game logic
 */

void start_game();
void init_song();
void play_note(Note note);
void runtimerA2(void);
void stoptimerA2(int reset);
void init_selector_buttons();
unsigned char get_pressed_button();
unsigned char get_pressed_button_hex();
void configUserLED(char inbits);
void configLabBoardLED(char inbits);
#endif /* GUITARHERO_H_ */
