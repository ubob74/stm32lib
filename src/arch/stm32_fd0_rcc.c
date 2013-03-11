#include <_stdlib.h>
#include <stm32_fd0_rcc.h>
#include <io.h>

static struct rcc_clk_config stm32_fd0_rcc_clk_config[] = {
	[0] = {
		.name = "hsi",
		.freq = HSI_FREQ,
	},
	[1] = {
		.name = "sysclk",
		.freq = HSI_FREQ,
	},
	[2] = {
		.name = "hclk",
		.freq = HSI_FREQ,
	},
	[3] = {
		.name = "pclk",
		.freq = HSI_FREQ,
	},
};

static struct rcc_clk_data stm32_fd0_rcc_clk_data = {
	.rcc_clk_config = stm32_fd0_rcc_clk_config,
	.nr_rcc_clk_config =
		sizeof(stm32_fd0_rcc_clk_config)/sizeof(stm32_fd0_rcc_clk_config[0]),
};

static int __rcc_sysclk_set_src(uint8_t sysclk_src)
{
	switch (sysclk_src) {
		case HSI_SYSCLK_SRC:
			set_value(RCC_CFGR, HSI_SYSCLK_SRC, SW_SHIFT, SW_WIDTH);
			break;
		case HSE_SYSCLK_SRC:
			set_value(RCC_CFGR, HSE_SYSCLK_SRC, SW_SHIFT, SW_WIDTH);
			break;
		case PLL_SYSCLK_SRC:
			set_value(RCC_CFGR, PLL_SYSCLK_SRC, SW_SHIFT, SW_WIDTH);
			break;
		default:
			break;
	}

	return 0;
}

static int __rcc_set_freq(const char *clk_name, uint32_t freq)
{
	int i;
	struct rcc_clk_config *rcc_clk_config = stm32_fd0_rcc_clk_data.rcc_clk_config;

	if (!clk_name || !freq)
		return 0;

	for (i = 0; i < stm32_fd0_rcc_clk_data.nr_rcc_clk_config; rcc_clk_config++, i++) {
		if (!strcmp(clk_name, rcc_clk_config->name)) {
			rcc_clk_config->freq = freq;
			return 0;
		}
	}

	return -1;
}

static uint32_t __rcc_get_freq(const char *clk_name)
{
	int i;
	struct rcc_clk_config *rcc_clk_config = stm32_fd0_rcc_clk_data.rcc_clk_config;

	if (!clk_name)
		return 0;
	
	for (i = 0; i < stm32_fd0_rcc_clk_data.nr_rcc_clk_config; rcc_clk_config++, i++) {
		if (!strcmp(clk_name, rcc_clk_config->name))
			return rcc_clk_config->freq;
	}

	return 0;
}

static int __rcc_pll_enable(void)
{
	uint32_t val;

	if (test_bit(RCC_CR, PLLRDY))
		goto exit;

	/* Set up PLLON bit */
	val = __raw_readl(RCC_CR);
	val |= BIT(PLLON);
	__raw_writel(RCC_CR, val);

	/* Wait until PLLRDY is set */
	while (!test_bit(RCC_CR, PLLRDY));

exit:
	return 0;
}

static int __rcc_pll_disable(void)
{
	uint32_t val;

	if (!test_bit(RCC_CR, PLLRDY))
		goto exit;

	/* Drop PLLON bit */
	val = __raw_readl(RCC_CR);
	val &= ~(BIT(PLLON));
	__raw_writel(RCC_CR, val);

	/* Wait until PLLRDY is cleared */
	while (test_bit(RCC_CR, PLLRDY));

exit:
	return 0;
}

static void __rcc_set_pll_mul(uint8_t pllmul)
{
	set_value(RCC_CFGR, (pllmul & 0x0F),
		PLLMUL_SHIFT, PLLMUL_WIDTH);
}

static void __rcc_set_48_mhz_freq(void)
{
	uint8_t pllmul;

	/* Disable PLL */
	__rcc_pll_disable();

	/* Set PLLMUL */
	pllmul = MAX_48_MHZ_FREQ/(HSI_FREQ/2) - 2;
	__rcc_set_pll_mul(pllmul);

	/* Set SYSCLK -> PLLCLK */
	__rcc_sysclk_set_src(PLL_SYSCLK_SRC);

	/* Enable PLL */
	__rcc_pll_enable();

	/* Update RCC clocks configuration */
	__rcc_set_freq("sysclk", MAX_48_MHZ_FREQ);
	__rcc_set_freq("hclk", MAX_48_MHZ_FREQ);
	__rcc_set_freq("pclk", MAX_48_MHZ_FREQ);
}

static void __rcc_set_8_mhz_freq(void)
{
	/* TODO */
}

uint32_t stm32_fd0_rcc_get_freq(const char *clk_name)
{
	return __rcc_get_freq(clk_name);
}

int stm32_fd0_rcc_sys_clk_set_freq(uint32_t freq)
{
	int ret = 0;
	uint32_t sysclk_cur_freq;

	/* Check current SYSCLK */
	sysclk_cur_freq = __rcc_get_freq("sysclk");
	if (sysclk_cur_freq == freq)
		return 0;

	switch (freq) {
		case MAX_48_MHZ_FREQ:
			__rcc_set_48_mhz_freq();
			break;
		case HSI_8_MHZ_FREQ:
			__rcc_set_8_mhz_freq();
			break;
		default:
			ret = -1;
			break;
	}

	return ret;
}
