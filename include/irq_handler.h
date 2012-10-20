#ifndef _IRQ_HANDLER_H_
#define _IRQ_HANDLER_H_

#include <stdint.h>

struct irq_handler {
	int (*handler)(void *);
	void *arg;
};

struct irq_handler_table {
	struct irq_handler *irq_handler;
	int nr_irq_handler;
};

int irq_handler_table_init(struct irq_handler_table *);
int irq_request(int nr, int (*handler)(void *), void *arg, int flags);
void irq_generic_handler(int nr);

#endif /* _IRQ_HANDLER_H_ */

