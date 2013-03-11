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
 * RCC interrupt handler
 */
void rcc_irq_handler(void)
{
	irq_generic_handler(RCC);
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
 * EXTI_4-15 interrupt handler
 */
void exti_4_15_irq_handler(void)
{
	int val, i;

	irq_generic_handler(EXTI4_15);

	/* drop pending bit */
	for (i = EXTI4; i <= EXTI15; i++) {
		val = stm32_exti_get_pending(i);
		if (val)
			stm32_exti_reset_pending(i);
	}
}

/**
 * USART1 interrupt handler
 */
void usart1_irq_handler(void)
{
	irq_generic_handler(USART1);
}

/**
 * USART2 interrupt handler
 */
void usart2_irq_handler(void)
{
	irq_generic_handler(USART2);
}

/* TODO */
void wwdg_irq_handler(void)
{
}

void pvd_irq_handler(void)
{
}

void rtc_irq_handler(void)
{
}

void flash_irq_handler(void)
{
}

void tsc_irq_handler(void)
{
}

void dma1_ch_1_irq_handler(void)
{
}

void dma1_ch_2_3_irq_handler(void)
{
}

void dma1_ch_4_5_irq_handler(void)
{
}

void adc1_comp_irq_handler(void)
{
}

void tim1_break_update_trigger_irq_handler(void)
{
}

void tim1_cc_irq_handler(void)
{
}

void tim2_irq_handler(void)
{
}

void tim3_irq_handler(void)
{
}

void tim6_dac_irq_handler(void)
{
}

void tim14_irq_handler(void)
{
}

void tim15_irq_handler(void)
{
}

void tim16_irq_handler(void)
{
}

void tim17_irq_handler(void)
{
}

void i2c1_irq_handler(void)
{
}

void i2c2_irq_handler(void)
{
}

void spi1_irq_handler(void)
{
}

void spi2_irq_handler(void)
{
}

void cec_irq_handler(void)
{
}

