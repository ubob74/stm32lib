.syntax unified
.cpu cortex-m0
.thumb

.global irq_vectors
.global reset_handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word _sidata
/* start address for the .data section. defined in linker script */  
.word _sdata
/* end address for the .data section. defined in linker script */
.word _edata
/* start address for the .bss section. defined in linker script */
.word _sbss
/* end address for the .bss section. defined in linker script */
.word _ebss

.equ  BootRAM, 0xF108F85F
/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

.pushsection .text.reset_handler
.global reset_handler
.type reset_handler, %function

reset_handler:

/* Copy the data segment initializers from flash to SRAM */  
    movs r1, #0
    b loop_copy_data_init

copy_data_init:
    ldr r3, =_sidata
    ldr r3, [r3, r1]
    str r3, [r0, r1]
    adds r1, r1, #4
    
loop_copy_data_init:
    ldr r0, =_sdata
    ldr r3, =_edata
    adds r2, r0, r1
    cmp r2, r3
    bcc copy_data_init
    ldr r2, =_sbss
    b zero_bss

/* zero fill the bss segment. */  
fill_zero_bss:
    movs r3, #0
    str r3, [r2]
    adds r2, r2, #4
    
zero_bss:
    ldr r3, = _ebss
    cmp r2, r3
    bcc fill_zero_bss

/* Call the application's entry point.*/
    bl main
    bx lr
.size reset_handler, .-reset_handler
.popsection

/*******************************************************************************
* The minimal vector table for a Cortex M0. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
*******************************************************************************/

.pushsection .isr_vector,"a",%progbits
.type irq_vectors, %object
.size irq_vectors, .-irq_vectors

irq_vectors:
    .word _estack
    .word reset_handler
    .word nmi_handler
    .word hard_fault_handler
    .word mem_manage_handler
    .word bus_fault_handler
    .word usage_fault_handler
    .word 0
    .word 0
    .word 0
    .word 0
    .word svc_handler
    .word debug_mon_handler
    .word 0
    .word pend_sv_handler
    .word systick_handler
    .word wwdg_irq_handler
    .word pvd_irq_handler
    .word rtc_irq_handler
    .word flash_irq_handler
    .word rcc_irq_handler
    .word exti_0_1_irq_handler
    .word exti_2_3_irq_handler
    .word exti_4_15_irq_handler
    .word tsc_irq_handler
    .word dma1_ch_1_irq_handler
    .word dma1_ch_2_3_irq_handler
    .word dma1_ch_4_5_irq_handler
    .word adc1_comp_irq_handler
    .word tim1_break_update_trigger_irq_handler
    .word tim1_cc_irq_handler
    .word tim2_irq_handler
    .word tim3_irq_handler
    .word tim6_dac_irq_handler
    .word 0
    .word tim14_irq_handler
    .word tim15_irq_handler
    .word tim16_irq_handler
    .word tim17_irq_handler
    .word i2c1_irq_handler
    .word i2c2_irq_handler
    .word spi1_irq_handler
    .word spi2_irq_handler
    .word usart1_irq_handler
    .word usart2_irq_handler
    .word 0
    .word cec_irq_handler
    .word 0

.popsection
