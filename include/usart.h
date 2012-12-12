#ifndef _USART_H_
#define _USART_H_

#include <stdint.h>

struct usart {
	int id;
	uint32_t base_addr;
};

struct usart_array {
	struct usart *usart;
	int nr_usart;
};

/**
 * The data to be written to USART
 */
struct usart_data {
	uint8_t *data;
	int size;
	int cur_pos;
	int complete;
};

/**
 * USART operations
 */
struct usart_ops {
	int (*usart_enable)(int id);
	int (*usart_disable)(int id);
	int (*set_word_length)(int id, uint8_t word_length);
	int (*set_baud_rate)(int id, uint32_t baud_rate);
	int (*set_stop_bit)(int id, uint8_t stop_bit);
	int (*transmitter_enable)(int id);
	int (*transmitter_disable)(int id);
	int (*receiver_enable)(int id);
	int (*receiver_disable)(int id);
};

int usart_init(struct usart_ops *);
int usart_enable(int id);
int usart_disable(int id);
int usart_set_word_length(int id, uint8_t word_length);
int usart_set_baud_rate(int id, uint32_t baud_rate);
int usart_set_stop_bit(int id, uint8_t stop_bit);
int usart_transmitter_enable(int id);
int usart_transmitter_disable(int id);
int usart_receiver_enable(int id);
int usart_receiver_disable(int id);

int usart_start_transmission(int id, struct usart_data *usart_data);

#endif /* _USART_H_ */
