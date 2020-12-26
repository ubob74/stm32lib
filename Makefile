SRCS = main.c io.c atomic.c clk.c gpio.c irq.c main.c usart.c stdlib.c \
	   stm32_fd0_clk.c stm32_fd0_exti.c stm32_fd0_gpio.c stm32_fd0_irq.c \
	   stm32_fd0_usart.c stm32_fd0_rcc.c \
	   exceptions.c nvic.c systick.c

SRCS_TEST = stm32_test_exti.c stm32_test_gpio.c stm32_test_stdlib.c \
			stm32_test_systick.c stm32_test_usart.c

PROJ_NAME=main

# Location of the linker scripts
LDSCRIPT_INC=./ldscripts

###################################################

ARM_PREFIX=arm-linux-gnueabi-
CC=$(ARM_PREFIX)gcc
LD=$(ARM_PREFIX)ld
OBJCOPY=$(ARM_PREFIX)objcopy
OBJDUMP=$(ARM_PREFIX)objdump
SIZE=$(ARM_PREFIX)size

CFLAGS  = -Wall -g -std=c99 -Os 
CFLAGS += -mlittle-endian
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m0
#CFLAGS += -march=armv6-a
#CFLAGS += -marm
#CFLAGS += -m32
CFLAGS += -nostdlib
CFLAGS += -ffunction-sections -fdata-sections -ffreestanding
#CFLAGS += -Wl,--gc-sections
CFLAGS += -Wl,-Map=$(PROJ_NAME).map

###################################################

vpath %.c src src/stdlib src/arch src/cortex src/test 

ROOT=$(shell pwd)

CFLAGS += -I./include -I./include/stdlib -I./include/arch
CFLAGS += -I./include/cortex -I./include/test

SRCS += ${SRCS_TEST}
ASMS += src/arch/stm32_fd0_startup.s # add startup file to build

OBJS += $(SRCS:.c=.o)
OBJS += $(ASMS:.s=.o)

###################################################

.PHONY: lib proj

all: proj

proj: $(PROJ_NAME).elf

%.o: %.c
	${CC} -Wall -Os -g -fno-pic ${CFLAGS} -c -o $@ $^

%.o: %.s
	${CC} -Wall -g -fno-pic ${CFLAGS} -c -o $@ $^

built-in.o: ${OBJS}
	${LD} -r -o $@ $^

$(PROJ_NAME).elf: built-in.o
	$(LD) -T $(LDSCRIPT_INC)/stm32_fd0.ld --no-dynamic-linker -Map $(PROJ_NAME).map $^ -o $@
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -j .isr_vector -j .text -j .data -j .rodata -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	$(OBJDUMP) -St $(PROJ_NAME).elf >$(PROJ_NAME).lst
	$(SIZE) $(PROJ_NAME).elf

clean:
	find ./ -name '*~' | xargs rm -f	
	find ./ -name '*.o' | xargs rm -f
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).hex
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f $(PROJ_NAME).lst

