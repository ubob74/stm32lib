//#include <stm32_fd0_usart.h>
//#include <stm32_fd0_irq.h>
#include <usart.h>
//#include <io.h>
//#include <irq.h>

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

int usart_set_parity(int id)
{
	return (usart_ops && usart_ops->set_parity)
		? usart_ops->set_parity(id) : -1;
}

int usart_start_tx(int id, struct usart_data *usart_data)
{
	return (usart_ops && usart_ops->start_tx)
		? usart_ops->start_tx(id, usart_data) : -1;
}

int usart_start_rx(int id, struct usart_data *usart_data)
{
	return (usart_ops && usart_ops->start_rx)
		? usart_ops->start_rx(id, usart_data) : -1;
}
