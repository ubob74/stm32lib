#include <io.h>
#include <nvic.h>
#include <irq.h>
#include <gpio.h>
#include <usart.h>
#include <clk.h>
#include <systick.h>
#include <exceptions.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_clk.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_usart.h>
#include <stm32_fd0_irq.h>

#define LED_GREEN	9
#define LED_BLUE	8

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

#if 1
static int _test_gpio(void)
{
	int ret = -1;
	struct clk *clk;
	uint8_t pin_num;
	uint32_t id = GPIO_C;
	/*int mode, otype, ospeed, pupd;*/

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

	ret = 0;
	clk_put(clk);
	return ret;

	/* Check GPIO_C settings */
#if 0
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
#endif
/*
	gpio_set_pin_value(GPIO_C, LED_BLUE, 1);
	gpio_set_pin_value(GPIO_C, LED_GREEN, 1);

	gpio_set_pin_value(GPIO_C, LED_BLUE, 0);
	gpio_set_pin_value(GPIO_C, LED_GREEN, 0);
*/
/*
	ret = 0;

err:
	clk_put(clk);
	return ret;
*/
}
#endif

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

	/* Mux PA0 to EXTI0 (default configuartion) */
	stm32_exti_cfg_mode(SYSCFG_EXTICR1, EXTI_CFG_0, EXTI_PA_MODE);

	/* Mux PA1 to EXTI1 (default configuartion) */
	stm32_exti_cfg_mode(SYSCFG_EXTICR1, EXTI_CFG_1, EXTI_PA_MODE);

	/* Config GPIO_A for PA0 */
	gpio_set_mode(GPIO_A, 0, GPIO_MODE_INPUT);
	gpio_set_pupd(GPIO_A, 0, GPIO_PUPD_NOPULL);

	/* Config GPIO_A for PA1 */
	gpio_set_mode(GPIO_A, 1, GPIO_MODE_INPUT);
	gpio_set_pupd(GPIO_A, 1, GPIO_PUPD_DOWN);

	/* Config EXTI1 line */
	bit_mask = BIT(EXTI1);
	stm32_exti_set_irq_mask(bit_mask);
	stm32_exti_set_rising_trigger(bit_mask);

	/* Request IRQ */
	irq_request(EXTI0_1, blue_led_handler, NULL, 0);
	irq_enable(EXTI0_1);

	ret = 0;
	return ret;

	/*clk_put(syscfg_clk);*/
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

void _test_usart(void)
{
	struct usart_data usart_data;
	uint8_t data[8] = {'9', 'A', '7', '6', '5', '3', '2', '1'};

	usart_data.data = &data[0];
	usart_data.size = sizeof(data);
	usart_data.cur_pos = 0;
	usart_data.complete = 0;

	/* enable USART2 */
	usart_enable(USART_2);

	/* set USART parameters */
	usart_set_word_length(USART_2, 8);
	usart_set_baud_rate(USART_2, 9600);
	usart_set_stop_bit(USART_2, 1);

	usart_start_transmission(USART_2, &usart_data);

	usart_disable(USART_2);
}

int main(void)
{
	stm32_fd0_irq_table_init();
	stm32_fd0_clk_init();
	stm32_fd0_gpio_init();
	stm32_fd0_usart_init();

	if (_test_gpio() < 0)
		goto out;

	_test_exti0();
	_test_systick();
	_test_usart();

out:
	while (1);
}
