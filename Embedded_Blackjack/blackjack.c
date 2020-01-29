/*
 * blackjack.c
 *
 *  Created on: Jan 28, 2020
 *      Author: johnm
 */

#include "blackjack.h"
void start_game() {

}
int cut_deck(card **deck, int num) {

}
void init_deck(card **deck) {

	/**
	 * 0-12 	Red Hearts
	 * 13-25 	Red Diamonds
	 * 26-38	Black Spade
	 * 39-51	Black Club
	 */

	for (unsigned int i = 0; i < 13; i++) {
		unsigned int card_val = i + 1;
		//Red Hearts
		deck[i] = (card *) malloc(sizeof(card));
		deck[i]->suit = red;
		deck[i]->face = Heart;
		deck[i]->value = card_val;

		//Red Diamonds
		deck[i + 13] = (card *) malloc(sizeof(card));
		deck[i + 13]->suit = red;
		deck[i + 13]->face = Diamond;
		deck[i + 13]->value = card_val;

		//Black Spade
		deck[i + 26] = (card *) malloc(sizeof(card));
		deck[i + 26]->suit = black;
		deck[i + 26]->face = Spade;
		deck[i + 26]->value = card_val;

		//Black Club
		deck[i + 39] = (card *) malloc(sizeof(card));
		deck[i + 39]->suit = black;
		deck[i + 39]->face = Club;
		deck[i + 39]->value = card_val;
	}

}

