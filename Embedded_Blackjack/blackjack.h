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
#define DECK_SIZE 52
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

} card;
//card *deck;

void init_deck(card **deck);

void add_card(card *card);

card *remove_card(card *card);

int check_card(card *card);

card *deal_card();
#endif /* BLACKJACK_H_ */
