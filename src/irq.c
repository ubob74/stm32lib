#include <irq.h>
#include <io.h>

static struct irq_table *table = NULL;

static struct irq * irq_get_entry(int nr)
{
	if (!table)
		return NULL;

	if (nr >= table->nr_irq)
		return NULL;

	return table->irq + nr;
}

int irq_table_init(struct irq_table *irq_table)
{
	if (!irq_table)
		return -1;

	if (table)
		return -1;

	table = irq_table;
	return 0;
}

int irq_request(int nr, int (*handler)(void *), void *arg, int flags)
{
	struct irq *irq = NULL;

	if (!table)
		return -1;

	if (nr >= table->nr_irq)
		return -1;

	irq = table->irq + nr;
	irq->handler = handler;
	irq->arg = arg;
	irq->flags = flags;

	return nr;
}

void irq_enable(int nr)
{
	if (table->irq_enable)
		table->irq_enable(nr);
}

void irq_disable(int nr)
{
	if (table->irq_disable)
		table->irq_disable(nr);
}

void irq_generic_handler(int nr)
{
	struct irq *irq = irq_get_entry(nr);
	if (irq && irq->handler)
		irq->handler(irq->arg);
}
