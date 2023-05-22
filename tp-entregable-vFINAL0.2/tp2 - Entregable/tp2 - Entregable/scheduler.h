/*
 * timer0.h
 *
 * Created: 14/05/2023 12:59:42
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include <string.h>
#include "MEF.h"

void SCHEDULER_Init(void);
void SCHEDULER_Tasks(void);
void SCHEDULER_IncrementarSec(void);
void SCHEDULER_ImprimirHora();
void SCHEDULER_CambiarFormatoHora(uint8_t selec);
void SCHEDULER_EditarHora(uint8_t num, uint8_t desplazador);
void SCHEDULER_GuardarHora(uint8_t selec);

#endif /* SCHEDULER_H_ */