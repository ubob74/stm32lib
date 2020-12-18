#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

#define BIT(x) (1UL << x)

static inline uint32_t readl(uint32_t addr)
{
	return (uint32_t)(*(volatile uint32_t *)addr);
}

static inline void writel(uint32_t addr, uint32_t val)
{
	*(uint32_t *)addr = val;
}

int set_bit(uint32_t addr, uint8_t pos);
int reset_bit(uint32_t addr, uint8_t pos);
int test_bit(uint32_t addr, uint8_t pos);

int set_value(uint32_t addr, int value, uint8_t pos, int width);
int get_value(uint32_t addr, uint8_t pos, int width);

#endif /* _IO_H_ */
