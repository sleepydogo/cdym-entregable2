/*
 * timer0.c
 *
 * Created: 14/05/2023 12:58:44
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */ 
#include "scheduler.h" 

uint8_t contador_MEF = 0,contador_Segundos = 0;
uint8_t segundos = 0,minutos = 0,horas = 0, formatoHora = 0; 
uint8_t cursor_flag = 0, MEF_flag = 0, imprimirHora_flag = 0, incrementarSegundo_flag = 0;
int8_t nuevaHora;
/**
*	
*	TIMER0_Init: Inicia el timer configurando los registros correspondientes, lee el tiempo de compilacion y crea las variables de conteo temporal.*/
void SCHEDULER_Init(void){
	TCCR0B |= (1 << CS02) | (1 << CS00); // Selecciona un prescaler de 1024 para el temporizador Timer0
	TCCR0A |= (1 << WGM01); // Configura el temporizador Timer0 en modo de comparación (CTC)
	OCR0A = 155;	// Carga el valor adecuado en el registro de comparación (OCR0A) para generar la interrupción cada 10ms
	TIMSK0 |= (1 << OCIE0A); 	// Habilita las interrupciones del temporizador Timer0
	sei();
	sscanf(__TIME__, "%hhu:%hhu:%hhu", &horas, &minutos, &segundos); // Lee la hora de compilación y la guarda en las variables correspondientes
	formatoHora = 0;
}

/**
*
*	TIMER0_Tasks: Se llama esta funcion en un loop, comprueba si debe actualizar la MEF, si debe imprimir la hora, si debe incrementar las variables de tiempo (hora,minutos,segundos).*/

void SCHEDULER_Tasks(void){
	if (MEF_flag) {	// Si esta activo el flag de actualizacion de estado llama a la funcion y resetea el flag.
		MEF_flag = 0;	
		MEF_Update(); 
	}
	if (imprimirHora_flag) { // Si esta activado el flag de imprimir hora llama a la funcion y resetea el flag.
		imprimirHora_flag = 0;
		SCHEDULER_ImprimirHora();
	}
	if (incrementarSegundo_flag) {
		incrementarSegundo_flag = 0;
		SCHEDULER_IncrementarSec();
	}
}

/**
*
*	TIMER0_IncrementarSec: Se llama cada 98 ticks (1 segundo) y actualiza las variables de tiempo.	*/
void SCHEDULER_IncrementarSec(void){ 
	segundos++;
    if (segundos > 59)
    {
        segundos = 0;
        minutos++;
    }
    if (minutos > 59)
    {
        minutos = 0;
        horas++;
    }
    if (horas > 23)
    {
        horas = 0;
    }
}

/**
*	Cambia el formato impresion de la hora.
*	@param {uint8_t} selec */

void SCHEDULER_CambiarFormatoHora(uint8_t selec){
	formatoHora = selec;
}

/**
*	Edita una variable temporal.
*	@param {uint8_t} num: Contiene el nuevo digito a almacenar en la hora. 
*	@param {uint8_t} desplazador: Contiene un valor 0 o 1 que indica que digito de la hora se esta modificando. 
*	@param {uint8_t} selec: Contiene un valor 1,2,3 que indica que variable de tiempo se esta modificando siendo horas, minutos, segundos respectivamente. */

void SCHEDULER_EditarHora(uint8_t num, uint8_t desplazador) { 
	if (desplazador == 0) {
		nuevaHora = num;
	} else {
		nuevaHora = nuevaHora*10 + num;
	}
}

/**
*	Comprueba si la modificacion realizada en la hora es valida y la almacena.
*	@param {uint8_t} selec: Contiene un valor 1,2,3 que indica que variable de tiempo se esta modificando siendo horas, minutos, segundos respectivamente. */

