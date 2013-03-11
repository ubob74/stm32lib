#include <io.h>

#define BIT_NUM_MAX 32

uint32_t __raw_readl(uint32_t addr)
{
	return *(volatile uint32_t *)addr;
}

uint8_t __raw_readb(uint32_t addr)
{
	return *(volatile uint8_t *)addr;
}

void __raw_writel(uint32_t addr, uint32_t val)
{
	*(uint32_t *)addr = val;
}

void __raw_writeb(uint32_t addr, uint8_t val)
{
	*(uint32_t *)addr = val;
}

int set_bit(uint32_t addr, uint8_t pos)
{
	uint32_t *val = 0;

	if (pos > BIT_NUM_MAX)
		return -1;

	val = (uint32_t *)addr;
	*val |= (1UL << pos);

	return 0;
}

int reset_bit(uint32_t addr, uint8_t pos)
{
	uint32_t *val = 0;

	if (pos > BIT_NUM_MAX)
		return -1;

	val = (uint32_t *)addr;
	*val &= ~(1UL << pos);

	return 0;
}

int test_bit(uint32_t addr, uint8_t pos)
{
	uint32_t *val = 0;

	if (pos > BIT_NUM_MAX)
		return -1;

	val = (uint32_t *)addr;
	return !!(*val & (1UL << pos));
}

int set_value(uint32_t addr, int value, uint8_t pos, int width)
{
	uint32_t regval = 0;
	int mask;

	if (pos + width > BIT_NUM_MAX)
		return -1;

	/* Get current value */
	regval = __raw_readl(addr);

	/* Drop out an old value */
	mask = (1 << width) - 1;
	regval &= ~(mask << pos);

	/* Setup a new value */
	regval |= (value << pos);
	__raw_writel(addr, regval);

	return 0;
}

int get_value(uint32_t addr, uint8_t pos, int width)
{
	uint32_t regval = 0;
	int mask;

	if (pos + width > BIT_NUM_MAX)
		return -1;

	regval = __raw_readl(addr);
	mask = (1 << width) - 1;
	return (regval >> pos) & mask;
}
