#include <stdlib.h>
#include <io.h>
#include <irq.h>
#include <gpio.h>
#include <usart.h>
#include <clk.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_clk.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_usart.h>
#include <stm32_fd0_irq.h>
#include <stm32_fd0_rcc.h>
#include <stm32_test.h>

#define LED_GREEN	9
#define LED_BLUE	8

#if 1
static int stm32_test_rcc(void)
{
	struct clk *sysclk;

	sysclk = clk_get("sysclk");
	if (!sysclk)
		return -1;

	if (clk_set_rate(sysclk, MAX_48_MHZ_FREQ) < 0)
		return -1;

	clk_put(sysclk);

#if 0
	/* Set MCO */
	gpio_mux(GPIO_A, 8, 0);

	/* Configure TX GPIO */
	gpio_set_mode(GPIO_A, 8, GPIO_MODE_AF);
	gpio_set_type(GPIO_A, 8, GPIO_OTYPE_PP);
	gpio_set_speed(GPIO_A, 8, GPIO_OSPEED_HIGH);
	gpio_set_pupd(GPIO_A, 8, GPIO_PUPD_UP);

	set_value(RCC_CFGR, 5, 24, 3);
#endif

	return 0;
}
#endif

int main(void)
{
	stm32_fd0_irq_table_init();
	stm32_fd0_clk_init();
	stm32_fd0_gpio_init();
	stm32_fd0_usart_init(USART_2);

	if (stm32_test_gpio() < 0)
		goto out;

	if (stm32_test_rcc() < 0)
		goto out;

	if (stm32_test_exti() < 0)
		goto out;

#if 1
    /* (!) Connect pin to the terminal to prevent dead loop */
	if (stm32_test_usart(USART_2) < 0)
		goto out;
#endif

	if (stm32_test_stdlib() < 0)
		goto out;

	if (stm32_test_systick() < 0)
		goto out;
out:
	while (1);
}
