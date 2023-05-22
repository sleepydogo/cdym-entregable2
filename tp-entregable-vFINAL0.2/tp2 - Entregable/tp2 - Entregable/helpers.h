/*
 * MEF.c
 *
 * Created: 12/05/2023 16:55:03
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string.h>
#include <avr/io.h>
#include "lcd.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void HELPER_ClearSecondLine(void);
uint8_t HELPER_CheckPassword(const char* psw);

#endif /* TIMER0_H_ */