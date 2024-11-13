#include "stm32f103xb.h"


#define SYSTICK_LOAD_VAL   8000
#define CTRL_ENABLE		   (1U<<0)
#define CTRL_CLKSRC		   (1U<<2)
#define CTRL_COUNTFLAG	   (1U<<16)


void systickDelayMs(int delay){
	/*Configurar SysTick*/

	/*Recargar con el numero de ciclos por milisegundos*/
	SysTick->LOAD = SYSTICK_LOAD_VAL;

	/*Borrar el valor de registro SysTick*/
	SysTick->VAL = 0;

	/*Habilitar systick y seleccionar clk interno*/
	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;

	for (int i = 0; i < delay; i++) {
		/*Espearar hasta que la COUNTFLAG se habilite */

		while((SysTick->CTRL & CTRL_COUNTFLAG) == 0){}

	}
	SysTick->CTRL = 0;

}
