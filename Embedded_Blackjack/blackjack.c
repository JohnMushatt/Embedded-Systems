/*
 * blackjack.c
 *
 *  Created on: Jan 28, 2020
 *      Author: johnm
 */

#include "blackjack.h"
void start_game() {
	//Allocate enough memory for 52 cards on the stack
	//card playing_deck[52];
	init_deck();

	//Alocate memory for 2 players
	init_hands();
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
	current_pot = 0;
	while (!deck_is_cut) {
		currKey = getKey();
		if (currKey >= '0' && currKey <= '9') {

			//If we are still at digit 0
			if (digit_0 == -1) {
				digit_0++;
				num[0] = currKey;
				swDelay(1);
				//currKey = '*';
			}
			//We are at digit 1
			else if (digit_0 == 0) {
				digit_1++;
				num[1] = currKey;
				val = atoi(num);
				if (val > 15) {
					start_game();
				}
				active_game = 1;
				deck_is_cut = !deck_is_cut;
				Graphics_clearDisplay(&g_sContext);
				Graphics_drawStringCentered(&g_sContext, "Game starting",
				AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
				Graphics_drawStringCentered(&g_sContext, "please enjoy!",
				AUTO_STRING_LENGTH, 48, 22, TRANSPARENT_TEXT);

				Graphics_flushBuffer(&g_sContext);
				swDelay(3);
				Graphics_clearDisplay(&g_sContext);

			}
		} else if (currKey == '*') {
			restart_game();
			deck_is_cut = 0;
			currKey = 0;
			digit_0 = -1;
			digit_1 = -1;
			val = 0;
			Graphics_clearDisplay(&g_sContext);
			Graphics_drawStringCentered(&g_sContext, "Please enter",
			AUTO_STRING_LENGTH, 48, 15, TRANSPARENT_TEXT);
			Graphics_drawStringCentered(&g_sContext, "a number",
			AUTO_STRING_LENGTH, 48, 22, TRANSPARENT_TEXT);

			Graphics_drawStringCentered(&g_sContext, "between 0-15!",
			AUTO_STRING_LENGTH, 48, 30, TRANSPARENT_TEXT);
			Graphics_flushBuffer(&g_sContext);
		}
	}
	cut_deck(val);

	for (unsigned int i = 0; i < 2; i++) {
		//Remove card at top of deck or smallest index in index_list
		deal_card(0);
		//player.hand[i] = player_card;
		deal_card(1);
		//dealer.hand[i] = dealer_card;
	}

}
void restart_game() {
	init_deck();
	init_hands();
	current_pot = 0;
	bet_made = 0;
	game_over = 0;
	active_game = 0;
	for (unsigned int i = 0; i < 52; i++) {
		deal_order[i] = -1;
	}
}
/**
 * Display the state of the game
 */
void display_game() {
	//Display the player's hand first

	Graphics_clearDisplay(&g_sContext);
	Graphics_drawString(&g_sContext, "Player's Hand:",
	AUTO_STRING_LENGTH, 0, 8, TRANSPARENT_TEXT);
	char *card_string;
	for (unsigned int i = 0; i < player.num_cards; i++) {
		card_string = card_as_string(player.hand[i]);
		Graphics_drawString(&g_sContext, card_string,
		AUTO_STRING_LENGTH, i * 20, 22, TRANSPARENT_TEXT);
		//Graphics_flushBuffer(&g_sContext);
		//free(card_string);
	}

	Graphics_drawString(&g_sContext, "Dealer's Hand:",
	AUTO_STRING_LENGTH, 0, 35, TRANSPARENT_TEXT);
	card_string = card_as_string(dealer.hand[0]);
	Graphics_drawString(&g_sContext, card_string,
	AUTO_STRING_LENGTH, 0, 47, TRANSPARENT_TEXT);

	Graphics_drawString(&g_sContext, "XXX", AUTO_STRING_LENGTH, 20, 47,
	TRANSPARENT_TEXT);

	Graphics_drawString(&g_sContext, "Score:",
	AUTO_STRING_LENGTH, 0, 80, TRANSPARENT_TEXT);

	char *score = (char *) malloc(sizeof(char) * 2);

	sprintf(score, "%d", player.score);

	Graphics_drawStringCentered(&g_sContext, score,
	AUTO_STRING_LENGTH, 45, 83, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);

	free(card_string);
	free(score);

}
void display_dealer_hand() {
	Graphics_clearDisplay(&g_sContext);

	Graphics_drawString(&g_sContext, "Dealers Hand:",
	AUTO_STRING_LENGTH, 0, 22, TRANSPARENT_TEXT);
	char *card_string;
	for (unsigned int i = 0; i < dealer.num_cards; i++) {
		card_string = card_as_string(dealer.hand[i]);
		Graphics_drawString(&g_sContext, card_string,
		AUTO_STRING_LENGTH, i * 20, 35, TRANSPARENT_TEXT);
		//Graphics_flushBuffer(&g_sContext);
		//free(card_string);
	}
	Graphics_flushBuffer(&g_sContext);
	free(card_string);
}
/**
 * Main beef of the program, make bet then draw cards
 */
void make_bet() {
	bet_prompt();
	int bet_made = 0;
	unsigned char currKey;
	int bet;
	char num[2];
	int current_cpu_bet;
	while (!bet_made) {
		currKey = getKey();
		if (currKey == '1' || currKey == '2' || currKey == '4'
				|| currKey == '8') {
			num[0] = currKey;
			bet = atoi(num);
			current_cpu_bet = cpu_bet(bet);
			display_bets(bet, current_cpu_bet);
			swDelay(2);
			if (bet < current_cpu_bet) {
				Graphics_clearDisplay(&g_sContext);

				Graphics_drawString(&g_sContext, "Please match or",
				AUTO_STRING_LENGTH, 0, 40,
				TRANSPARENT_TEXT);
				Graphics_drawString(&g_sContext, "raise dealer bet",
				AUTO_STRING_LENGTH, 0, 50,
				TRANSPARENT_TEXT);
				Graphics_flushBuffer(&g_sContext);
			} else {
				bet_made = 1;
				current_pot += bet;
			}

		} else if (currKey == '*') {
			restart_game();
			bet_made = 0;
		}
	}

	swDelay(3);

}
void prompt_deal() {
	Graphics_clearDisplay(&g_sContext);

	Graphics_drawString(&g_sContext, "Draw a card: 1",
	AUTO_STRING_LENGTH, 0, 16, TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, "Hold: #",
	AUTO_STRING_LENGTH, 0, 24, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
}
void play_round() {

	prompt_deal();
	int action_made = 0;
	char currKey;
	int player_turn_over = 0;
	int player_hold = 0;
	while (!action_made) {
		currKey = getKey();
		if (currKey == '1') {
			deal_card(0);
			if (player.score > 21) {
				player_lose();
				return;
			}
			player_turn_over = 1;
			display_game();
			swDelay(5);
			prompt_deal();

		}
		//Hold
		else if (currKey == '#') {
			player_hold = 1;

			//action_made = 1;
		} else if (currKey == '*') {
			restart_game();
			return;
		}
		//Dealer's turn
		//If the dealer can draw a card
		if (player_turn_over || player_hold) {
			swDelay(5);
			//Check if dealer can draw a card
			if (player.score <= 21 && dealer.score < 17) {
				deal_card(1);
			}
			//If dealer did not bust, but can't draw anymore cards, display and check who wins
			else {

				action_made = 1;
				swDelay(3);
				if (player.score >= dealer.score) {
					player_win();
				} else {
					display_dealer_hand();

					player_lose();
				}
			}
			//Check if dealer goes bust
			if (dealer.score > 21) {
				swDelay(3);
				player_win();
				return;
			}
			player_turn_over = 0;
		}

	}

}
void bet_prompt() {
	Graphics_clearDisplay(&g_sContext);

	Graphics_drawString(&g_sContext, "Please enter", AUTO_STRING_LENGTH, 0, 57,
	TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, "a bet!", AUTO_STRING_LENGTH, 0, 67,
	TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);

}
void display_bets(int player_bet, int dealer_bet) {
	Graphics_clearDisplay(&g_sContext);

	Graphics_drawString(&g_sContext, "Player bet: ",
	AUTO_STRING_LENGTH, 0, 8, TRANSPARENT_TEXT);

	char *p_bet = (char *) malloc(sizeof(char));

	sprintf(p_bet, "%d", player_bet);

	Graphics_drawStringCentered(&g_sContext, p_bet,
	AUTO_STRING_LENGTH, 70, 12, TRANSPARENT_TEXT);

	Graphics_flushBuffer(&g_sContext);

	Graphics_drawString(&g_sContext, "Dealer bet: ",
	AUTO_STRING_LENGTH, 0, 20, TRANSPARENT_TEXT);

	char *d_bet = (char *) malloc(sizeof(char));

	sprintf(d_bet, "%d", dealer_bet);

	Graphics_drawStringCentered(&g_sContext, d_bet,
	AUTO_STRING_LENGTH, 70, 22, TRANSPARENT_TEXT);

	Graphics_flushBuffer(&g_sContext);

	free(p_bet);
	free(d_bet);
}

void player_lose() {
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "You lost",
	AUTO_STRING_LENGTH, 30, 30, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
	BuzzerOn();
	swDelay(3);
	BuzzerOff();
	//swDelay(1);

	swDelay(3);
	Graphics_clearDisplay(&g_sContext);
	game_over = 0;
	bet_made = 0;

}
void player_win() {
	display_dealer_hand();
	swDelay(5);
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawStringCentered(&g_sContext, "You won!",
	AUTO_STRING_LENGTH, 50, 30, TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, "Balance increased",
	AUTO_STRING_LENGTH, 0, 50, TRANSPARENT_TEXT);
	char *balance = (char *) malloc(sizeof(char) * 2);

	sprintf(balance, "%d", current_pot);
	Graphics_drawStringCentered(&g_sContext, balance,
	AUTO_STRING_LENGTH, 50, 65, TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
	BuzzerOn();
	swDelay(3);
	BuzzerOff();
	//swDelay(1);

	swDelay(3);
	Graphics_clearDisplay(&g_sContext);
}
int cpu_bet(int player_bet) {

	if (dealer.score >= 13 && dealer.score <= 18) {
		current_pot += 4;
		return 4;
	} else if (dealer.score >= 19) {
		current_pot += 8;
		return 8;
	} else if (dealer.score >= 7 && dealer.score <= 12) {
		current_pot += 2;
		return 2;
	} else {
		current_pot += 1;
		return 1;
	}
}
char *card_as_string(card card) {
	char *face_string;

	enum Face face = card.face;
	switch (face) {
	case Diamond:
		face_string = "D";
		break;
	case Heart:
		face_string = "H";
		break;
	case Spade:
		face_string = "S";
		break;
	case Club:
		face_string = "C";
		break;
	default:
		break;
	}
	char *final_string = malloc(sizeof(char) * 4);
	snprintf(final_string, 4, "%s-%u", face_string, card.value);
	return final_string;
}
int get_card_value(int val) {
	if (val == 1) {
		return 11;
	} else if (val >= 11 && val <= 13) {
		return 10;
	} else {
		return val;
	}
}
void deal_card(int num) {

	for (unsigned int i = 0; i < 52; i++) {
		//Get the index of the next card to draw
		int next_index = deal_order[i];
		/**
		 * 0 indicates that that card has already been dealt
		 */
		if (!playing_deck[next_index].in_play) {
			playing_deck[next_index].in_play = 1;
			/**
			 * Value of !0 for in_hand indicates a spot to put a card in
			 */
			for (unsigned int j = 0; j < 14; j++) {
				if (num == 0) {
					if (!player.hand[j].in_hand) {
						playing_deck[next_index].in_hand = 1;
						player.hand[j].face = playing_deck[next_index].face;
						player.hand[j].in_hand =
								playing_deck[next_index].in_hand;
						player.hand[j].in_play =
								playing_deck[next_index].in_play;
						player.hand[j].suit = playing_deck[next_index].suit;
						player.hand[j].value = playing_deck[next_index].value;

						player.num_cards++;
						player.score += get_card_value(player.hand[j].value);
						j = 14;
						i = 52;
					}
				} else if (num) {
					if (!dealer.hand[j].in_hand) {
						playing_deck[next_index].in_hand = 1;
						dealer.hand[j].face = playing_deck[next_index].face;
						dealer.hand[j].in_hand =
								playing_deck[next_index].in_hand;
						dealer.hand[j].in_play =
								playing_deck[next_index].in_play;
						dealer.hand[j].suit = playing_deck[next_index].suit;
						dealer.hand[j].value = playing_deck[next_index].value;
						dealer.score += get_card_value(dealer.hand[j].value);

						dealer.num_cards++;
						j = 14;
						i = 52;
					}
				}
			}
		}
	}
}

void cut_deck(int num) {
	for (unsigned int i = 0; i < 52; i++) {
		deal_order[i] = -1;
	}
	//Seed rng
	srand(num);

	//Generate 52 random unique indexes
	int seen = 0;
	for (unsigned int i = 0; i < 52; i++) {

		int index = rand() % 52;

		for (unsigned int j = 0; j < 52; j++) {

			if (deal_order[j] == index) {

				j = 52;
				seen = 1;
			}

		}
		if (!seen) {
			deal_order[i] = index;
		} else {
			i--;
		}
		seen = 0;
	}
}

void init_hands() {
	player.balance = 0;
	player.score = 0;
	player.num_cards = 0;

	dealer.balance = 0;
	dealer.score = 0;
	dealer.num_cards = 0;
	for (unsigned int i = 0; i < 14; i++) {
		player.hand[i].in_hand = 0;
		player.hand[i].in_play = 0;
		dealer.hand[i].in_hand = 0;
		dealer.hand[i].in_play = 0;
	}
}

void init_deck() {

	/**
	 * 0-12 	Red Hearts
	 * 13-25 	Red Diamonds
	 * 26-38	Black Spade
	 * 39-51	Black Club
	 */

	for (unsigned int i = 0; i < 13; i++) {
		unsigned int card_val = i + 1;
		//Red Hearts

		playing_deck[i].suit = red;
		playing_deck[i].face = Heart;
		playing_deck[i].value = card_val;
		playing_deck[i].in_play = 0;

		//Red Diamonds
		playing_deck[i + 13].suit = red;
		playing_deck[i + 13].face = Diamond;
		playing_deck[i + 13].value = card_val;
		playing_deck[i + 13].in_play = 0;
		//Black Spade
		playing_deck[i + 26].suit = black;
		playing_deck[i + 26].face = Spade;
		playing_deck[i + 26].value = card_val;
		playing_deck[i + 26].in_play = 0;
		//Black Club
		playing_deck[i + 39].suit = black;
		playing_deck[i + 39].face = Club;
		playing_deck[i + 39].value = card_val;
		playing_deck[i + 39].in_play = 0;

	}

}
void swDelay(char numLoops) {
	// This function is a software delay. It performs
	// useless loops to waste a bit of time
	//
	// Input: numLoops = number of delay loops to execute
	// Output: none
	//
	// smj, ECE2049, 25 Aug 2013

	volatile unsigned int i, j;	// volatile to prevent removal in optimization
								// by compiler. Functionally this is useless code

	for (j = 0; j < numLoops; j++) {
		i = 50000;					// SW Delay
		while (i > 0)				// could also have used while (i)
			i--;
	}
}
