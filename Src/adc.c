#include "stm32f103xb.h"
#include "adc.h"

#define GPIOAEN			(1U<<2)
#define ADC1EN 			(1U<<9)
#define ADC_CH1			(1U<<0)
#define ADC_SE1_LEN_1 	 0x00
#define CR2_ADON		(1U<<0)
#define CR2_SWSTART 	(1U<<22)
#define SR_EOC			(1U<<1)
#define CR2_CONT 		(1U<<1)


void pa1_adc_init(void){
    /* Configurar el ADC GPIO PIN */

    /* Habilitar el reloj para GPIOA */
    RCC->APB2ENR |= GPIOAEN;

    /* Habilitar el modo analógico para el PA1 */
    GPIOA->CRL &=~ (1U<<4);
    GPIOA->CRL &=~ (1U<<5);
    GPIOA->CRL &=~ (1U<<6);
    GPIOA->CRL &=~ (1U<<7);

    /* Habilitar el reloj para ADC */
    RCC->APB2ENR |= ADC1EN;

    /* Calibración del ADC */
    ADC1->CR2 |= CR2_ADON;  // Encender el ADC
    ADC1->CR2 |= CR2_ADON;  // Escribir de nuevo para asegurar encendido (requerido para algunos modelos)
    ADC1->CR2 |= (1U << 3); // Iniciar calibración
    while (ADC1->CR2 & (1U << 3)); // Esperar hasta que termine la calibración

    /* Sequencia de conversión */
    ADC1->SQR3 = ADC_CH1;

    /* Largo de secuencia */
    ADC1->SQR1 = ADC_SE1_LEN_1;

    /* Configurar tiempo de muestreo para el canal 1 en 7.5 ciclos */
    ADC1->SMPR2 |= (0U << 3);  // Tiempo de muestreo de 7.5 ciclos para ADC_CH1 (PA1)

    /* Habilitar módulo ADC */
    ADC1->CR2 |= CR2_ADON;
}

void start_converstion(void){
	/*Habilitar conversion continua*/
	ADC1->CR2 |= CR2_CONT;
	/*Comenzar conversion ADC*/
	ADC1->CR2 |= CR2_SWSTART;

}

uint32_t adc_read(void){

	/*Esperar conversion completa*/
	while(!(ADC1->SR & SR_EOC)){}

	/*Lee el resultado*/
	return (ADC1->DR);

}
