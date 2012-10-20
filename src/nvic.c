#include <nvic.h>
#include <io.h>

uint32_t nvic_get_cpuid(void)
{
	return __raw_readl(CPUID + SCB_BASE_ADDR);
}

int nvic_irq_set_enable(uint8_t irq_num)
{
	return set_bit(NVIC_ISER + NVIC_BASE_ADDR, irq_num);
}

int nvic_irq_get_enable(uint8_t irq_num)
{
	return test_bit(NVIC_ISER + NVIC_BASE_ADDR, irq_num);
}

int nvic_irq_clear_enable(uint8_t irq_num)
{
	return set_bit(NVIC_ICER + NVIC_BASE_ADDR, irq_num);
}

int nvic_irq_set_pending(uint8_t irq_num)
{
	return set_bit(NVIC_ISPR + NVIC_BASE_ADDR, irq_num);
}

int nvic_irq_get_pending(uint8_t irq_num)
{
	return test_bit(NVIC_ISPR + NVIC_BASE_ADDR, irq_num);
}

int nvic_irq_clear_pending(uint8_t irq_num)
{
	return set_bit(NVIC_ICPR + NVIC_BASE_ADDR, irq_num);
}

int nvic_irq_set_priority(uint8_t irq_num)
{
	/* TODO */
	return 0;
}

int nvic_irq_get_priority(uint8_t irq_num)
{
	/* TODO */
	return 0;
}

