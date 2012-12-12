#include <stm32_fd0_usart.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_irq.h>
#include <io.h>
#include <clk.h>
#include <gpio.h>
#include <irq.h>
#include <usart.h>

static struct usart stm32_fd0_usart[] = {
	[0] = {
		.id = USART_1,
		.base_addr = USART1_BASE_ADDR,
	},
	[1] = {
		.id = USART_2,
		.base_addr = USART2_BASE_ADDR,
	},
};

struct usart_array stm32_fd0_usart_array = {
	.usart = stm32_fd0_usart,
	.nr_usart = sizeof(stm32_fd0_usart)/sizeof(stm32_fd0_usart[0]),
};

static int stm32_fd0_usart_set_word_length(int id, uint8_t word_length)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	switch (word_length) {
	case 8:
		reset_bit(usart->base_addr + USART_CR1, M);
		break;
	case 9:
		set_bit(usart->base_addr + USART_CR1, M);
		break;
	default:
		return -1;
	}

	return 0;
}

static int stm32_fd0_usart_set_baud_rate(int id, uint32_t baud_rate)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	switch (baud_rate) {
	case 9600:
		__raw_writel(usart->base_addr + USART_BRR, 0x341);
		break;
	default:
		return -1;
	}

	return 0;
}

static int stm32_fd0_usart_set_stop_bit(int id, uint8_t stop_bit)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	/* set stop bit */
	switch (stop_bit) {
	case 1:
		set_value(usart->base_addr + USART_CR2, 0, STOP, 2);
		break;
	case 2:
		set_value(usart->base_addr + USART_CR2, 2, STOP, 2);
		break;
	default:
		return -1;
	}

	return 0;
}

int stm32_fd0_usart_set_reg(uint32_t reg, uint32_t bit_mask)
{
	return 0;
}

int stm32_fd0_usart_enable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return set_bit(usart->base_addr + USART_CR1, UE);
}

int stm32_fd0_usart_disable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return reset_bit(usart->base_addr + USART_CR1, UE);
}

int stm32_fd0_usart_transmitter_enable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return set_bit(usart->base_addr + USART_CR1, TE);
}

int stm32_fd0_usart_transmitter_disable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return reset_bit(usart->base_addr + USART_CR1, TE);
}

int stm32_fd0_usart_receiver_enable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return 0;
}

int stm32_fd0_usart_receiver_disable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return 0;
}

struct usart_ops stm32_fd0_usart_ops = {
	.usart_enable = stm32_fd0_usart_enable,
	.usart_disable = stm32_fd0_usart_disable,
	.set_word_length = stm32_fd0_usart_set_word_length,
	.set_baud_rate = stm32_fd0_usart_set_baud_rate,
	.set_stop_bit = stm32_fd0_usart_set_stop_bit,
	.transmitter_enable = stm32_fd0_usart_transmitter_enable,
	.transmitter_disable = stm32_fd0_usart_transmitter_disable,
	.receiver_enable = stm32_fd0_usart_receiver_enable,
	.receiver_disable = stm32_fd0_usart_receiver_disable,
};

int stm32_fd0_usart_init(void)
{
	int ret = 0;
	struct clk *usart2_clk, *gpio_a;

	/* Enable clocks */
	usart2_clk = clk_get("usart2");
	if (!usart2_clk)
		return -1;

	gpio_a = clk_get("gpio_a");
	if (!gpio_a) {
		clk_put(usart2_clk);
		return -1;
	}

	/* Set PA2 to MODE1 (USART2 TX) */
	gpio_mux(GPIO_A, 2, MODE1);

	/* Configure TX GPIO (PA2) */
	gpio_set_mode(GPIO_A, 2, GPIO_MODE_AF);
	gpio_set_type(GPIO_A, 2, GPIO_OTYPE_PP);
	gpio_set_speed(GPIO_A, 2, GPIO_OSPEED_HIGH);
	gpio_set_pupd(GPIO_A, 2, GPIO_PUPD_UP);

	usart_init(&stm32_fd0_usart_ops);

	return ret;
}
