#include <stm32_fd0_usart.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_irq.h>
#include <io.h>
#include <clk.h>
#include <gpio.h>
#include <irq.h>
#include <usart.h>

static int usart_default = USART_2;

static struct usart_gpio usart1_gpio[] = {
	[0] = {
		.id = GPIO_A,
		.pin_num = 9,
		.mode = USART1_TX,
		.clk_name = "gpio_a",
	},
};

static struct usart_gpio usart2_gpio[] = {
	[0] = {
		.id = GPIO_A,
		.pin_num = 2,
		.mode = USART2_TX,
		.clk_name = "gpio_a",
	},
};

static struct usart stm32_fd0_usart[] = {
	[0] = {
		.id = USART_1,
		.base_addr = USART1_BASE_ADDR,
		.irq = USART1,
		.clk_name = "usart1",
		.state = USART_DISABLE,
		.usart_resources = {
			.gpio = usart1_gpio,
			.nr_gpio = ARRAY_SIZE(usart1_gpio),
		},
	},
	[1] = {
		.id = USART_2,
		.base_addr = USART2_BASE_ADDR,
		.irq = USART2,
		.clk_name = "usart2",
		.state = USART_ENABLE,
		.usart_resources = {
			.gpio = usart2_gpio,
			.nr_gpio = ARRAY_SIZE(usart2_gpio),
		},
	},
};

struct usart_array stm32_fd0_usart_array = {
	.usart = stm32_fd0_usart,
	.nr_usart = ARRAY_SIZE(stm32_fd0_usart),
};

static inline void __enable_tx(uint32_t base_addr)
{
	uint32_t cr1 = readl(base_addr + USART_CR1);
	cr1 |= BIT(TXEIE) | BIT(TE);
	writel(base_addr + USART_CR1, cr1);
}

static inline void __disable_tx(uint32_t base_addr)
{
	reset_bit(base_addr + USART_CR1, TE);
}

static __attribute__((unused)) int __enable_rx(uint32_t base_addr)
{
	/* TODO */
	return 0;
}

static __attribute__((unused)) int __disable_rx(uint32_t base_addr)
{
	/* TODO */
	return 0;
}

static inline struct usart *get_usart(void)
{
	return stm32_fd0_usart_array.usart + usart_default;
}

static int stm32_fd0_usart_set_default(u8 id)
{
	if (id > stm32_fd0_usart_array.nr_usart - 1)
		return -1;
	usart_default = id;
	return 0;
}

static int stm32_fd0_usart_set_word_length(uint8_t word_length)
{
	struct usart *usart = get_usart();

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

static int stm32_fd0_usart_set_baud_rate(uint32_t baud_rate)
{
	struct usart *usart = get_usart();
	uint32_t brr_val;
	struct clk *pclk;
	uint32_t pclk_rate;

	pclk = clk_get_parent(usart->clk);
	if (!pclk)
		return -1;

	pclk_rate = clk_get_rate(pclk);
	clk_put(pclk);

	/* 48 MHz / 115200 = 416 */
	brr_val = 416;
	writel(usart->base_addr + USART_BRR, brr_val);

	return 0;
}

static int stm32_fd0_usart_set_stop_bit(uint8_t stop_bit)
{
	struct usart *usart = get_usart();
	uint8_t val;

	/* set stop bit */
	switch (stop_bit) {
	case 1:
		val = 0;
		break;
	case 2:
		val = 2;
		break;
	default:
		return -1;
	}

	return set_value(usart->base_addr + USART_CR2, val, STOP, 2);
}

static int stm32_fd0_usart_set_parity(void)
{
	struct usart *usart = get_usart();
	return set_bit(usart->base_addr + USART_CR1, PCE);
}

static int stm32_fd0_usart_enable(void)
{
	struct usart *usart = get_usart();
	return set_bit(usart->base_addr + USART_CR1, UE);
}

static int stm32_fd0_usart_disable(void)
{
	struct usart *usart = get_usart();
	return reset_bit(usart->base_addr + USART_CR1, UE);
}

static int stm32_fd0_usart_irq_handler(void *arg)
{
	uint32_t isr;
	struct usart *usart = (struct usart *)arg;
	struct usart_data *usart_data = usart->usart_data;

	isr = readl(usart->base_addr + USART_ISR);

	if (isr & BIT(TXE)) {
		if (usart_data->cur_pos < usart_data->size) {
			uint8_t val = usart_data->data[usart_data->cur_pos++];
			writel(usart->base_addr + USART_TDR, val);
		} else {
			reset_bit(usart->base_addr + USART_CR1, TXEIE);

			/* complete transmission */
			usart_data->complete = 1;
		}
	}

	return 0;
}

int stm32_fd0_usart_start_tx(struct usart_data *usart_data)
{
	struct usart *usart = get_usart();

	if (!usart_data->data || !usart_data->size)
		return -1;

	usart_data->complete = 0;
	usart_data->cur_pos = 0;

	usart->usart_data = usart_data;

	/* enable USART2 transmitter */
	__enable_tx(usart->base_addr);

	/* wait for completion */
	while(usart_data->complete != 1);
	
	/* disable transmitter and USART2 */
	__disable_tx(usart->base_addr);

	usart_data->complete = 0;
	usart_data->cur_pos = 0;

	return 0;
}

int stm32_fd0_usart_start_rx(struct usart_data *usart_data)
{
	return 0;
}

struct usart_ops stm32_fd0_usart_ops = {
	.usart_enable = stm32_fd0_usart_enable,
	.usart_disable = stm32_fd0_usart_disable,
	.set_default = stm32_fd0_usart_set_default,
	.set_word_length = stm32_fd0_usart_set_word_length,
	.set_baud_rate = stm32_fd0_usart_set_baud_rate,
	.set_stop_bit = stm32_fd0_usart_set_stop_bit,
	.set_parity = stm32_fd0_usart_set_parity,
	.start_tx = stm32_fd0_usart_start_tx,
	.start_rx = stm32_fd0_usart_start_rx,
};

int stm32_fd0_usart_init(void)
{
	int i;
	struct clk *gpio_clk;
	struct usart *usart = get_usart();
	struct usart_gpio *usart_gpio;

	/* Enable clocks */
	usart->clk = clk_get(usart->clk_name);
	if (!usart->clk)
		return -1;

	for (i = 0; i < usart->usart_resources.nr_gpio; i++) {
		usart_gpio = usart->usart_resources.gpio + i;

		gpio_clk = clk_get(usart_gpio->clk_name);
		if (!gpio_clk) {
			clk_put(usart->clk);
			return -1;
		}

		/* Set mode (USART2 TX) */
		gpio_mux(usart_gpio->id,
			usart_gpio->pin_num, usart_gpio->mode);

		/* Configure TX GPIO */
		gpio_set_mode(usart_gpio->id, usart_gpio->pin_num, GPIO_MODE_AF);
		gpio_set_type(usart_gpio->id, usart_gpio->pin_num, GPIO_OTYPE_PP);
		gpio_set_speed(usart_gpio->id, usart_gpio->pin_num, GPIO_OSPEED_HIGH);
		gpio_set_pupd(usart_gpio->id, usart_gpio->pin_num, GPIO_PUPD_UP);

		clk_put(gpio_clk);
	}

	atomic_init(&usart->ref_count);
	usart->irq_handler = stm32_fd0_usart_irq_handler;
	irq_request(usart->irq, usart->irq_handler, (void *)usart, 0);

	irq_enable(usart->irq);

	usart_init(&stm32_fd0_usart_ops);

	return 0;
}
