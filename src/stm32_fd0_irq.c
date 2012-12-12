#include <io.h>
#include <irq.h>
#include <nvic.h>
#include <stm32_fd0_irq.h>
#include <stm32_fd0_exti.h>

static void stm32_fd0_irq_enable(int nr);
static void stm32_fd0_irq_disable(int nr);
static struct irq stm32_fd0_irq[STM32_FD0_IRQ_NUM];

static struct irq_table stm32_fd0_irq_table = {
	.irq = stm32_fd0_irq,
	.nr_irq = STM32_FD0_IRQ_NUM,
	.irq_enable = stm32_fd0_irq_enable,
	.irq_disable = stm32_fd0_irq_disable,
};

static void stm32_fd0_irq_enable(int nr)
{
	nvic_irq_set_enable(nr);
}

static void stm32_fd0_irq_disable(int nr)
{
	nvic_irq_clear_enable(nr);
}

int stm32_fd0_irq_table_init(void)
{
	int i;
	struct irq *irq = NULL;

	for (i = 0; i < stm32_fd0_irq_table.nr_irq; i++) {
		irq = stm32_fd0_irq_table.irq + i;
		irq->handler = NULL;
		irq->arg = NULL;
	}

	return irq_table_init(&stm32_fd0_irq_table);
}

/**
 * EXTI_0_1 interrupt handler
 */
void exti_0_1_irq_handler(void)
{
	int val;

	irq_generic_handler(EXTI0_1);

	/* drop pending bit */
	val = stm32_exti_get_pending(EXTI0);
	if (val)
		stm32_exti_reset_pending(EXTI0);

	val = stm32_exti_get_pending(EXTI1);
	if (val)
		stm32_exti_reset_pending(EXTI1);
}

/**
 * EXTI_2_3 interrupt handler
 */
void exti_2_3_irq_handler(void)
{
	int val;

	irq_generic_handler(EXTI2_3);

	/* drop pending bit */
	val = stm32_exti_get_pending(EXTI2);
	if (val)
		stm32_exti_reset_pending(EXTI2);

	val = stm32_exti_get_pending(EXTI3);
	if (val)
		stm32_exti_reset_pending(EXTI3);
}

/**
 * USART1 interrupt handler
 */
void usart1_irq_handler(void)
{
	irq_generic_handler(USART1);

	/* drop pending bit */
	stm32_exti_reset_pending(USART1);
}

/**
 * USART2 interrupt handler
 */
void usart2_irq_handler(void)
{
	irq_generic_handler(USART2);

	/* drop pending bit */
	stm32_exti_reset_pending(USART2);
}
