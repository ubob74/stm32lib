#ifndef _STM32_FD0_USART_H_
#define _STM32_FD0_USART_H_

#include <_stdint.h>
#include <usart.h>
#include <stm32_fd0_gpio.h>

#define USART_1		0
#define USART_2		1

#define USART1_BASE_ADDR	0x40013800
#define USART2_BASE_ADDR	0x40004400

/* USARTx register offsets */
#define USART_CR1		0x00
#define USART_CR2		0x04
#define USART_CR3		0x08
#define USART_BRR		0x0C
#define USART_GTPR		0x10
#define USART_RTOR		0x14
#define USART_RQR		0x18
#define USART_ISR		0x1C
#define USART_ICR		0x20
#define USART_RDR		0x24
#define USART_TDR		0x28

/* CR1 bits:
 * - UE			- USART enable
 * - UESM		- USART enable in stop mode
 * - RE			- receiver enable
 * - TE			- transmitter enable
 * - IDLEIE		- IDLE interrupt enable
 * - RXNEIE		- RXNE interrupt enable
 * - TCIE		- transmission complete interrupt enable
 * - TXEIE		- interrupt enable
 * - PEIE		- PE interrupt enable
 * - PS			- parity selection
 * - PCE		- parity control enable
 * - WAKE		- receiver wake-up method
 * - M			- word length
 * - OVER8		- oversampling mode
 */
#define UE			0
#define UESM		1
#define RE			2
#define TE			3
#define IDLEIE		4
#define RXNEIE		5
#define TCIE		6
#define TXEIE		7
#define PEIE		8
#define PS			9
#define PCE			10
#define WAKE		11
#define M			12
#define OVER8		15

/* CR2 bits
 * - ADDM7		- selection between 4-/7-bit address detection
 * - LBDL		- selection between 11 or 10 bit break detection
 * - STOP		- STOP bits programming
 * - SWAP		- swap TX/RX pins
 * - RXINV		- RX pin active level inversion
 * - TXINV		- TX pin active level inversion
 */
#define ADDM7		4
#define LBDL		5
#define STOP		12
#define SWAP		15
#define RXINV		16
#define TXINV		17

/* CR3 bits:
 * - EIE		- error interrupt enable
 * - HDSEL		- half-duplex selection
 * - RTSE		- RTS enable
 * - CTSE		- CTS enable
 * - CTSIE		- CTS interrupt enable
 * - DEM		- Driver enable mode
 */
#define EIE			0
#define HDSEL		3
#define RTSE		8
#define CTSE		9
#define CTSIE		10
#define DEM			14

/* Interrupt & status register
 * - PER		- parity error
 * - FE			- framing error
 * - NF			- noise detected flag
 * - ORE		- overrun error
 * - IDLE		- idle line detected
 * - RXNE		- read data register not empty
 * - TC			- transmission complete
 * - TXE		- transmit data register empty
 * - CTSIF		- CTS interrupt flag
 * - CTS		- CTS flag
 */
#define PER			0
#define FE			1
#define NF			2
#define ORE			3
#define IDLE		4
#define RXNE		5
#define TC			6
#define TXE			7
#define CTSIF		9
#define CTS			10

/* Interrupt flag clear register
 * - PECF		- parity error clear flag
 * - FECF		- framing error clear flag
 * - NCF		- noise detect clear flag
 * - ORECF		- overrun error clear flag
 * - IDLECF		- idle line detected clear flag
 * - TCCF		- transmission complete clear flag
 * - CTSCF		- CTS clear flag
 */
#define PECF		0
#define FECF		1
#define NCF			2
#define ORECF		3
#define IDLECF		4
#define TCCF		6
#define CTSCF		9

/*
 * USART GPIO mode
 */
#define USART1_TX MODE1
#define USART2_TX MODE1

int stm32_fd0_usart_init(int);

#endif /* _STM32_FD0_USART_H_ */
