#include <stdlib.h>
#include <usart.h>
#include <stm32_test.h>

int stm32_test_usart(int id)
{
	const char *data = "ABCDEF\r\nabcdef\r\n\0";
	const char *data1 = "fuck off\r\n";
	const char *str = "test string\n";
	const char *str2 = "test string2\n";

	/* set USART parameters */
	usart_set_word_length(8);
	usart_set_baud_rate(115200);
	usart_set_stop_bit(1);

	/* enable USART2 */
	usart_enable();

	usart_start_tx((const u8 *)data, strlen(data));
	usart_start_tx((const u8 *)data1, strlen(data1));

	/* Disable USART2 */
	//usart_disable(id);

	printf("Test printf: %d %x %X\r\n", 10, 30, 30);
	printf("Test printf #2: %d %d\r\n", 123, 456845);
	printf("%s\r\n", str);
	printf("%s %X %X\r\n", str2, 1000, 0x1000FF);

	return 0;
}
