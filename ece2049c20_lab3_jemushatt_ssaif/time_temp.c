/*
 * time_temp.c
 *
 *  Created on: Feb 19, 2020
 *      Author: johnm
 */

#include "time_temp.h"

void displayTime(long unsigned int inTime) {

	long unsigned int months = inTime / SECONDS / MINUTES / HOURS / DAYS;
	long unsigned int month_copy = months;
	char months_array[5];
	int i;

	if (months < 10) {
		months_array[0] = months + '0';
		months_array[1] = 0;
	} else {
		for (i = 0; i < 4; i++) {
			months_array[i] = ' ';
		}
		int digit_count = 0;
		while (digit_count < 4 && months != 0) {
			months_array[digit_count] = months % 10 + '0';
			months /= 10;
			digit_count++;
		}
		int i;
		for (i = 0; i < 4 / 2; ++i) {
			char c = months_array[i];
			months_array[i] = months_array[4 - i - 1];
			months_array[4 - i - 1] = c;
		}

		months_array[4] = 0;

	}
	long unsigned int months_in_seconds = month_copy * 28 * 24 * 60 * 60;

	long unsigned int time_minus_months = inTime - months_in_seconds;

	long unsigned int days = time_minus_months / SECONDS / MINUTES / HOURS;
	long unsigned int days_copy = days;
	char days_array[3];

	if (days < 10) {
		days_array[0] = days + '0';
		days_array[1] = 0;
	} else {
		for (i = 0; i < 3; i++) {
			days_array[i] = ' ';
		}
		int digit_count = 0;
		while (digit_count < 3 && days != 0) {
			days_array[digit_count] = days % 10 + '0';
			days /= 10;
			digit_count++;
		}
		int i;
		for (i = 0; i < 2 / 2; ++i) {
			char c = days_array[i];
			days_array[i] = days_array[2 - i - 1];
			days_array[2 - i - 1] = c;
		}

		days_array[2] = 0;

	}

	long unsigned int days_in_seconds = days_copy * 24 * 60 * 60;

	long unsigned int time_minus_days = time_minus_months - days_in_seconds;

	long unsigned int hours = time_minus_days / SECONDS / MINUTES / HOURS;
	long unsigned int hours_copy = hours;
	char hours_array[3];

	if (hours < 10) {
		hours_array[0] = hours + '0';
		hours_array[1] = 0;
	} else {
		for (i = 0; i < 3; i++) {
			hours_array[i] = ' ';
		}
		int digit_count = 0;
		while (digit_count < 3 && hours != 0) {
			hours_array[digit_count] = hours % 10 + '0';
			hours /= 10;
			digit_count++;
		}
		int i;
		for (i = 0; i < 2 / 2; ++i) {
			char c = hours_array[i];
			hours_array[i] = hours_array[2 - i - 1];
			hours_array[2 - i - 1] = c;
		}

		hours_array[2] = 0;

	}
	long unsigned int hours_in_seconds = hours_copy * 60 * 60;

	long unsigned int time_minus_hours = time_minus_days - hours_in_seconds;

	long unsigned int minutes = time_minus_hours / SECONDS;
	long unsigned int minutes_copy = minutes;
	char minutes_array[3];

	if (minutes < 10) {
		minutes_array[0] = minutes + '0';
		minutes_array[1] = 0;
	} else {
		for (i = 0; i < 3; i++) {
			minutes_array[i] = ' ';
		}
		int digit_count = 0;
		while (digit_count < 3 && minutes != 0) {
			minutes_array[digit_count] = minutes % 10 + '0';
			minutes /= 10;
			digit_count++;
		}
		int i;
		for (i = 0; i < 2 / 2; ++i) {
			char c = minutes_array[i];
			minutes_array[i] = minutes_array[2 - i - 1];
			minutes_array[2 - i - 1] = c;
		}

		minutes_array[2] = 0;

	}

	long unsigned int minutes_in_seconds = minutes_copy * 60;

	long unsigned int time_minus_minutes = time_minus_hours
			- minutes_in_seconds;

	long unsigned int seconds = time_minus_minutes;
	long unsigned int seconds_copy = seconds;
	char seconds_array[3];

	if (seconds < 10) {
		seconds_array[0] = seconds + '0';
		seconds_array[1] = 0;
	} else {
		for (i = 0; i < 3; i++) {
			seconds_array[i] = ' ';
		}
		int digit_count = 0;
		while (digit_count < 3 && seconds != 0) {
			seconds_array[digit_count] = seconds % 10 + '0';
			seconds /= 10;
			digit_count++;
		}
		int i;
		for (i = 0; i < 2 / 2; ++i) {
			char c = seconds_array[i];
			seconds_array[i] = seconds_array[2 - i - 1];
			seconds_array[2 - i - 1] = c;
		}

		seconds_array[2] = 0;

	}
	Graphics_clearDisplay(&g_sContext);
	Graphics_drawString(&g_sContext, months_array, AUTO_STRING_LENGTH, 10, 10,
	TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, days_array, AUTO_STRING_LENGTH, 20, 10,
	TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, hours_array, AUTO_STRING_LENGTH, 30, 10,
	TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, minutes_array, AUTO_STRING_LENGTH, 40, 10,
	TRANSPARENT_TEXT);
	Graphics_drawString(&g_sContext, seconds_array, AUTO_STRING_LENGTH, 55, 10,
	TRANSPARENT_TEXT);

	Graphics_flushBuffer(&g_sContext);

}
void displayTemp(float inAverageTempC) {

	char final_string[100];
	int decimal_value;
	int dec;
	inAverageTempC += .005;

	decimal_value = inAverageTempC;
	dec = (int) (inAverageTempC * 100) % 100;
	memset(final_string, 0, 100);

	int i = 2;
	while (decimal_value > 0) {
		final_string[i] = (decimal_value % 10) + '0';
		decimal_value /= 10;
		i--;
	}
	i = 3;
	final_string[i] = '.';
	final_string[i + 1] = (dec % 10) + '0';
	final_string[i + 2] = (dec / 10) + '0';
	final_string[i + 3] = 'C';
	Graphics_drawString(&g_sContext, final_string, AUTO_STRING_LENGTH, 10, 30,
	TRANSPARENT_TEXT);

	float inAverageTempF = ((9.0 / 5) * inAverageTempC) + 32;

	decimal_value = inAverageTempF;
	dec = (int) (inAverageTempF * 100) % 100;
	memset(final_string, 0, 100);

	i = 2;
	while (decimal_value > 0) {
		final_string[i] = (decimal_value % 10) + '0';
		decimal_value /= 10;
		i--;
	}
	i = 3;
	final_string[i] = '.';
	final_string[i + 1] = (dec % 10) + '0';
	final_string[i + 2] = (dec / 10) + '0';
	final_string[i + 3] = 'F';
	Graphics_drawString(&g_sContext, final_string, AUTO_STRING_LENGTH, 10, 45,
	TRANSPARENT_TEXT);
	Graphics_flushBuffer(&g_sContext);
}
