#include <stm32_fd0_gpio.h>
#include <stm32_fd0_exti.h>
#include <io.h>
#include <gpio.h>

#define CFG_PER_REG		4
#define CFG_WIDTH		4

#define GPIO_SETTINGS(X) { \
	.id = GPIO_##X, \
	.addr = GPIO_##X##_BASE_ADDR, \
}

static struct gpio_data stm32_fd0_gpio_data[] = {
	[0] = GPIO_SETTINGS(A),
	[1] = GPIO_SETTINGS(B),
	[2] = GPIO_SETTINGS(C),
	[3] = GPIO_SETTINGS(D),
	[4] = GPIO_SETTINGS(F),
};

static uint32_t stm32_fd0_gpio_bitmap[5];

static struct gpio_data_array stm32_fd0_gpio_data_array = {
	.gpio_data = stm32_fd0_gpio_data,
	.nr_gpio_data = ARRAY_SIZE(stm32_fd0_gpio_data),
};

static struct gpio_data * __get_gpio_data_by_id(uint8_t id)
{
	return (id > GPIO_F) ? NULL : (stm32_fd0_gpio_data_array.gpio_data + id);
}

static int __set_gpio_value(uint8_t id, uint8_t pin_num, int value, uint32_t offset)
{
	struct gpio_data *gpio_data = NULL;
	uint32_t addr;
	int width;

	gpio_data = __get_gpio_data_by_id(id);
	if (!gpio_data)
		return -1;

	addr = gpio_data->addr + offset;

	switch(offset) {
	case GPIO_MODER_OFFSET:
	case GPIO_OSPEEDR_OFFSET:
	case GPIO_PUPDR_OFFSET:
		width = 2;
		return set_value(addr, value, pin_num << 1, width);

	case GPIO_AFRL_OFFSET:
	case GPIO_AFRH_OFFSET:
		width = 4;
		return set_value(addr, value, (pin_num % 8) << 2, width);

	default:
		return (value)
			? set_bit(addr, pin_num)
			: reset_bit(addr, pin_num);
	}
}

static int __get_gpio_value(uint8_t id, uint8_t pin_num, uint32_t offset)
{
	int width;
	struct gpio_data *gpio_data = NULL;
	uint32_t addr;

	gpio_data = __get_gpio_data_by_id(id);
	if (!gpio_data)
		return -1;

	addr = gpio_data->addr + offset;

	switch (offset) {
	case GPIO_MODER_OFFSET:
	case GPIO_OSPEEDR_OFFSET:
	case GPIO_PUPDR_OFFSET:
		width = 2;
		return get_value(addr, pin_num << 1, width);
	default:
		return test_bit(addr, pin_num);
	}
}

/*
 * GPIO access control
 */
static int stm32_fd0_gpio_request(uint8_t id, uint8_t pin_num)
{
	/* TODO */
	return 0;
}

static int stm32_fd0_gpio_free(uint8_t id, uint8_t pin_num)
{
	/* TODO */
	return 0;
}

/*
 * GPIO mode control
 */
static int stm32_fd0_gpio_set_mode(uint8_t id, uint8_t pin_num, int mode)
{
	return __set_gpio_value(id, pin_num, mode, GPIO_MODER_OFFSET);
}

static int stm32_fd0_gpio_get_mode(uint8_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_MODER_OFFSET);
}

/*
 * GPIO output type control
 */
static int stm32_fd0_gpio_set_type(uint8_t id, uint8_t pin_num, int type)
{
	return __set_gpio_value(id, pin_num, type, GPIO_OTYPER_OFFSET);
}

static int stm32_fd0_gpio_get_type(uint8_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_OTYPER_OFFSET);
}

/*
 * GPIO output speed control
 */
static int stm32_fd0_gpio_set_speed(uint8_t id, uint8_t pin_num, int speed)
{
	return __set_gpio_value(id, pin_num, speed, GPIO_OSPEEDR_OFFSET);
}

static int stm32_fd0_gpio_get_speed(uint8_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_OSPEEDR_OFFSET);
}

/*
 * GPIO pull-up/down control
 */
static int stm32_fd0_gpio_set_pupd(uint8_t id, uint8_t pin_num, int pupd)
{
	return __set_gpio_value(id, pin_num, pupd, GPIO_PUPDR_OFFSET);
}

static int stm32_fd0_gpio_get_pupd(uint8_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_PUPDR_OFFSET);
}

/*
 * GPIO muxing (alternate functions)
 */
int stm32_fd0_gpio_mux(uint8_t id, uint8_t pin_num, int mode)
{
	uint32_t offset;

	if (id > GPIO_B)
		return -1;

	offset = (pin_num > 7) ? GPIO_AFRH_OFFSET : GPIO_AFRL_OFFSET;
	return __set_gpio_value(id, pin_num, mode, offset);
}

/*
 * GPIO bit input/output control
 */
static int stm32_fd0_gpio_set_pin_value(uint8_t id, uint8_t pin_num, uint8_t value)
{
	return (value)
		? __set_gpio_value(id, pin_num, 1, GPIO_BSRR_OFFSET)
		: __set_gpio_value(id, pin_num + PIN_NUM_MAX, 1, GPIO_BSRR_OFFSET);
}

static int stm32_fd0_gpio_get_pin_value(uint8_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_IDR_OFFSET);
}

/*
 * GPIO to IRQ
 */
static int stm32_fd0_gpio_to_irq(uint8_t id, uint8_t pin_num, int exti_num)
{
	uint8_t offset, mode;
	uint32_t reg = SYSCFG_BASE_ADDR;
	uint32_t bit_mask = 0;

	if ((pin_num > 15) || (exti_num > 15))
		return -1;

	if (id > GPIO_F)
		return -1;

	mode = EXTI_PA_MODE + id;

	if (pin_num < 4)
		reg += SYSCFG_EXTICR1;
	else if (pin_num < 8)
		reg += SYSCFG_EXTICR2;
	else if (pin_num < 12)
		reg += SYSCFG_EXTICR3;
	else
		reg += SYSCFG_EXTICR4;

	offset = (pin_num % CFG_PER_REG) << 2;

	bit_mask |= BIT(exti_num);
	stm32_exti_set_irq_mask(bit_mask);
	stm32_exti_set_rising_trigger(bit_mask);

	return set_value(reg, mode, offset, CFG_WIDTH);
}

struct gpio_ops stm32_fd0_gpio_ops = {
	.request = stm32_fd0_gpio_request,
	.free = stm32_fd0_gpio_free,
	.set_mode = stm32_fd0_gpio_set_mode,
	.get_mode = stm32_fd0_gpio_get_mode,
	.set_type = stm32_fd0_gpio_set_type,
	.get_type = stm32_fd0_gpio_get_type,
	.set_speed = stm32_fd0_gpio_set_speed,
	.get_speed = stm32_fd0_gpio_get_speed,
	.set_pupd = stm32_fd0_gpio_set_pupd,
	.get_pupd = stm32_fd0_gpio_get_pupd,
	.set_pin_value = stm32_fd0_gpio_set_pin_value,
	.get_pin_value = stm32_fd0_gpio_get_pin_value,
	.mux = stm32_fd0_gpio_mux,
	.gpio_to_irq = stm32_fd0_gpio_to_irq,
};

int stm32_fd0_gpio_init()
{
	int i;
	for (i = 0; i < sizeof(stm32_fd0_gpio_bitmap)/sizeof(stm32_fd0_gpio_bitmap[0]); i++)
		stm32_fd0_gpio_bitmap[i] = 0;

	return gpio_init(&stm32_fd0_gpio_ops);
}
