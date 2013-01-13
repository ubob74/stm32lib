#include <stm32_fd0_exti.h>
#include <io.h>

#define MAX_MASK_BIT_NUM	28

static int __set_mask(uint32_t reg, uint32_t bit_mask, unsigned flag)
{
	uint8_t bit_num;

	for (bit_num = 0; bit_num < MAX_MASK_BIT_NUM; bit_num++) {
		if (bit_mask & (1UL << bit_num)) {
			(flag)
				? set_bit(reg + EXTI_BASE_ADDR, bit_num)
				: reset_bit(reg + EXTI_BASE_ADDR, bit_num);
		}
	}
	return 0;
}

/**
 * Interrupt mask register
 */
int stm32_exti_set_irq_mask(uint32_t bit_mask)
{
	return __set_mask(EXTI_IMR, bit_mask, 1);
}

int stm32_exti_get_irq_mask(uint8_t bit_num)
{
	return test_bit(EXTI_IMR + EXTI_BASE_ADDR, bit_num);
}

int stm32_exti_reset_irq_mask(uint32_t bit_mask)
{
	return __set_mask(EXTI_IMR, bit_mask, 0);
}

/**
 * Event mask register
 */
int stm32_exti_set_event_mask(uint32_t bit_mask)
{
	return __set_mask(EXTI_EMR, bit_mask, 1);
}

int stm32_exti_get_event_mask(uint8_t bit_num)
{
	return test_bit(EXTI_EMR + EXTI_BASE_ADDR, bit_num);
}

int stm32_exti_reset_event_mask(uint32_t bit_mask)
{
	return __set_mask(EXTI_EMR, bit_mask, 0);
}

/**
 * Rising trigger register
 */
int stm32_exti_set_rising_trigger(uint32_t bit_mask)
{
	return __set_mask(EXTI_RTSR, bit_mask, 1);
}

int stm32_exti_get_rising_trigger(uint8_t bit_num)
{
	return test_bit(EXTI_RTSR + EXTI_BASE_ADDR, bit_num);
}

int stm32_exti_reset_rising_trigger(uint32_t bit_mask)
{
	return __set_mask(EXTI_RTSR, bit_mask, 0);
}

/**
 * Falling trigger register
 */
int stm32_exti_set_falling_trigger(uint32_t bit_mask)
{
	return __set_mask(EXTI_FTSR, bit_mask, 1);
}

int stm32_exti_get_falling_trigger(uint8_t bit_num)
{
	return test_bit(EXTI_FTSR + EXTI_BASE_ADDR, bit_num);
}

int stm32_exti_reset_falling_trigger(uint32_t bit_mask)
{
	return __set_mask(EXTI_FTSR, bit_mask, 0);
}

/**
 * Software interrupt event register
 */
int stm32_exti_set_sw_int_event(uint32_t bit_mask)
{
	return __set_mask(EXTI_SWIER, bit_mask, 1);
}

int stm32_exti_get_sw_int_event(uint8_t bit_num)
{
	return test_bit(EXTI_SWIER + EXTI_BASE_ADDR, bit_num);
}

int stm32_exti_reset_sw_int_event(uint32_t bit_mask)
{
	return __set_mask(EXTI_SWIER, bit_mask, 0);
}

/**
 * Pending register
 */
int stm32_exti_get_pending(uint8_t bit_num)
{
	return test_bit(EXTI_PR + EXTI_BASE_ADDR, bit_num);
}

int stm32_exti_reset_pending(uint32_t bit_num)
{
	return set_bit(EXTI_PR + EXTI_BASE_ADDR, bit_num);
}

/**
 * EXTI configuration
 */
/*int stm32_exti_cfg_mode(uint32_t reg, int cfg_num, int mode)
{
	uint8_t offset = (cfg_num % CFG_PER_REG) << 2;
	return set_value(SYSCFG_BASE_ADDR + reg, mode, offset, CFG_WIDTH);
}*/
