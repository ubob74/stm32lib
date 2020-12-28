#include <usart.h>

static struct usart_ops *usart_ops = NULL;

int usart_init(struct usart_ops *arch_usart_ops)
{
	if (!arch_usart_ops)
		return -1;

	usart_ops = arch_usart_ops;

	return 0;
}

int usart_enable(void)
{
	return (usart_ops && usart_ops->usart_enable)
		? usart_ops->usart_enable() : -1;
}

int usart_disable(void)
{
	return (usart_ops && usart_ops->usart_disable)
		? usart_ops->usart_disable() : -1;
}

int usart_set_default(u8 id)
{
	return (usart_ops && usart_ops->set_default)
		? usart_ops->set_default(id) : -1;
}

int usart_set_word_length(uint8_t word_length)
{
	return (usart_ops && usart_ops->set_word_length)
		? usart_ops->set_word_length(word_length) : -1;
}

int usart_set_baud_rate(uint32_t baud_rate)
{
	return (usart_ops && usart_ops->set_word_length)
		? usart_ops->set_baud_rate(baud_rate) : -1;
}

int usart_set_stop_bit(uint8_t stop_bit)
{
	return (usart_ops && usart_ops->set_stop_bit)
		? usart_ops->set_stop_bit(stop_bit) : -1;
}

int usart_set_parity(void)
{
	return (usart_ops && usart_ops->set_parity)
		? usart_ops->set_parity() : -1;
}

int usart_start_tx(const u8 *data, size_t size)
{
	struct usart_data usart_data = {
		.data = data,
		.size = size,
	};
	return (usart_ops && usart_ops->start_tx)
		? usart_ops->start_tx(&usart_data) : -1;
}

int usart_start_rx(struct usart_data *usart_data)
{
	return (usart_ops && usart_ops->start_rx)
		? usart_ops->start_rx(usart_data) : -1;
}
