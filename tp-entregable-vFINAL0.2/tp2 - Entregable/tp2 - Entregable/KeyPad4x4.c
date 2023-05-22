/*
 * KeyPad4x4.c
 *
 * Created: 12/05/2023 19:03:37
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 


#include "KeyPad4x4.h"

// Mapeo de las teclas del keypad (65,66,67,68 = A,B,C,D ; 42,35 = *,#)
static const uint8_t keypadCaracteres[4][4] =
{
	{1, 2, 3, 65},
	{4, 5, 6, 66},
	{7, 8, 9, 67},
	{42, 0, 35, 68}
};

static const uint8_t filas[4] = {PINB4,PINB3,PINB0,PIND7};
static const uint8_t columnas[4] = {PIND3,PIND5,PIND4,PIND2};

void KEYPAD_Init()
{
	// Se utilizaron las filas como entrada (PULL-UP)
	DDRB &= ~((1<<PORTB0) | (1<<PORTB3) | (1<<PORTB4));
	DDRD &= ~(1<<PORTD7);
	
	PORTB |= (1<<PORTB0) | (1<<PORTB3) | (1<<PORTB4);
	PORTD |= (1<<PORTD7);
	
	// Se utilizaron las columnas como salidas
	DDRD |= (1 << PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
	PORTD &= ~((1<< PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));
}

uint8_t KEYPAD_Scan(uint8_t *key)
{
	uint8_t reposo;
	static uint8_t ultimaTecla = 0xFF;  // Variable para almacenar la última tecla presionada.
	uint8_t presiono = 0;
	
	for (uint8_t c=0; c < 4; c++)
	{
		DDRD |= (1<<columnas[c]);
		PORTD &= ~(1<<columnas[c]);
		
		for (uint8_t f=0; f < 4; f++){
			if (f < 3){
				reposo = PINB & (1<<filas[f]);
				}else{
				reposo = PIND & (1<<filas[f]);
			}
			
			if (!reposo && ultimaTecla != keypadCaracteres[f][c]){  // Detectar el flanco descendente de la tecla presionada.
				ultimaTecla = keypadCaracteres[f][c];  // Almacenar la última tecla presionada.
				}else if (reposo && ultimaTecla == keypadCaracteres[f][c]){  // Detectar el flanco ascendente de la tecla liberada.
				presiono = 1;
				*key = ultimaTecla;  // Establecer el valor de la tecla solo cuando se libera.
				ultimaTecla = 0xFF;  // Restablecer la última tecla presionada.
			}
		}
		
		PORTD |= (1<<columnas[c]);
		DDRD &= ~(1<<columnas[c]);
	}
	
	return presiono;
}