void SCHEDULER_GuardarHora(uint8_t selec) {
	switch (selec) {
		case 1: 
		if (nuevaHora <= 23) {
			horas = nuevaHora;
		}
		break;
		case 2:
		if (nuevaHora <= 59) {
			minutos = nuevaHora;
		}
		break;
		case 3:
		if (nuevaHora <= 59) {
			segundos = nuevaHora;
		}
		break;
	}
}


/**
*	Cambia el formato de hora a imprimir siendo: 
*
*		0 Modo normal			--> 00:00:00			
*		1 Modo edicion horas	-->  _:00:00 ,   :00:00, _0:00:00,  0:00:00
*		2 Modo edicion minutos	--> 00: _:00 , 00:  :00, 00:_0:00,  0: 0:00
*		3 Modo edicion segundos	--> 00:00: _ , 00:00:  , 00:00:_0,  0:00: 0		*/

void SCHEDULER_ImprimirHora() {
	LCDGotoXY(4,0);
	char horario[8];
	switch (formatoHora) {	// Dependiendo del formato de hora elegido, imprime en modo normal o edicion.
		case 0: // Modo normal
			sprintf(horario, "%02d:%02d:%02d", horas, minutos, segundos);
			nuevaHora = -1;
		break;
		case 1: // Modo edicion horas
			if ((nuevaHora == -1) & (cursor_flag)) {
				sprintf(horario, " _:%02d:%02d", minutos, segundos);	
			} else if ((nuevaHora == -1) & !(cursor_flag)) {
				sprintf(horario, "  :%02d:%02d", minutos, segundos);
			} else if ((nuevaHora < 9) & (cursor_flag)) {
				sprintf(horario, "_%d:%02d:%02d", nuevaHora, minutos, segundos);
			} else if ((nuevaHora < 9) & !(cursor_flag)) {
				sprintf(horario, " %d:%02d:%02d", nuevaHora, minutos, segundos);
			} else {
				sprintf(horario, "%02d:%02d:%02d", nuevaHora, minutos, segundos);
			}
		break;	
		case 2: // Modo edicion minutos
			if ((nuevaHora == -1) & (cursor_flag)) {
				sprintf(horario, "%02d: _:%02d", horas, segundos);	
			} else if ((nuevaHora == -1) & !(cursor_flag)) {
				sprintf(horario, "%02d:  :%02d", horas, segundos);
			} else if ((nuevaHora < 9) & (cursor_flag)) {
				sprintf(horario, "%02d:_%d:%02d", horas, nuevaHora, segundos);
			} else if ((nuevaHora < 9) & !(cursor_flag)) {
				sprintf(horario, "%02d: %d:%02d", horas, nuevaHora, segundos);
			} else {
				sprintf(horario, "%02d:%02d:%02d", horas, nuevaHora, segundos);
			} 
		break;		
		case 3: // Modo edicion minutos
			if ((nuevaHora == -1) & (cursor_flag)) {
				sprintf(horario, "%02d:%02d: _", horas, minutos);	
			} else if ((nuevaHora == -1) & !(cursor_flag)) {
				sprintf(horario, "%02d:%02d:  ", horas, minutos);
			} else if ((nuevaHora < 9) & (cursor_flag)) {
				sprintf(horario, "%02d:%02d:_%d", horas, minutos, nuevaHora);
			} else if ((nuevaHora < 9) & !(cursor_flag)) {
				sprintf(horario, "%02d:%02d: %d", horas, minutos, nuevaHora);
			} else {
				sprintf(horario, "%02d:%02d:%02d", horas, minutos, nuevaHora);
			}
		break;	
	}
	LCDstring(horario, 8);
}

  
ISR(TIMER0_COMPA_vect) // cada 10 ms
{
	// Actualizar MEF cada 100 ms
	if (++contador_MEF==10) {
		MEF_flag=1;
		contador_MEF=0;
	}
	
	// Se ejecuta cada 1 segundo
	if (++contador_Segundos == 100){ 
		cursor_flag = !cursor_flag;
		imprimirHora_flag = 1;
		incrementarSegundo_flag = 1;
		contador_Segundos = 0;
	}
}