#ifndef _STM32_FD0_EXTI_H_
#define _STM32_FD0_EXTI_H_

#include <stdint.h>
#include <io.h>

#define EXTI_BASE_ADDR		0x40010400
#define SYSCFG_BASE_ADDR	0x40010000

/* EXTI registers offsets */
#define EXTI_IMR		0x00
#define EXTI_EMR		0x04
#define EXTI_RTSR		0x08
#define EXTI_FTSR		0x0C
#define EXTI_SWIER		0x10
#define EXTI_PR			0x14

/* SYSCFG registers */
#define SYSCFG_CFGR1	0x00
#define SYSCFG_CFGR2	0x18
#define SYSCFG_EXTICR1	0x08
#define SYSCFG_EXTICR2	0x0C
#define SYSCFG_EXTICR3	0x10
#define SYSCFG_EXTICR4	0x14

/* Input external line bits */
#define EXTI0			0
#define EXTI1			1
#define EXTI2			2
#define EXTI3			3
#define EXTI4			4
#define EXTI5			5
#define EXTI6			6
#define EXTI7			7
#define EXTI8			8
#define EXTI9			9

/* EXTI configuration */
#define EXTI_CFG_0		0
#define EXTI_CFG_1		1
#define EXTI_CFG_2		2
#define EXTI_CFG_3		3
#define EXTI_CFG_4		4
#define EXTI_CFG_5		5
#define EXTI_CFG_6		6
#define EXTI_CFG_7		7
#define EXTI_CFG_8		8
#define EXTI_CFG_9		9
#define EXTI_CFG_10		10
#define EXTI_CFG_11		11
#define EXTI_CFG_12		12
#define EXTI_CFG_13		13
#define EXTI_CFG_14		14
#define EXTI_CFG_15		15

/* SYSCFG configurtion mode */
#define EXTI_PA_MODE	0x00
#define EXTI_PB_MODE	0x01
#define EXTI_PC_MODE	0x02
#define EXTI_PD_MODE	0x03
#define EXTI_PE_MODE	0x04
#define EXTI_PF_MODE	0x05

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

int stm32_exti_cfg_mode(uint32_t reg, int cfg_num, int val);

#endif /* _STM32_FD0_EXTI_H_ */
