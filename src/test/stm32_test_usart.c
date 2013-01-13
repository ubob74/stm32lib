#include <_stdlib.h>
#include <io.h>
#include <nvic.h>
#include <irq.h>
#include <gpio.h>
#include <usart.h>
#include <clk.h>
#include <stm32_fd0_gpio.h>
#include <stm32_fd0_exti.h>
#include <stm32_fd0_irq.h>
#include <stm32_fd0_clk.h>
#include <stm32_fd0_usart.h>
#include <stm32_test.h>

int stm32_test_usart(int id)
{
	int ret = -1;
	struct usart_data usart_data;
	uint8_t data[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

	usart_data.data = &data[0];
	usart_data.size = sizeof(data);

	/* set USART parameters */
	usart_set_word_length(id, 8);
	usart_set_baud_rate(id, 9600);
	usart_set_stop_bit(id, 1);
	usart_set_parity(id);

	/* enable USART2 */
	usart_enable(id);

	usart_start_tx(id, &usart_data);

	/* Disable USART2 */
	usart_disable(id);

	ret = 0;
	return ret;
}

