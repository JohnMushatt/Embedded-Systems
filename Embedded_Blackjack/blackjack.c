/*
 * blackjack.c
 *
 *  Created on: Jan 28, 2020
 *      Author: johnm
 */

#include "blackjack.h"
int *start_game(card deck[],player_hand player, player_hand dealer) {
	//Clear display
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "Please enter", AUTO_STRING_LENGTH,
			48, 15, TRANSPARENT_TEXT);
	Graphics_drawStringCentered(&g_sContext, "a number", AUTO_STRING_LENGTH, 48,
			22, TRANSPARENT_TEXT);

	Graphics_drawStringCentered(&g_sContext, "between 0-15!",
	AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
	int deck_is_cut = 0;
	unsigned char currKey = 0;
	int digit_0 = -1;
	int digit_1 = -1;
	char num[2];
	int val = 0;
	while (!deck_is_cut) {
		currKey = getKey();
		if (currKey >= '0' && currKey <= '9') {

			//If we are still at digit 0
			if (digit_0 == -1) {
				digit_0++;
				num[0] = currKey;
			}
			//We are at digit 1
			else {
				digit_1++;
				num[1] = currKey;
				val = atoi(num);
				if (val > 15) {
					start_game(deck);
				}
				deck_is_cut = !deck_is_cut;
			}
		}
	}
	int *dealing_order = cut_deck(val);

	for(unsigned int i = 0; i < 2; i++) {
		//Remove card at top of deck or smallest index in index_list
		card player_card = remove_card(deck);
		player.hand[i] = player_card;
		card dealer_card = remove_card(deck);
		dealer.hand[i] = dealer_card;
	}
	return dealing_order;

}

int *cut_deck(int num) {
	//Seed rng
	srand(num);
	int *index_list = malloc(sizeof(int) * 52);
	//Generate 52 random unique indexes
	int seen = 0;
	for (unsigned int i = 0; i < 52; i++) {

		int index = rand() % 52;

		for (unsigned int j = 0; j < 52; j++) {

			if (index_list[j] == index) {

				j = 52;
				seen = 1;
			}

		}
		if (!seen) {
			index_list[i] = index;
		} else {
			i--;
		}
		seen = 0;
	}
	return index_list;
}
void init_deck(card deck[]) {

	/**
	 * 0-12 	Red Hearts
	 * 13-25 	Red Diamonds
	 * 26-38	Black Spade
	 * 39-51	Black Club
	 */

	for (unsigned int i = 0; i < 13; i++) {
		unsigned int card_val = i + 1;
		//Red Hearts

		deck[i].suit = red;
		deck[i].face = Heart;
		deck[i].value = card_val;
		deck[i].in_play = 0;

		//Red Diamonds
		deck[i + 13].suit = red;
		deck[i + 13].face = Diamond;
		deck[i + 13].value = card_val;
		deck[i + 13].in_play = 0;
		//Black Spade
		deck[i + 26].suit = black;
		deck[i + 26].face = Spade;
		deck[i + 26].value = card_val;
		deck[i + 26].in_play = 0;
		//Black Club
		deck[i + 39].suit = black;
		deck[i + 39].face = Club;
		deck[i + 39].value = card_val;
		deck[i + 39].in_play = 0;

	}

}

