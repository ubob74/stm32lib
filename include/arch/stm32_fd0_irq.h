#ifndef _STM32_FD0_IRQ_HANDLER_H_
#define _STM32_FD0_IRQ_HANDLER_H_

#define STM32_FD0_IRQ_NUM 30

int stm32_fd0_irq_table_init(void);

#define IRQ_HANDLER(name, index) \
    void name(void) { \
        irq_generic_handler(index); \
    }

/**
 * Window Watchdog interrupt
 * PVD through EXTI line detection interrupt
 * RTC global interrupt
 * Flash global interrupt
 * RCC global interrupt
 * EXTI_0_1 interrupt handler
 * EXTI 2-3 interrupt handler
 * EXTI 4-15 interrupt handler
 * Touch sensing interrupt
 * DMA channel 1 interrupt
 * DMA channel 2-3 interrupt
 * DMA channel 4-5 interrupt
 * ADC and comparator 1 and 2 interrupts
 * TIM1 Break, update, trigger and commutation interrupt
 * TIM1 Capture Compare interrupt
 * TIM2 global interrupt
 * TIM3 global interrupt
 * TIM6 and DAC underrun interrupt
 * TIM14 global interrupt
 * TIM15 global interrupt
 * TIM16 global interrupt
 * TIM17 global interrupt
 * I2C1 global interrupt
 * I2C2 global interrupt
 * SPI1 global interrupt
 * SPI2 global interrupt
 * USART1 interrupt handler
 * USART2 interrupt handler
 * CEC global interrupt
 */
#define WWDG        0
#define PVD         1
#define RTC         2
#define FLASH       3
#define RCC         4
#define EXTI0_1     5
#define EXTI2_3     6
#define EXTI4_15    7
#define TSC         8
#define DMA_CH1     9
#define DMA_CH2_3   10
#define DMA_CH4_5   11
#define ADC_COMP    12
#define TIM1_BREAK_UPDATE_TRIGGER 13
#define TIM1_CC     14
#define TIM2        15
#define TIM3        16
#define TIM6_DAC    17
#define TIM14       19
#define TIM15       20
#define TIM16       21
#define TIM17       22
#define I2C1        23
#define I2C2        24
#define SPI1        25
#define SPI2        26
#define USART1		27
#define USART2		28
#define CEC         30

#endif /* _STM32_FD0_IRQ_HANDLER_H_ */
