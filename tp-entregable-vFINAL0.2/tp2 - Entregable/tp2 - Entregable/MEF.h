/*
 * MEF.h
 *
 * Created: 12/05/2023 17:26:39
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 


#ifndef MEF_H_

#define MEF_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "lcd.h"
#include "KeyPad4x4.h"
#include "scheduler.h"
#include "helpers.h"



/**
	Esta funcion se encarga de iniciar la maquina de estados, establece el estado CERRADO, inicia el 
	state_call_counter que se encarga de contar cuantas veces fue ejecutada la funcion y limpia el
	LCD.	*/
void MEF_Init(void);


/**
    Esta funcion modifica el tiempo en el modulo TIMER0.
    @param {uint8_t} selec Selecciona el campo de la hora que sera modificado, 1: hs, 2: min, 3: seg.*/
void MEF_ModificarHora(uint8_t selec);

/**
    Esta funcion actualiza el estado del sistema, no recibe parametros ni retorna nada. */
void MEF_Update(void);

/**
    Esta funcion cambia al estado cerrado, resetea el state_call_count y cambia el formato de la hora a normal. */
void MEF_GuardarHora ();



typedef enum
{
	E_ABIERTO,
	E_CERRADO,
	E_DENEGADO,
	E_HORAS,
	E_MINUTOS,
	E_SEGUNDOS
} eSystem_state;

#endif /* MEF_H_ */