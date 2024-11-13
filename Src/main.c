#include <stdio.h>
#include <stdint.h>

#include "stm32f103xb.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"

#define GPIOCEN 			(1U<<4)  //0000 0000 0000 0000 0000 0000 0001 0000 Máscara para habilitar el reloj de GPIOC (Bit 4 en el registro RCC)

#define PIN_13				(1U<<13) // Máscara para el pin 13 de GPIOC (Bit 13)
#define LED_PIN				PIN_13


int main(void){

	//Habilitar el reloj para GPIOC
	RCC->APB2ENR |= GPIOCEN;

	// Configurar el pin 13 de GPIOC como salida (modo general de salida)
	//GPIOC->CRH   |= (1U<<21);
	GPIOC->CRH &= ~(0xF << 20);  // Limpiar los bits 20-23
    GPIOC->CRH |= (0x2 << 20);

	while(1){

			GPIOC->ODR ^= LED_PIN;
			systickDelayMs(1000);

	}
}

