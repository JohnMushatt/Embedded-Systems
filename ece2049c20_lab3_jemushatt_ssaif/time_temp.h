/*
 * time_temp.h
 *
 *  Created on: Feb 19, 2020
 *      Author: johnm
 */

#ifndef TIME_TEMP_H_
#define TIME_TEMP_H_
#define SECONDS 60
#define MINUTES 60
#define HOURS 24
#define DAYS 28
#include "peripherals.h"
#include <msp430.h>
#include <stdlib.h>
#include <string.h>
long unsigned int global_time;

void displayTime(long unsigned int inTime);

void displayTemp(float inAverageTempC);
#endif /* TIME_TEMP_H_ */
