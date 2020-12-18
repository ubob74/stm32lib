#include <io.h>

#define BIT_NUM_MAX 32

int set_bit(uint32_t addr, uint8_t pos)
{
	uint32_t val = readl(addr);
	val |= (1UL << pos);
	writel(addr, val);
	return 0;
}

int reset_bit(uint32_t addr, uint8_t pos)
{
	uint32_t val = readl(addr);
	val &= ~(1UL << pos);
	writel(addr, val);
	return 0;
}

int test_bit(uint32_t addr, uint8_t pos)
{
	uint32_t val = readl(addr);
	return !!(val & (1UL << pos));
}

int set_value(uint32_t addr, int value, uint8_t pos, int width)
{
	uint32_t regval = 0;
	int mask;

	if (pos + width > BIT_NUM_MAX)
		return -1;

	/* Get current value */
	regval = readl(addr);

	/* Drop out an old value */
	mask = (1 << width) - 1;
	regval &= ~(mask << pos);

	/* Setup a new value */
	regval |= (value << pos);
	writel(addr, regval);

	return 0;
}

int get_value(uint32_t addr, uint8_t pos, int width)
{
	uint32_t regval = 0;
	int mask;

	if (pos + width > BIT_NUM_MAX)
		return -1;

	regval = readl(addr);
	mask = (1 << width) - 1;
	return (regval >> pos) & mask;
}
