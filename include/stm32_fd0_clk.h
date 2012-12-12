#ifndef _STM32_FD0_CLK_H_
#define _STM32_FD0_CLK_H_

#include <stdint.h>

#define RCC_BASE_ADDR		0x40021000

/* Register offset */
#define RCC_CR					0x00
#define RCC_CR2					0x34
#define RCC_CFGR				0x04
#define RCC_CFGR2				0x2C
#define RCC_CFGR3				0x30
#define RCC_CIR					0x08
#define RCC_APB1RSTR		0x10
#define RCC_APB2RSTR		0x0C
#define RCC_AHBENR			0x14
#define RCC_APB1ENR			0x1C
#define RCC_APB2ENR			0x18
#define RCC_BDCR				0x20
#define RCC_CSR					0x24
#define RCC_AHBRSTR			0x28

int stm32_fd0_clk_init(void);

#endif /* _STM32_FD0_CLK_H_ */
