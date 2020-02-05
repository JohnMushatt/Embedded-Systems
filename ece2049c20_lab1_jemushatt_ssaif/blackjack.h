/*
 * blackjack.h
 *
 *  Created on: Jan 28, 2020
 *      Author: johnm
 */

#ifndef BLACKJACK_H_
#define BLACKJACK_H_
#include <stdlib.h>
#include <stdio.h>
#include "peripherals.h"
#include <msp430.h>

#define DECK_SIZE 52
#define WIN_AMOUNT 21



void swDelay(char numLoops);


typedef struct _Card {
	enum Face {
		Heart, Diamond, Spade, Club
	};
	enum Face face;
	enum Suit {
		red, black
	};
	enum Suit suit;
	unsigned int value;
	int in_play;
	int in_hand;

} card;
typedef struct player {
	card hand[14];
	int score;
	int balance;
	int num_cards;
} player_hand;
//Stack allocated deck
card playing_deck[52];
int deal_order[52];
player_hand player;
player_hand dealer;
int current_pot;
int game_over;
int bet_made;
int active_game;
/*
 * Card and deck related functions
 */
void init_deck();

void deal_card(int choice);

void cut_deck(int num);

void init_hands();

void play_round();
//--------------------------
void start_game();

void restart_game();
/*
 * Graphics related functions
 */
void display_game();
void prompt_deal();

int get_card_value(int val);

void player_lose();

void player_win();
char *card_as_string(card card);

void setLedHex(unsigned char state);
/*
 * Betting related functions
 */
void make_bet();
int cpu_bet(int player_bet);
void bet_prompt();
void display_bets(int player_bet, int dealer_bet);
void display_dealer_hand();
#endif /* BLACKJACK_H_ */
