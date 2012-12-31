#include <_stdlib.h>
#include <io.h>
#include <irq.h>
#include <gpio.h>
#include <clk.h>
#include <systick.h>
#include <exceptions.h>
#include <stm32_fd0_gpio.h>
#include <stm32_test.h>

#define LED_GREEN	9

static int green_led_handler(void *arg)
{
	int val = 0;
	static int i = 0;
	struct clk *clk;

	clk = clk_get("gpio_c");
	if (!clk)
		return val;

	val = (i++)%2;
	gpio_set_pin_value(GPIO_C, LED_GREEN, val);

	clk_put(clk);
	return val;
}

int stm32_test_systick(void)
{
	uint32_t freq = 8000000; /* 8 MHz */
	uint32_t csr_val = 0;
	uint32_t rvr_val = 0;
	uint32_t cvr_val = 0;
	struct clk *clk;

	/* Use processor clock 8 MHz */
	csr_val = BIT(CSR_ENABLE) | BIT(CSR_TICKINT) | BIT(CSR_CLKSRC);
	rvr_val = freq;

	systick_set_csr(csr_val);
	systick_set_rvr(rvr_val);
	systick_set_cvr(cvr_val);

	clk = clk_get("gpio_c");
	if (!clk)
		return -1;

	/* Configure PC9 */
	gpio_set_mode(GPIO_C, 9, GPIO_MODE_OUTPUT);
	gpio_set_type(GPIO_C, 9, GPIO_OTYPE_PP);
	gpio_set_speed(GPIO_C, 9, GPIO_OSPEED_HIGH);
	gpio_set_pupd(GPIO_C, 9, GPIO_PUPD_NOPULL);

	clk_put(clk);

	set_exception(SYSTICK_EXCEPTION, green_led_handler, NULL, 0);

	return 0;
}
