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

IRQ_HANDLER(wwdg_irq_handler, WWDG);
IRQ_HANDLER(pvd_irq_handler, PVD);
IRQ_HANDLER(rtc_irq_handler, RTC);
IRQ_HANDLER(flash_irq_handler, FLASH);
IRQ_HANDLER(rcc_irq_handler, RCC);

void exti_0_1_irq_handler(void)
{
	int val;

	irq_generic_handler(EXTI0_1);

	/* drop pending bits */
	val = stm32_exti_get_pending(EXTI0);
	if (val)
		stm32_exti_reset_pending(EXTI0);

	val = stm32_exti_get_pending(EXTI1);
	if (val)
		stm32_exti_reset_pending(EXTI1);
}

void exti_2_3_irq_handler(void)
{
	int val;

	irq_generic_handler(EXTI2_3);

	/* drop pending bits */
	val = stm32_exti_get_pending(EXTI2);
	if (val)
		stm32_exti_reset_pending(EXTI2);

	val = stm32_exti_get_pending(EXTI3);
	if (val)
		stm32_exti_reset_pending(EXTI3);
}

void exti_4_15_irq_handler(void)
{
	int val, i;

	irq_generic_handler(EXTI4_15);

	/* drop pending bits */
	for (i = EXTI4; i <= EXTI15; i++) {
		val = stm32_exti_get_pending(i);
		if (val)
			stm32_exti_reset_pending(i);
	}
}

IRQ_HANDLER(tsc_irq_handler, TSC);
IRQ_HANDLER(dma1_ch_1_irq_handler, DMA_CH1);
IRQ_HANDLER(dma1_ch_2_3_irq_handler, DMA_CH2_3);
IRQ_HANDLER(dma1_ch_4_5_irq_handler, DMA_CH4_5);
IRQ_HANDLER(adc1_comp_irq_handler, ADC_COMP);
IRQ_HANDLER(tim1_break_update_trigger_irq_handler, TIM1_BREAK_UPDATE_TRIGGER);
IRQ_HANDLER(tim1_cc_irq_handler, TIM1_CC);
IRQ_HANDLER(tim2_irq_handler, TIM2);
IRQ_HANDLER(tim3_irq_handler, TIM3);
IRQ_HANDLER(tim6_dac_irq_handler, TIM6_DAC);
IRQ_HANDLER(tim14_irq_handler, TIM14);
IRQ_HANDLER(tim15_irq_handler, TIM15);
IRQ_HANDLER(tim16_irq_handler, TIM16);
IRQ_HANDLER(tim17_irq_handler, TIM17);
IRQ_HANDLER(i2c1_irq_handler, I2C1);
IRQ_HANDLER(i2c2_irq_handler, I2C2);
IRQ_HANDLER(spi1_irq_handler, SPI1);
IRQ_HANDLER(spi2_irq_handler, SPI2);
IRQ_HANDLER(usart1_irq_handler, USART1);
IRQ_HANDLER(usart2_irq_handler, USART2);
IRQ_HANDLER(cec_irq_handler, CEC);

