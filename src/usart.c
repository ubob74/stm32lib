#include <stm32_fd0_usart.h>
#include <stm32_fd0_irq.h>
#include <usart.h>
#include <io.h>
#include <irq.h>

static struct usart_ops *usart_ops = NULL;

int usart_init(struct usart_ops *arch_usart_ops)
{
	if (!arch_usart_ops)
		return -1;

	usart_ops = arch_usart_ops;
	return 0;
}

int usart_enable(int id)
{
	return (usart_ops && usart_ops->usart_enable)
		? usart_ops->usart_enable(id) : -1;
}

int usart_disable(int id)
{
	return (usart_ops && usart_ops->usart_disable)
		? usart_ops->usart_disable(id) : -1;
}

int usart_set_word_length(int id, uint8_t word_length)
{
	return (usart_ops && usart_ops->set_word_length)
		? usart_ops->set_word_length(id, word_length) : -1;
}

int usart_set_baud_rate(int id, uint32_t baud_rate)
{
	return (usart_ops && usart_ops->set_word_length)
		? usart_ops->set_baud_rate(id, baud_rate) : -1;
}

int usart_set_stop_bit(int id, uint8_t stop_bit)
{
	return (usart_ops && usart_ops->set_stop_bit)
		? usart_ops->set_stop_bit(id, stop_bit) : -1;
}

int usart_transmitter_enable(int id)
{
	return (usart_ops && usart_ops->transmitter_enable)
		? usart_ops->transmitter_enable(id) : -1;
}

int usart_transmitter_disable(int id)
{
	return (usart_ops && usart_ops->transmitter_disable)
		? usart_ops->transmitter_disable(id) : -1;
}

int usart_receiver_enable(int id)
{
	return (usart_ops && usart_ops->receiver_enable)
		? usart_ops->receiver_enable(id) : -1;
}

int usart_receiver_disable(int id)
{
	return (usart_ops && usart_ops->receiver_disable)
		? usart_ops->receiver_disable(id) : -1;
}

int usart2_handler(void *arg)
{
	uint32_t isr_val, cr1_val;
	uint8_t val;
	struct usart_data *usart_data = (struct usart_data *)arg;

	isr_val = __raw_readl(USART2_BASE_ADDR + USART_ISR);

	if (isr_val & BIT(TXE)) {
		if (usart_data->cur_pos < usart_data->size) {
			val = usart_data->data[usart_data->cur_pos];
			__raw_writeb(USART2_BASE_ADDR + USART_TDR, val);
			usart_data->cur_pos++;
			return 0;
		}
	}

	if (isr_val & BIT(TC)) {
		/* clear TXEIE and TCIE flags */
		cr1_val = __raw_readl(USART2_BASE_ADDR + USART_CR1);
		cr1_val &= ~(BIT(TXEIE) | BIT(TCIE));
		__raw_writel(USART2_BASE_ADDR + USART_CR1, cr1_val);

		/* complete transmission */
		usart_data->complete = 1;
	}

	return 0;
}

int usart_start_transmission(int id, struct usart_data *usart_data)
{
	uint32_t cr1_val;

	if (!usart_data ||
		!usart_data->data ||
		!usart_data->size)
		return -1;

	usart_data->complete = 0;
	usart_data->cur_pos = 0;

	/* enable USART2 transmitter */
	usart_transmitter_enable(USART_2);

	cr1_val = __raw_readl(USART2_BASE_ADDR + USART_CR1);
	cr1_val |= BIT(TXEIE) | BIT(TCIE);
	__raw_writel(USART2_BASE_ADDR + USART_CR1, cr1_val);

	irq_request(USART2, usart2_handler, (void *)usart_data, 0);
	irq_enable(USART2);

	/* wait for completion */
	for(;usart_data->complete != 1;);
	irq_disable(USART2);

	/* disable transmitter and USART2 */
	usart_transmitter_disable(USART_2);

	usart_data->complete = 0;
	usart_data->cur_pos = 0;

	return 0;
}
