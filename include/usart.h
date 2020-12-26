#ifndef _USART_H_
#define _USART_H_

#include <stddef.h>
#include <stdint.h>
#include <atomic.h>

struct clk;
struct usart_ops;

/*
 * USART state
 * - 0 - disable
 * - 1 - enable
 */
enum usart_state {
	USART_DISABLE,
	USART_ENABLE,
};

/*
 * The USART data
 */
struct usart_data {
	uint8_t *data;
	int size;
	int cur_pos;
	volatile int complete;
};

struct usart_gpio {
	int id;
	uint8_t pin_num;
	int mode;
	const char *clk_name;
};

struct usart_resources {
	struct usart_gpio *gpio;
	int nr_gpio;
};

struct usart {
	int id;
	uint32_t base_addr;
	int irq;
	enum usart_state state;
	int (*irq_handler)(void *);
	const char *clk_name;
	struct clk *clk;
	struct usart_resources usart_resources;
	struct usart_data *usart_data;
	atomic_t ref_count;
};

struct usart_array {
	struct usart *usart;
	int nr_usart;
};


/*
 * USART operations
 */
struct usart_ops {
	struct usart *usart; /* back reference */
	int (*usart_enable)(int id);
	int (*usart_disable)(int id);
	int (*set_word_length)(int id, uint8_t word_length);
	int (*set_baud_rate)(int id, uint32_t baud_rate);
	int (*set_stop_bit)(int id, uint8_t stop_bit);
	int (*set_parity)(int id);
	int (*start_tx)(int id, struct usart_data *usart_data);
	int (*start_rx)(int id, struct usart_data *usart_data);
};

int usart_init(struct usart_ops *);
int usart_enable(int id);
int usart_disable(int id);
int usart_set_word_length(int id, uint8_t word_length);
int usart_set_baud_rate(int id, uint32_t baud_rate);
int usart_set_stop_bit(int id, uint8_t stop_bit);
int usart_set_parity(int id);
int usart_start_tx(int id, struct usart_data *usart_data);
int usart_start_rx(int id, struct usart_data *usart_data);

#endif /* _USART_H_ */
