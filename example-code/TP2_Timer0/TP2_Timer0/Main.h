#ifndef _MAIN_H
#define _MAIN_H

	// Archivo de cabecera del Microcontrolador
	#include <avr/io.h>

	// Interrupciones
	#include <avr/interrupt.h>
	
	// Tipos de datos definidos por el usuario
	//typedef unsigned char uint8_t; // Esta incluido en stdint.h
	
	// Oscillator / resonator frequency (in Hz)
	#define F_CPU 16000000UL		// 16 MHz
	
	// Delays perdiendo tiempo del uC
	#include <util/delay.h>

	// Tipos de datos enteros estandar
	#include <stdint.h>	

	// Manejo de caracteres y mas
	#include <stdlib.h>	

#endif

/*------------------------------------------------------------*-
---- END OF FILE --------------------------------------------
-*------------------------------------------------------------*/
