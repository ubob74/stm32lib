#ifndef _STM32_FD0_GPIO_H_
#define _STM32_FD0_GPIO_H_

#include <stdint.h>

#define GPIO_BASE				0x48000000

/* GPIOx base address */
#define GPIO_A_BASE_ADDR		(GPIO_BASE)					/* 0x4800 0000 */
#define GPIO_B_BASE_ADDR		(GPIO_BASE + 0x0400)		/* 0x4800 0400 */
#define GPIO_C_BASE_ADDR		(GPIO_BASE + 0x0800)		/* 0x4800 0800 */
#define GPIO_D_BASE_ADDR		(GPIO_BASE + 0x0C00)		/* 0x4800 0C00 */
#define GPIO_F_BASE_ADDR		(GPIO_BASE + 0x1400)		/* 0x4800 1400 */

/* Register offset */
#define GPIO_MODER_OFFSET		0x00
#define GPIO_OTYPER_OFFSET		0x04
#define GPIO_OSPEEDR_OFFSET		0x08
#define GPIO_PUPDR_OFFSET		0x0C
#define GPIO_IDR_OFFSET			0x10
#define GPIO_ODR_OFFSET			0x14
#define GPIO_BSRR_OFFSET		0x18
#define GPIO_AFRL_OFFSET		0x20
#define GPIO_AFRH_OFFSET		0x24
#define GPIO_BRR_OFFSET			0x28

/* GPIO mode */
#define GPIO_MODE_INPUT			0x00
#define GPIO_MODE_OUTPUT		0x01
#define GPIO_MODE_AF			0x02
#define GPIO_MODE_ANALOG		0x03

/* GPIO output type */
#define GPIO_OTYPE_PP			0x00
#define GPIO_OTYPE_OD			0x01

/* GPIO output speed */
#define GPIO_OSPEED_LOW			0x00
#define GPIO_OSPEED_MEDIUM		0x01
#define GPIO_OSPEED_HIGH		0x03

/* GPIO pull-up/down configuration */
#define GPIO_PUPD_NOPULL		0x00
#define GPIO_PUPD_UP			0x01
#define GPIO_PUPD_DOWN			0x02

#define GPIO_A	0
#define GPIO_B	1
#define GPIO_C	2
#define GPIO_D	3
#define GPIO_F	4

#define PIN_NUM_MAX 16

/* GPIO modes */
#define MODE0		0
#define MODE1		1
#define MODE2		2
#define MODE3		3
#define MODE4		4
#define MODE5		5
#define MODE6		6
#define MODE7		7

int stm32_fd0_gpio_init(void);

#endif /* _STM32_FD0_GPIO_H_ */
