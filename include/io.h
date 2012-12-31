#ifndef _IO_H_
#define _IO_H_

#include <_stdint.h>

#ifndef NULL
#define NULL ((void *)0)
#endif

#define BIT(x) (1UL << x)

uint32_t __raw_readl(uint32_t addr);
uint8_t __raw_readb(uint32_t addr);
void __raw_writel(uint32_t addr, uint32_t val);
void __raw_writeb(uint32_t addr, uint8_t val);

int set_bit(uint32_t addr, uint8_t pos);
int reset_bit(uint32_t addr, uint8_t pos);
int test_bit(uint32_t addr, uint8_t pos);

int set_value(uint32_t addr, int value, uint8_t pos, int width);
int get_value(uint32_t addr, uint8_t pos, int width);

#endif /* _IO_H_ */
