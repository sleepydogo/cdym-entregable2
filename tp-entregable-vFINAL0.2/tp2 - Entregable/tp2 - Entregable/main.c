/*
 * main.c
 *
 * Created: 11/5/2023 19:21:23
 *	Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa	 
 * Conexion display : Marron(D7) Negro Amarillo Blanco Verde Naranja Amarillo(RS)
 */ 

#include "main.h"

int main() {
  	LCDinit();  	// inicializa el lcd
    KEYPAD_Init(); 	// inicializa el keypad
    SCHEDULER_Init(); 	// inicializa el timer
    MEF_Init(); 	// inicializa la mef
    while (1){
        SCHEDULER_Tasks();
    }
}