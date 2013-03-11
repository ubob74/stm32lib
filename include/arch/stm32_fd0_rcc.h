#ifndef _STM32_FD0_RCC_H_
#define _STM32_FD0_RCC_H_

#include <_stdint.h>

#define HSI_FREQ 8000000UL
#define HSI_8_MHZ_FREQ HSI_FREQ
#define MAX_48_MHZ_FREQ 48000000UL

struct rcc_clk_config {
	const char *name;
	uint32_t freq;
};

struct rcc_clk_data {
	struct rcc_clk_config *rcc_clk_config;
	int nr_rcc_clk_config;
};

#define RCC_BASE_ADDR		0x40021000

/* Register offset */
#define RCC_CR				(RCC_BASE_ADDR + 0x00)
#define RCC_CR2				(RCC_BASE_ADDR + 0x34)
#define RCC_CFGR			(RCC_BASE_ADDR + 0x04)
#define RCC_CFGR2			(RCC_BASE_ADDR + 0x2C)
#define RCC_CFGR3			(RCC_BASE_ADDR + 0x30)
#define RCC_CIR				(RCC_BASE_ADDR + 0x08)
#define RCC_APB1RSTR		(RCC_BASE_ADDR + 0x10)
#define RCC_APB2RSTR		(RCC_BASE_ADDR + 0x0C)
#define RCC_AHBENR			(RCC_BASE_ADDR + 0x14)
#define RCC_APB1ENR			(RCC_BASE_ADDR + 0x1C)
#define RCC_APB2ENR			(RCC_BASE_ADDR + 0x18)
#define RCC_BDCR			(RCC_BASE_ADDR + 0x20)
#define RCC_CSR				(RCC_BASE_ADDR + 0x24)
#define RCC_AHBRSTR			(RCC_BASE_ADDR + 0x28)

/* SYSCLK source */
#define HSI_SYSCLK_SRC		0
#define HSE_SYSCLK_SRC		1
#define PLL_SYSCLK_SRC		2

/* Clock Control Register bits */
#define PLLRDY		25
#define PLLON		24
#define HSIRDY		1
#define HSION		0

/* Clock configuration register bits */
#define PLLSRC			16
#define PLLMUL_SHIFT	18
#define PLLMUL_WIDTH	4
#define SW_SHIFT		0
#define SW_WIDTH		2

uint32_t stm32_fd0_rcc_get_freq(const char *clk_name);
int stm32_fd0_rcc_sys_clk_set_freq(uint32_t freq);

#endif
