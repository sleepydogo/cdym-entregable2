/*
 * MEF.c
 *
 * Created: 12/05/2023 16:55:03
 *  Author: Tomas E. Schattmann y Mariano A. Rodriguez Mesa
 */

#include "MEF.h"

static eSystem_state System_state;
uint8_t state_call_count = 0, cantTeclasIngresadas = 0, teclaPresionada = 0, psw[4] = {0,0,0,0}, letrasKeypadAscii[3] = {65,66,67}; 
char pswLcd[4][4] = {"*   ", "**  ", "*** ", "****"};
uint8_t posDigitoAEditar = 0;

void MEF_Init()
{
    System_state = E_CERRADO;
    state_call_count = -1;
    LCDclr();
}

void MEF_GuardarHora () {
	System_state = E_CERRADO;
	state_call_count = -1;
	SCHEDULER_CambiarFormatoHora(0);
}

void MEF_ModificarHora(uint8_t selec)
{
	// Si es la primera vez que se invoca la funcion, selecciona el formato de hora que se imprimira en el LCD.
	if (state_call_count == 0)
	{
		cantTeclasIngresadas = 0; // reinicia el contador de teclas presionadas
		HELPER_ClearSecondLine();
		LCDGotoXY(4,1);
		LCDstring("CERRADO", 7);	
		SCHEDULER_CambiarFormatoHora(selec);

	}

	// Si una tecla es presionada, comprueba si la tecla es un numero, una letra o un simbolo.
	if (KEYPAD_Scan(&teclaPresionada))
	{
		// Ingreso un numero, se intenta editar la hora
		if ((teclaPresionada >= 0) && (teclaPresionada <= 9))
		{
			SCHEDULER_EditarHora(teclaPresionada, posDigitoAEditar);
			posDigitoAEditar++;
		}
		// Ingreso la tecla de guardado
		else if (teclaPresionada == letrasKeypadAscii[selec - 1])
		{
			SCHEDULER_GuardarHora(selec);
			MEF_GuardarHora();
		}
		// Ingreso #
		else if (teclaPresionada == 35)
		{
			MEF_GuardarHora();
		}
	}

	// Si la posicion del digito a modificar es igual o mayor a 2, la resetea a 0
	if (posDigitoAEditar >= 2)
	{
		posDigitoAEditar = 0;
	}

	// Si se llamo la funcion 250 veces (25 segundos para timeout), se intenta guardar la hora y cambia a estado CERRADO.
	if (state_call_count == 250)
	{
		MEF_GuardarHora();
	}
}


void MEF_Update()
{	
	state_call_count++; //  Primero se incrementa state_call_counter. Esta variable lleva cuenta de cuantas veces fue llamada la funcion.
    switch (System_state)
    {
	case E_ABIERTO:
        if (state_call_count == 0) { // Si es la primera vez que se llama la funcion, limpia la segunda linea y escribe en ella ABIERTO.  
			cantTeclasIngresadas = 0;			
			HELPER_ClearSecondLine();
			LCDGotoXY(4,1);
			LCDstring("ABIERTO", 7);
        } else if (state_call_count == 30) { // Cuenta 3 segundos, limpia la segunda linea y vuelve al estado por defecto.
            HELPER_ClearSecondLine();
            System_state = E_CERRADO;
            state_call_count = -1;
        }
        break;
		
    case E_CERRADO:
		if ((state_call_count == 0)) {	// Si es la primera vez que se llama a la funcion imprime CERRADO en el LCD.
	        HELPER_ClearSecondLine();
	        LCDGotoXY(4,1);
	        LCDstring("CERRADO", 7);
        }
		// Al ser una variable de 8 bits una vez que llega al valor maximo lo reinicia.
        if (state_call_count == 255) { 
            state_call_count = 1;
        } 
        // Checkea si se apretaron 4 teclas, comprueba si es la contraseña correcta y cambia al estado abierto o cerrado respectivamente.
        if (cantTeclasIngresadas == 4)
        {
            if (HELPER_CheckPassword(psw) == 1)
            {
                System_state = E_ABIERTO;
                state_call_count = -1;
            }
            else
            {
                System_state = E_DENEGADO;
                state_call_count = -1;
            }
        }
		// Lee el teclado, si hay una tecla presionada realiza la respectiva funcion de cada una.
        if (KEYPAD_Scan(&teclaPresionada))
        {
            switch (teclaPresionada)
            {
            case 65: // Funcion: Modificar horas
                System_state = E_HORAS;
                state_call_count = -1;
                break;
            case 66: // Funcion: Modificar minutos
                System_state = E_MINUTOS;
                state_call_count = -1;
                break;
            case 67: // Funcion: Modificar segundos
                System_state = E_SEGUNDOS;
                state_call_count = -1;
            break;
			case 35: // '#' Estas teclas no realizan nada al momento de ingresar la clave, por lo que se contemplan en estos case vacios..
			case 42: // '*'
            case 68: // 'D'
                // no hace nada...
                break;
            /*
			*	Si es un numero, limplia la segunda linea del LCD, e imprime la cantidad de asteriscos necesarios 
			*	para representar la contraseña y aumenta la cantidad de teclas ingresadas.
			*/
			default: 
				HELPER_ClearSecondLine();
                LCDGotoXY(6,1);
				LCDstring(pswLcd[cantTeclasIngresadas], 4);
				psw[cantTeclasIngresadas] = teclaPresionada;
				cantTeclasIngresadas++;
                break;
            }
        }
        break;
	
    case E_DENEGADO:
        if (state_call_count == 0) { // Si es la primera vez que se invoca la funcion
			cantTeclasIngresadas = 0; // reinicia el contador de teclas presionadas.
		    HELPER_ClearSecondLine(); // Limpia la segunda linea
			LCDGotoXY(4,1); 
			LCDstring("DENEGADO", 8); // Imprime DENEGADO en el LCD.
        } else if (state_call_count == 20) // cuento hasta 2 segundo y salgo del estado.
        {
            HELPER_ClearSecondLine();
            System_state = E_CERRADO;
            state_call_count = -1;
        }
        break;
    case E_HORAS:
		MEF_ModificarHora(1);
        break;
    case E_MINUTOS:
		MEF_ModificarHora(2);
        break;
    case E_SEGUNDOS:
		MEF_ModificarHora(3);
		break;
    }
}