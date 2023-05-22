/*
 * KeyPad4x4.h
 *
 * Created: 12/05/2023 19:03:52
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 


#ifndef KEYPAD4X4_H_
#define KEYPAD4X4_H_

#include <avr/io.h>
#include "lcd.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void KEYPAD_Init();
uint8_t KEYPAD_Scan(uint8_t *key);


#endif /* KEYPAD4X4_H_ */