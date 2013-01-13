#include <_stdlib.h>
#include <gpio.h>
#include <clk.h>
#include <stm32_fd0_gpio.h>
#include <stm32_test.h>

#define LED_GREEN	9
#define LED_BLUE	8

int stm32_test_gpio(void)
{
	int ret = -1;
	struct clk *clk;
	uint8_t pin_num;
	uint32_t id = GPIO_C;
	int mode, otype, ospeed, pupd;

	clk = clk_get("gpio_c");
	if (!clk)
		return -1;

	/* Set GPIO_C parameters */
	for (pin_num = 0; pin_num < PIN_NUM_MAX; pin_num++) {
		gpio_set_mode(id, pin_num, GPIO_MODE_OUTPUT);
		gpio_set_type(id, pin_num, GPIO_OTYPE_PP);
		gpio_set_speed(id, pin_num, GPIO_OSPEED_HIGH);
		gpio_set_pupd(id, pin_num, GPIO_PUPD_NOPULL);
	}

	/* Check GPIO_C settings */
	for (pin_num = 0; pin_num < PIN_NUM_MAX; pin_num++) {
		/* Test mode */
		mode = gpio_get_mode(id, pin_num);
		if (mode != GPIO_MODE_OUTPUT)
			goto err;

		/* Test output type */
		otype = gpio_get_type(id, pin_num);
		if (otype != GPIO_OTYPE_PP)
			goto err;

		/* Test output speed */
		ospeed = gpio_get_speed(id, pin_num);
		if (ospeed != GPIO_OSPEED_HIGH)
			goto err;

		/* Test pull-up/down */
		pupd = gpio_get_pupd(id, pin_num);
		if (pupd != GPIO_PUPD_NOPULL)
			goto err;
	}

	gpio_set_pin_value(GPIO_C, LED_BLUE, 1);
	gpio_set_pin_value(GPIO_C, LED_GREEN, 1);

	gpio_set_pin_value(GPIO_C, LED_BLUE, 0);
	gpio_set_pin_value(GPIO_C, LED_GREEN, 0);
	ret = 0;

err:
	clk_put(clk);
	return ret;
}

