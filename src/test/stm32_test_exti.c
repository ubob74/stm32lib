#include <_stdlib.h>
#include <io.h>
#include <irq.h>
#include <gpio.h>
#include <clk.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_irq.h>
#include <stm32_test.h>

#define LED_BLUE	8

static int blue_led_handler(void *arg)
{
	int val = 0;
	static int i = 1;
	struct clk *clk;

	clk = clk_get("gpio_c");
	if (!clk)
		return val;

	val = (i++)%2;
	gpio_set_pin_value(GPIO_C, LED_BLUE, val);

	clk_put(clk);
	return val;
}

int stm32_test_exti(void)
{
	int ret = -1;
	struct clk *gpio_a_clk;
	struct clk *gpio_c_clk;
	struct clk *syscfg_clk;

	/* Enable GPIO_A clock */
	gpio_a_clk = clk_get("gpio_a");
	if (!gpio_a_clk)
		return ret;

	gpio_c_clk = clk_get("gpio_c");
	if (!gpio_c_clk)
		goto gpio_c_clk_err;

	/* Enable syscfg clock */
	syscfg_clk = clk_get("syscfg");
	if (!syscfg_clk)
		goto syscfg_clk_err;

	/* Config GPIO_A for PA0 */
	gpio_set_mode(GPIO_A, 0, GPIO_MODE_INPUT);
	gpio_set_pupd(GPIO_A, 0, GPIO_PUPD_DOWN);

	/* Configure PC8 (blue led) */
	gpio_set_mode(GPIO_C, 8, GPIO_MODE_OUTPUT);
	gpio_set_type(GPIO_C, 8, GPIO_OTYPE_PP);
	gpio_set_speed(GPIO_C, 8, GPIO_OSPEED_HIGH);
	gpio_set_pupd(GPIO_C, 8, GPIO_PUPD_DOWN);

	/* Config GPIO_B for PB10 */
	/*gpio_set_mode(GPIO_B, 10, GPIO_MODE_INPUT);
	gpio_set_pupd(GPIO_B, 10, GPIO_PUPD_DOWN);

	gpio_to_irq(GPIO_B, 10, EXTI10);*/
    gpio_to_irq(GPIO_A, 0, EXTI0);

	/* Request IRQ */
	/*irq_request(EXTI4_15, blue_led_handler, NULL, 0);*/
	irq_request(EXTI0_1, blue_led_handler, NULL, 0);
	/*irq_enable(EXTI4_15);*/
	irq_enable(EXTI0_1);

	ret = 0;

	clk_put(syscfg_clk);
syscfg_clk_err:
	clk_put(gpio_c_clk);
gpio_c_clk_err:
	clk_put(gpio_a_clk);

	return ret;
}

