#ifndef _IRQ_HANDLER_H_
#define _IRQ_HANDLER_H_

#include <stddef.h>
#include <stdint.h>

struct irq {
	int (*handler)(void *);
	void *arg;
	int flags;
};

struct irq_table {
	struct irq *irq;
	int nr_irq;
	void (*irq_enable)(int);
	void (*irq_disable)(int);
};

int irq_table_init(struct irq_table *);
int irq_request(int nr, int (*handler)(void *), void *arg, int flags);
void irq_enable(int nr);
void irq_disable(int nr);
void irq_generic_handler(int nr);

#endif /* _IRQ_HANDLER_H_ */
