#include <uart.h>


#define GPIOAEN 			(1U<<2)  //0000 0000 0000 0000 0000 0000 0000 0100 Máscara para habilitar el reloj de GPIOC (Bit 4 en el registro RCC)
//#define GPIOCEN 			(1U<<4)  //0000 0000 0000 0000 0000 0000 0001 0000 Máscara para habilitar el reloj de GPIOC (Bit 4 en el registro RCC)
#define UART1EN				(1U<<14)

#define CR1_RE 				(1U<<2)
#define CR1_TE 				(1U<<3)

#define SR_TXE	 			(1U<<7)
#define SR_RXNE	 			(1U<<5)
#define CR1_UE 				(1U<<13)

#define SYS_FREQ 			8000000
#define APB2_CLK			SYS_FREQ
#define UART_BAUDRATE		115200

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudrRate);
static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudrRate);


void uart1_write(int ch);

int __io_putchar(int ch){
	uart1_write(ch);
	return ch;
}

void uart1_rxtx_init(void){
	//PINA9 UART TX
	//**** Habilitar reloj para el GPIOA****//
	   RCC->APB2ENR |= GPIOAEN;

	//Habilitar pin9 como salida funcion alternativa UART TX
		GPIOA->CRH   |= (1U<<4);
		GPIOA->CRH   |= (1U<<5);
		GPIOA->CRH   &=~(1U<<6);
		GPIOA->CRH   |= (1U<<7);

	//Habilitar pin10 como entrada UART RX
		GPIOA->CRH   &=~(1U<<8);
		GPIOA->CRH   &=~(1U<<9);
		GPIOA->CRH   &=~(1U<<10);
		GPIOA->CRH   |= (1U<<11);

	//**** Habilitar reloj para el GPIOA****//
		RCC->APB2ENR |= UART1EN;

	//Configurar Baudios
		uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	//Configurar direccion de tranferencia Y recepcion
		USART1->CR1 = (CR1_TE | CR1_RE);

	//Habilitar modulo UART
		USART1->CR1 |= CR1_UE;

}


void uart1_tx_init(void){
	//PINA9 UART TX
	//**** Habilitar reloj para el GPIOA****//
	RCC->APB2ENR |= GPIOAEN;

	//Habilitar pin como salida funcion alternativa UART TX

		GPIOA->CRH   |= (1U<<4);
		GPIOA->CRH   |= (1U<<5);
		GPIOA->CRH   &=~(1U<<6);
		GPIOA->CRH   |= (1U<<7);

	//**** Habilitar reloj para el GPIOA****//
		RCC->APB2ENR |= UART1EN;

	//Configurar Baudios
		uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	//Configurar direccion de tranferencia
		USART1->CR1 = CR1_TE;

	//Habilitar modulo UART
		USART1->CR1 |= CR1_UE;

}

char uart1_read(void){

	//Asegurar que el receptor no este vacio
	while(!(USART1-> SR & SR_RXNE)){}

	//Leer al receptor
	return USART1-> DR;
}


void uart1_write(int ch){

	//Asegurar que el transmisor este vacio
	while(!(USART1-> SR & SR_TXE)){}

	//Escribir al transmisor
	USART1-> DR = (ch & 0xFF);
}


static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudrRate){

	USARTx->BRR = compute_uart_bd(PeriphClk, BaudrRate);

}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudrRate){

	return ((PeriphClk + (BaudrRate/2U))/BaudrRate);

}
