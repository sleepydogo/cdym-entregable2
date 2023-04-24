/*
 * TP2_Timer0.c
 *
 * Created: 15/04/2022 11:58:35
 * Author : J.Juarez
 */ 

#include "main.h"

//CONFIGURACION DEL TIMER0
//Activaci�n de interrupci�n peri�dica y
//Generaci�n de se�al peri�dica en terminal OC0A (PD6) 
void Timer0Init(void){
	DDRD |= (1<<PORTD6); //waveform generator output
	//configuraci�n del TOPE del contador TCNT0 
	// Registro de comparacion
	OCR0A=39;			//39+1
	// Registros de configuracion
	TCCR0A =(1<<COM0A0) | (1<<WGM01); //modo CTC, Toggle on compare match
	TCCR0B =(1<<CS01);	//CTC CLK/8 =16MHz/8 =2MHz
	// Configuracion de interrupciones
	TIMSK0 =(1<<OCIE0A); // habilitamos interrpuci�n COMPA
}

//MANEJADOR DE INTERRUPCION DEL COMPARADOR A DEL TIMER0
//se activa periodicamente cuando TCNT0==OCR0A modo CTC
//ejecuta una tarea dummy para test

ISR(TIMER0_COMPA_vect){ //interrupci�n peri�dica de periodo Tisr=40/2MHz=20us  o fisr=2MHz/40=500kHz

		PORTB ^=(1<<5); //toggle cada 100ms
}


//MANEJADOR DE INTERRUPCI�N INT0
//ejecuta una tarea dummy para test
ISR(INT0_vect){
	//tarea dummy
	PORTC ^=(1<<PORTC3);
	//debouncing
	_delay_ms(10);
}

//INT0
//Configuraci�n y activaci�n
//de la interrupci�n externa INT0
void Int0Init(){
	DDRD &=~(1<<PORTD2);
	PORTD |=(1<<PORTD2); //Pullup PD2
	//  EICRA: External Interrupt Control Register, Referencia diap 27 clase 7
	EICRA |=(1<<ISC01); //INT0 x flanco de bajada
	// EIMSK: External Interrupt Mask Register
	EIMSK |=(1<<INT0);  //Habilitar int INT0	
}

//PROGRAMA MAIN
//Inicializaciones y activaci�n de interrupciones
//pone en ejecuci�n una tarea dummy en el loop.
int main(void)
{
	DDRC |=(1<<DDC3);
	DDRD |=(1<<DDD1);
	DDRB |=(1<<DDB5);	
	Int0Init();
	Timer0Init();
	sei();

    while (1) 
    {
		//tarea dummy
		PORTD |=(1<<PORTD1);
		_delay_ms(50);
		PORTD &=~(1<<PORTD1);
		_delay_ms(50);
    }
}

