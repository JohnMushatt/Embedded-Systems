/*
 * guitarhero.h
 *
 *  Created on: Feb 5, 2020
 *      Author: johnm
 */

#ifndef GUITARHERO_H_
#define GUITARHERO_H_
#include <msp430.h>

/**
 * 5 byte structure
 * 2 bytes for pitch b/c 16-bit unsigned int is 16/8 =2  bytes
 * 2 bytes for duration b/c 16-bit unsigned int is 16/8 =2  bytes
 * 1 byte for LED b.c 8-bit unsigned char is 1 byte
 * maybe some additional padding
 */
typedef struct _Note {
	unsigned int pitch; // 2 bytes
	unsigned int duration; // 2 bytes
	unsigned char LED; // 1 byte
} Note;
Note song[100];
unsigned int timer;
int timer_on;
unsigned int leap_cnt;
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

void runtimerA2(void);
void stoptimerA2(int reset);
void init_selector_buttons();
unsigned char get_pressed_button();
unsigned char get_pressed_button_hex();
void configUserLED(char inbits);
void configLabBoardLED(char inbits);
#endif /* GUITARHERO_H_ */
