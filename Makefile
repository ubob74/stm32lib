SRCS = main.c io.c atomic.c clk.c gpio.c irq.c main.c usart.c _stdlib.c \
	   stm32_fd0_clk.c stm32_fd0_exti.c stm32_fd0_gpio.c stm32_fd0_irq.c \
	   stm32_fd0_usart.c stm32_fd0_rcc.c \
	   exceptions.c nvic.c systick.c

SRCS_TEST = stm32_test_exti.c stm32_test_gpio.c stm32_test_stdlib.c \
			stm32_test_systick.c stm32_test_usart.c

PROJ_NAME=main

# Location of the linker scripts
LDSCRIPT_INC=./ldscripts

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump
SIZE=arm-none-eabi-size

CFLAGS  = -Wall -g -std=c99 -Os 
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0
CFLAGS += -ffunction-sections -fdata-sections -nostdlib
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(PROJ_NAME).map

###################################################

vpath %.c src src/_stdlib src/arch src/cortex src/test 

ROOT=$(shell pwd)

CFLAGS += -I./include -I./include/_stdlib -I./include/arch
CFLAGS += -I./include/cortex -I./include/test

SRCS += ${SRCS_TEST}
SRCS += src/arch/stm32_fd0_startup.s # add startup file to build

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ -L$(LDSCRIPT_INC) -Tstm32_fd0.ld
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

clean:
	find ./ -name '*~' | xargs rm -f	
	rm -f *.o
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

