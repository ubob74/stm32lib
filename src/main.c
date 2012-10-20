#include <io.h>
#include <nvic.h>
#include <irq_handler.h>
#include <gpio.h>
#include <clk.h>
#include <systick.h>
#include <exceptions.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_clk.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_irq_handler.h>

#define LED_GREEN	PC9
#define LED_BLUE	PC8

int blue_led_handler(void *arg)
{
	int val = 0;
	static int i = 1;
	struct clk *clk;

	clk = clk_get("gpio_c");
	if (!clk)
		return val;

	val = (i++)%2;
	gpio_set_pin_value(GPIO_C, LED_BLUE, val);

	val = stm32_exti_get_pending(EXTI0);
	if (val)
		stm32_exti_reset_pending(EXTI0);

	clk_put(clk);
	return val;
}

int green_led_handler(void *arg)
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

static int _test_gpio(void)
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
		gpio_set_output_type(id, pin_num, GPIO_OTYPE_PP);
		gpio_set_output_speed(id, pin_num, GPIO_OSPEED_HIGH);
		gpio_set_pupd(id, pin_num, GPIO_PUPD_NOPULL);
	}

	/* Check GPIO_C settings */
	for (pin_num = 0; pin_num < PIN_NUM_MAX; pin_num++) {
		/* Test mode */
		mode = gpio_get_mode(id, pin_num);
		if (mode != GPIO_MODE_OUTPUT)
			goto err;

		/* Test output type */
		otype = gpio_get_output_type(id, pin_num);
		if (otype != GPIO_OTYPE_PP)
			goto err;

		/* Test output speed */
		ospeed = gpio_get_output_speed(id, pin_num);
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

int _test_exti0()
{
	int ret = -1;
	uint32_t bit_mask = 0;
	struct clk *gpio_a_clk, *gpio_c_clk, *syscfg_clk;

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
	gpio_set_mode(GPIO_A, PA0, GPIO_MODE_INPUT);
	gpio_set_pupd(GPIO_A, PA0, GPIO_PUPD_NOPULL);

	/* Config EXTI0 line */
	bit_mask |= BIT(EXTI0);
	stm32_exti_set_irq_mask(bit_mask);
	stm32_exti_set_falling_trigger(bit_mask);

	/* Request IRQ */
	irq_request(EXTI0_1, blue_led_handler, NULL, 0);

	ret = 0;

	clk_put(syscfg_clk);
syscfg_clk_err:
	clk_put(gpio_c_clk);
gpio_c_clk_err:
	clk_put(gpio_a_clk);

	return ret;
}

void _test_systick(void)
{
	uint32_t freq = 8000000; /* 8 MHz */
	uint32_t csr_val = 0;
	uint32_t rvr_val = 0;
	uint32_t cvr_val = 0;

	/* Use processor clock 8 MHz */
	csr_val = BIT(CSR_ENABLE) | BIT(CSR_TICKINT) | BIT(CSR_CLKSRC);
	rvr_val = freq;

	systick_set_csr(csr_val);
	systick_set_rvr(rvr_val);
	systick_set_cvr(cvr_val);

	set_exception(SYSTICK_EXCEPTION, green_led_handler, NULL, 0);
}

int main(void)
{
	stm32_fd0_irq_handler_table_init();
	stm32_fd0_clk_init();
	stm32_fd0_gpio_init();

	if (_test_gpio() < 0)
		goto out;

	_test_exti0();

	_test_systick();
	
out:
	while (1);
}

