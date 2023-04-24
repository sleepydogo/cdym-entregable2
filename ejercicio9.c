// Muestra digitos ingresados al teclado matricial 4x4 en una pantalla LCD 16x2

// Muestra digitos ingresados al teclado matricial 4x4 en una pantalla LCD 16x2

#include <avr/io.h>
#include "lcd.h"
#include <stdio.h>
#include <util/delay.h>

static const uint8_t keypadCaracteres[4][4] =
{
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'0', '*', '#', 'D'}
};

static const uint8_t filas[4] = {PINB4,PINB3,PINB0,PIND7};
static const uint8_t columnas[4] = {PIND3,PIND5,PIND4,PIND2};

void KEYPAD_Init()
{
	// Se utilizarán las filas como salida (PULL-UP)
	DDRB &= ~((1<<PORTB0) | (1<<PORTB3) | (1<<PORTB4));
	DDRD &= ~(1<<PORTD7);
	
	PORTB |= (1<<PORTB0) | (1<<PORTB3) | (1<<PORTB4);
	PORTD |= (1<<PORTD7);
	
	// Se utilizarán las columnas como salidas
	DDRD |= (1 << PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5);
	PORTD &= ~((1<< PORTD2) | (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD5));
}

uint8_t KEYPAD_Scan(uint8_t *key)
{
	uint8_t reposo;
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
			
			if (!reposo){
				*key = keypadCaracteres[f][c];
				presiono = 1;
			}
		}
		
		PORTD |= (1<<columnas[c]);
		DDRD &= ~(1<<columnas[c]);
	}
	
	return presiono;
}
int main(void){
	
	/* Setup */
	LCDinit();
	
	KEYPAD_Init();
	_delay_ms(1);
	
	LCDclr();
	_delay_ms(1);
	
	LCDGotoXY(0,0);
	LCDstring("      CDYM      ", 16);
	
	while (1){
		_delay_ms(50);
		uint8_t tecla;
		uint8_t presiono = KEYPAD_Scan(&tecla);
		while (presiono){
			LCDGotoXY(8,1);
			LCDsendChar(tecla);
			_delay_ms(10);
			tecla = "";
			uint8_t presiono = KEYPAD_Scan(&tecla);
		}
		
	}
	
}
