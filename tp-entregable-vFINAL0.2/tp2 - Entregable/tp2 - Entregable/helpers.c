/*
 * helpers.c
 *
 * Created: 20/05/2023 12:58:44
 *	Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 

#include "helpers.h"

uint8_t correctPsw[4] = {5, 9, 1, 3};

uint8_t HELPER_CheckPassword(const char* psw)
{
	uint8_t checker = 0;
	for (int i = 0; i < 4; i++) {
		if (psw[i] == correctPsw[i]) {
			checker++;
		}
	}
	if (checker == 4) {
		return 1;
		} else {
		return 0;
	}
}

void HELPER_ClearSecondLine() {
	LCDGotoXY(0,1);
	LCDstring("                ", 16);
}