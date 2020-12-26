#include <stdlib.h>
#include <usart.h>
#include <stm32_test.h>

int stm32_test_usart(int id)
{
	struct usart_data usart_data;
	//uint8_t data[] = {'A', 'B', 'C', 'D', 'E', 'F', '\r', '\n', 'a', 'b', 'c', 'd', 'e', 'f', '\r', '\n' };
	char *data = "ABCDEF\r\nabcdef\r\n\0";

	usart_data.data = (uint8_t *)data;
	usart_data.size = strlen(data);

	/* set USART parameters */
	usart_set_word_length(id, 8);
	usart_set_baud_rate(id, 115200);
	usart_set_stop_bit(id, 1);

	/* enable USART2 */
	usart_enable(id);

	usart_start_tx(id, &usart_data);

	/* Disable USART2 */
	//usart_disable(id);

	return 0;
}
