#include <_stdlib.h>

#include <stm32_fd0_rcc.h>
#include <stm32_fd0_clk.h>
#include <clk.h>
#include <io.h>

static struct clk stm32_fd0_clk[] = {
	[0] = {
		.name = "sysclk",
		.flags = ALWAYS_ON,
		.set_rate = stm32_fd0_rcc_sys_clk_set_freq,
		.get_rate = stm32_fd0_rcc_get_freq,
	},
	[1] = {
		.name = "hclk",
		.parent = "sysclk",
		.flags = ALWAYS_ON,
		.get_rate = stm32_fd0_rcc_get_freq,
	},
	[2] = {
		.name = "pclk",
		.parent = "hclk",
		.flags = ALWAYS_ON,
		.get_rate = stm32_fd0_rcc_get_freq,
	},
	[3] = {
		.name = "gpio_a",
		.parent = "pclk",
		.enable_reg = RCC_AHBENR,
		.enable_bit = 17,
	},
	[4] = {
		.name = "gpio_b",
		.parent = "pclk",
		.enable_reg = RCC_AHBENR,
		.enable_bit = 18,
	},
	[5] = {
		.name = "gpio_c",
		.parent = "pclk",
		.enable_reg = RCC_AHBENR,
		.enable_bit = 19,
	},
	[6] = {
		.name = "syscfg",
		.parent = "pclk",
		.enable_reg = RCC_APB2ENR,
		.enable_bit = 0,
	},
	[7] = {
		.name = "usart1",
		.parent = "pclk",
		.enable_reg = RCC_APB2ENR,
		.enable_bit = 14,
	},
	[8] = {
		.name = "usart2",
		.parent = "pclk",
		.enable_reg = RCC_APB1ENR,
		.enable_bit = 17,
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

	if (!name)
		return NULL;

	for (id = 0; id < stm32_fd0_clk_array.nr_clk; clk++, id++) {
		if (!strcmp(clk->name, name)) {
			if (clk->flags & ALWAYS_ON)
				return clk;

			if (stm32_fd0_clk_enable(clk) < 0)
				return NULL;
	
			atomic_inc(&clk->ref_count);
			return clk;
		}
	}

	return NULL;
}

int stm32_fd0_clk_put(struct clk *clk)
{
	if (!clk)
		return -1;

	if (clk->flags & ALWAYS_ON)
		return 0;

	atomic_dec(&clk->ref_count);

	if (!atomic_read(&clk->ref_count))
		stm32_fd0_clk_disable(clk);

	return 0;
}

static struct clk * stm32_fd0_clk_get_parent(struct clk *clk)
{
	if (!clk)
		return NULL;
		
	return stm32_fd0_clk_get(clk->parent);
}

static int stm32_fd0_clk_enable(struct clk *clk)
{
	if (!clk)
		return -1;

	if (clk->flags & ALWAYS_ON)
		return 0;

	return set_bit(clk->enable_reg,
		clk->enable_bit);
}

static int stm32_fd0_clk_disable(struct clk *clk)
{
	if (!clk)
		return -1;

	if (clk->flags & ALWAYS_ON)
		return 0;

	return reset_bit(clk->enable_reg,
		clk->enable_bit);
}

static int stm32_fd0_clk_set_rate(struct clk *clk, uint32_t rate)
{
	if (!clk)
		return -1;

	return (clk->set_rate) ? clk->set_rate(rate) : 0;
}

static uint32_t stm32_fd0_clk_get_rate(struct clk *clk)
{
	if (!clk)
		return 0;

	return (clk->get_rate) ? clk->get_rate(clk->name) : 0;
}

struct clk_ops stm32_fd0_clk_ops = {
	.get = stm32_fd0_clk_get,
	.put = stm32_fd0_clk_put,
	.get_parent = stm32_fd0_clk_get_parent,
	.enable = stm32_fd0_clk_enable,
	.disable = stm32_fd0_clk_disable,
	.set_rate = stm32_fd0_clk_set_rate,
	.get_rate = stm32_fd0_clk_get_rate,
};

/**
 * STM32 F0 RCC init
 */
int stm32_fd0_clk_init()
{
	int id;
	struct clk *clk = stm32_fd0_clk_array.clk;

	for (id = 0; id < stm32_fd0_clk_array.nr_clk; clk++, id++)
		atomic_init(&clk->ref_count);

	return clk_init(&stm32_fd0_clk_ops);
}
