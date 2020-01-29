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
typedef struct card {
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

} card;
typedef struct player {
	card hand[14];
	int score;
	int balance;
} player_hand;
//card *deck;

void init_deck(card deck[]);

void add_card(card deck[], card *card);

card *remove_card(card deck[]);

int check_card(card deck[], card *card);

int *cut_deck(int num);
int *start_game(card deck[],player_hand player, player_hand dealer);
card *deal_card();
#endif /* BLACKJACK_H_ */
