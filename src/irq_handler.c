#include <irq_handler.h>
#include <nvic.h>
#include <io.h>

static struct irq_handler_table *table = NULL;

static struct irq_handler * irq_handler_get_entry(int nr)
{
	if (!table)
		return NULL;

	if (nr >= table->nr_irq_handler)
		return NULL;

	return table->irq_handler + nr;
}

int irq_handler_table_init(struct irq_handler_table *irq_handler_table)
{
	if (!irq_handler_table)
		return -1;

	if (table)
		return -1;

	table = irq_handler_table;
	return 0;
}

int irq_request(int nr, int (*handler)(void *), void *arg, int flags)
{
	struct irq_handler *irq_handler = NULL;

	if (!table)
		return -1;

	if (nr >= table->nr_irq_handler)
		return -1;

	irq_handler = table->irq_handler + nr;
	irq_handler->handler = handler;
	irq_handler->arg = arg;

	/* Config NVIC */
	nvic_irq_set_enable(nr);

	return nr;
}

void irq_generic_handler(int nr)
{
	struct irq_handler *entry = NULL;

	entry = irq_handler_get_entry(nr);
	if (entry && entry->handler)
			entry->handler(entry->arg);
}

