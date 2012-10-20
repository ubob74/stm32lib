#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stm32_fd0_clk.h>
#include <clk.h>
#include <io.h>

static struct clk stm32_fd0_clk[] = {
	[0] = {
		.name = "gpio_a",
		.ref_count = 0,
		.enable_reg = RCC_AHBENR,
		.enable_bit = 17,
	},
	[1] = {
		.name = "gpio_b",
		.ref_count = 0,
		.enable_reg = RCC_AHBENR,
		.enable_bit = 18,
	},
	[2] = {
		.name = "gpio_c",
		.ref_count = 0,
		.enable_reg = RCC_AHBENR,
		.enable_bit = 19,
	},
	[3] = {
		.name = "syscfg",
		.ref_count = 0,
		.enable_reg = RCC_APB2ENR,
		.enable_bit = 0,
	},
};

static struct clk_array stm32_fd0_clk_array = {
	.clk = stm32_fd0_clk,
	.nr_clk = sizeof(stm32_fd0_clk)/sizeof(stm32_fd0_clk[0]),
};

static struct clk * stm32_fd0_clk_get(const char *name);
int stm32_fd0_clk_put(struct clk *clk);
static int stm32_fd0_clk_enable(struct clk *clk);
static int stm32_fd0_clk_disable(struct clk *clk);

static struct clk * stm32_fd0_clk_get(const char *name)
{
	int id;
	struct clk *clk = stm32_fd0_clk_array.clk;

	for (id = 0; id < stm32_fd0_clk_array.nr_clk; clk++, id++) {
		if (!strcmp(clk->name, name)) {
			if (stm32_fd0_clk_enable(clk) < 0)
				return NULL;
	
			/* TODO: it should be atomic operation */
			clk->ref_count++;
			return clk;
		}
	}
	return NULL;
}

int stm32_fd0_clk_put(struct clk *clk)
{
	if (!clk)
		return -1;

	if (!clk->ref_count)
		return 0;

	clk->ref_count--;
	return stm32_fd0_clk_disable(clk);
}

static int stm32_fd0_clk_enable(struct clk *clk)
{
	if (!clk)
		return -1;

	return set_bit(clk->enable_reg + RCC_BASE_ADDR,
		clk->enable_bit);
}

static int stm32_fd0_clk_disable(struct clk *clk)
{
	if (!clk)
		return -1;

	return reset_bit(clk->enable_reg + RCC_BASE_ADDR,
		clk->enable_bit);
}

struct clk_ops stm32_fd0_clk_ops = {
	.get = stm32_fd0_clk_get,
	.put = stm32_fd0_clk_put,
	.enable = stm32_fd0_clk_enable,
	.disable = stm32_fd0_clk_disable,
};

/**
 * STM32 FD0 RCC init
 */
int stm32_fd0_clk_init()
{
	return clk_init(&stm32_fd0_clk_ops);
}

