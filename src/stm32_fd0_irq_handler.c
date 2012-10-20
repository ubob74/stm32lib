#include <io.h>
#include <irq_handler.h>
#include <stm32_fd0_irq_handler.h>

static struct irq_handler stm32_fd0_irq_handler[STM32_FD0_IRQ_HANDLER_NUM];

static struct irq_handler_table stm32_fd0_irq_handler_table = {
	.irq_handler = stm32_fd0_irq_handler,
	.nr_irq_handler = STM32_FD0_IRQ_HANDLER_NUM,
};

int stm32_fd0_irq_handler_table_init(void)
{
	int i;
	struct irq_handler *entry = NULL;

	for (i = 0; i < stm32_fd0_irq_handler_table.nr_irq_handler; i++) {
		entry = stm32_fd0_irq_handler_table.irq_handler + i;
		entry->handler = NULL;
		entry->arg = NULL;
	}

	return irq_handler_table_init(&stm32_fd0_irq_handler_table);
}

