#ifndef _STM32_FD0_IRQ_HANDLER_H_
#define _STM32_FD0_IRQ_HANDLER_H_

#define STM32_FD0_IRQ_NUM 30

int stm32_fd0_irq_table_init(void);

/* TODO: Interrupt vectors */
#define PVD			1
#define RTC			2
#define FLASH		3
#define RCC			4
#define EXTI0_1		5
#define EXTI2_3		6
#define EXTI4_15	7
#define USART1		27
#define USART2		28

#endif /* _STM32_FD0_IRQ_HANDLER_H_ */
