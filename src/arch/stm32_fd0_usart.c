#include <stm32_fd0_usart.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_irq.h>
#include <io.h>
#include <clk.h>
#include <gpio.h>
#include <irq.h>
#include <usart.h>

static struct usart_gpio usart1_gpio[] = {
	[0] = {
		.id = GPIO_A,
		.pin_num = 9,
		.mode = USART1_TX,
	},
};

static struct usart_gpio usart2_gpio[] = {
	[0] = {
		.id = GPIO_A,
		.pin_num = 2,
		.mode = USART2_TX,
	},
};

static struct usart stm32_fd0_usart[] = {
	[0] = {
		.id = USART_1,
		.base_addr = USART1_BASE_ADDR,
		.irq = USART1,
		.clk_name = "usart1",
		.resources = {
			.usart_gpio = usart1_gpio,
			.nr_resources = 1,
		},
	},
	[1] = {
		.id = USART_2,
		.base_addr = USART2_BASE_ADDR,
		.irq = USART2,
		.clk_name = "usart2",
		.resources = {
			.usart_gpio = usart2_gpio,
			.nr_resources = 1,
		},
	},
};

struct usart_array stm32_fd0_usart_array = {
	.usart = stm32_fd0_usart,
	.nr_usart = sizeof(stm32_fd0_usart)/sizeof(stm32_fd0_usart[0]),
};

static int __transmitter_enable(uint32_t base_addr)
{
	uint32_t cr1_val;

	set_bit(base_addr + USART_CR1, TE);

	cr1_val = __raw_readl(base_addr + USART_CR1);
	cr1_val |= BIT(TXEIE) | BIT(TCIE);
	__raw_writel(base_addr + USART_CR1, cr1_val);

	return 0;
}

static int __transmitter_disable(uint32_t base_addr)
{
	return reset_bit(base_addr + USART_CR1, TE);
}

static int __receiver_enable(uint32_t base_addr)
{
	/* TODO */
	return 0;
}

static int __receiver_disable(uint32_t base_addr)
{
	/* TODO */
	return 0;
}

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

static int stm32_fd0_usart_set_parity(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return set_bit(usart->base_addr + USART_CR1, PCE);
}

static int stm32_fd0_usart_enable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return set_bit(usart->base_addr + USART_CR1, UE);
}

static int stm32_fd0_usart_disable(int id)
{
	struct usart *usart;

	if (id < 0 || id > 1)
		return -1;

	usart = stm32_fd0_usart_array.usart + id;

	return reset_bit(usart->base_addr + USART_CR1, UE);
}

static int stm32_fd0_usart_irq_handler(void *arg)
{
	uint32_t isr, cr1;
	uint8_t val;
	struct usart *usart = (struct usart *)arg;
	struct usart_data *usart_data = usart->usart_data;

	isr = __raw_readl(usart->base_addr + USART_ISR);

	if (isr & BIT(TXE)) {
		if (usart_data->cur_pos < usart_data->size) {
			val = usart_data->data[usart_data->cur_pos];
			__raw_writeb(usart->base_addr + USART_TDR, val);
			usart_data->cur_pos++;
			return 0;
		}
	}

	if (isr & BIT(TC)) {
		/* clear TXEIE and TCIE flags */
		cr1 = __raw_readl(usart->base_addr + USART_CR1);
		cr1 &= ~(BIT(TXEIE) | BIT(TCIE));
		__raw_writel(usart->base_addr + USART_CR1, cr1);

		/* complete transmission */
		usart_data->complete = 1;
	}

	return 0;
}

int stm32_fd0_usart_start_tx(int id, struct usart_data *usart_data)
{
	struct usart *usart = stm32_fd0_usart + id;

	if (!usart_data->data ||
		!usart_data->size)
		return -1;

	usart_data->complete = 0;
	usart_data->cur_pos = 0;

	usart->usart_data = usart_data;

	irq_enable(usart->irq);

	/* enable USART2 transmitter */
	__transmitter_enable(usart->base_addr);

	/* wait for completion */
	for(;usart_data->complete != 1;);

	/* disable transmitter and USART2 */
	__transmitter_disable(usart->base_addr);

	irq_disable(usart->irq);

	usart_data->complete = 0;
	usart_data->cur_pos = 0;

	return 0;
}

int stm32_fd0_usart_start_rx(int id, struct usart_data *usart_data)
{
	return 0;
}

struct usart_ops stm32_fd0_usart_ops = {
	.usart_enable = stm32_fd0_usart_enable,
	.usart_disable = stm32_fd0_usart_disable,
	.set_word_length = stm32_fd0_usart_set_word_length,
	.set_baud_rate = stm32_fd0_usart_set_baud_rate,
	.set_stop_bit = stm32_fd0_usart_set_stop_bit,
	.set_parity = stm32_fd0_usart_set_parity,
	.start_tx = stm32_fd0_usart_start_tx,
	.start_rx = stm32_fd0_usart_start_rx,
};

int stm32_fd0_usart_init(int id)
{
	int i;
	struct clk *gpio_a;
	struct usart *usart = stm32_fd0_usart + id;
	struct usart_gpio *usart_gpio;

	/* Enable clocks */
	usart->clk = clk_get(usart->clk_name);
	if (!usart->clk)
		return -1;

	gpio_a = clk_get("gpio_a");
	if (!gpio_a) {
		clk_put(usart->clk);
		return -1;
	}

	for (i = 0; i < usart->resources.nr_resources; i++) {
		usart_gpio = usart->resources.usart_gpio + i;

		/* Set mode (USART2 TX) */
		gpio_mux(usart_gpio->id,
			usart_gpio->pin_num, usart_gpio->mode);

		/* Configure TX GPIO */
		gpio_set_mode(usart_gpio->id, usart_gpio->pin_num, GPIO_MODE_AF);
		gpio_set_type(usart_gpio->id, usart_gpio->pin_num, GPIO_OTYPE_PP);
		gpio_set_speed(usart_gpio->id, usart_gpio->pin_num, GPIO_OSPEED_HIGH);
		gpio_set_pupd(usart_gpio->id, usart_gpio->pin_num, GPIO_PUPD_UP);
	}

	atomic_init(&usart->ref_count);
	usart->irq_handler = stm32_fd0_usart_irq_handler;
	irq_request(usart->irq, usart->irq_handler, (void *)usart, 0);

	usart_init(&stm32_fd0_usart_ops);

	return 0;
}
