#ifndef _STM32_EXTI_H_
#define _STM32_EXTI_H_

#include <stdint.h>
#include <io.h>

#define EXTI_BASE_ADDR 0x40010400

/* EXTI registers offsets */
#define EXTI_IMR		0x00
#define EXTI_EMR		0x04
#define EXTI_RTSR		0x08
#define EXTI_FTSR		0x0C
#define EXTI_SWIER	0x10
#define EXTI_PR			0x14

/* Input external line bits */
#define EXTI0				0
#define EXTI1				1
#define EXTI2				2
#define EXTI3				3
#define EXTI4				4

int stm32_exti_set_irq_mask(uint32_t bit_mask);
int stm32_exti_get_irq_mask(uint8_t bit_num);
int stm32_exti_reset_irq_mask(uint32_t bit_mask);

int stm32_exti_set_event_mask(uint32_t bit_mask);
int stm32_exti_get_event_mask(uint8_t bit_num);
int stm32_exti_reset_event_mask(uint32_t bit_mask);

int stm32_exti_set_rising_trigger(uint32_t bit_mask);
int stm32_exti_get_rising_trigger(uint8_t bit_num);
int stm32_exti_reset_rising_trigger(uint32_t bit_mask);

int stm32_exti_set_falling_trigger(uint32_t bit_mask);
int stm32_exti_get_falling_trigger(uint8_t bit_num);
int stm32_exti_reset_falling_trigger(uint32_t bit_mask);

int stm32_exti_set_sw_int_event(uint32_t bit_field);
int stm32_exti_get_sw_int_event(uint8_t bit_num);
int stm32_exti_reset_sw_int_event(uint32_t bit_mask);

int stm32_exti_get_pending(uint8_t bit_num);
int stm32_exti_reset_pending(uint32_t bit_num);

#endif /* _STM32_EXTI_H_ */

