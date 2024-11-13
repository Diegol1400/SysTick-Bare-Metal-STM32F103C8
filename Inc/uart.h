
#ifndef UART_H_
#define UART_H_

#include <stm32f103xb.h>
#include <stdint.h>

void uart1_tx_init(void);
char uart1_read(void);
void uart1_rxtx_init(void);


#endif /* UART_H_ */
