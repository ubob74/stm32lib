#include <stm32_fd0_gpio.h>
#include <io.h>
#include <gpio.h>

#define GPIO_SETTINGS(X) { \
		.id			= GPIO_##X, \
		.addr		= GPIO_##X##_BASE_ADDR, \
}

static struct gpio_data stm32_fd0_gpio_data[] = {
	[0] = GPIO_SETTINGS(A),
	[1] = GPIO_SETTINGS(B),
	[2] = GPIO_SETTINGS(C),
	[3] = GPIO_SETTINGS(D),
	[4] = GPIO_SETTINGS(F),
};

static struct gpio_data_array stm32_fd0_gpio_data_array = {
	.gpio_data = stm32_fd0_gpio_data,
	.nr_gpio_data = sizeof(stm32_fd0_gpio_data)/sizeof(stm32_fd0_gpio_data[0]),
};

static struct gpio_data * __get_gpio_data_by_id(uint32_t id)
{
	return (id > GPIO_F) ? NULL : (stm32_fd0_gpio_data_array.gpio_data + id);
}

static int __set_gpio_value(uint32_t id, uint8_t pin_num, int value, uint32_t offset)
{
	struct gpio_data *gpio_data = NULL;
	uint32_t addr;
	int width = 2;

	gpio_data = __get_gpio_data_by_id(id);
	if (!gpio_data)
		return -1;

	addr = gpio_data->addr + offset;

	switch(offset) {
	case GPIO_MODER_OFFSET:
	case GPIO_OSPEEDR_OFFSET:
	case GPIO_PUPDR_OFFSET:
		return set_value(addr, value, pin_num << 1, width);

	default:
		return (value)
			? set_bit(addr, pin_num)
			: reset_bit(addr, pin_num);
	}
}

static int __get_gpio_value(uint32_t id, uint8_t pin_num, uint32_t offset)
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

/**
 * GPIO mode control
 */
static int stm32_fd0_gpio_set_mode(uint32_t id, uint8_t pin_num, int mode)
{
	return __set_gpio_value(id, pin_num, mode, GPIO_MODER_OFFSET);
}

static int stm32_fd0_gpio_get_mode(uint32_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_MODER_OFFSET);
}

/**
 * GPIO output type control
 */
static int stm32_fd0_gpio_set_output_type(uint32_t id, uint8_t pin_num, int type)
{
	return __set_gpio_value(id, pin_num, type, GPIO_OTYPER_OFFSET);
}

static int stm32_fd0_gpio_get_output_type(uint32_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_OTYPER_OFFSET);
}

/**
 * GPIO output speed control
 */
static int stm32_fd0_gpio_set_output_speed(uint32_t id, uint8_t pin_num, int speed)
{
	return __set_gpio_value(id, pin_num, speed, GPIO_OSPEEDR_OFFSET);
}

static int stm32_fd0_gpio_get_output_speed(uint32_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_OSPEEDR_OFFSET);
}

/**
 * GPIO pull-up/down control
 */
static int stm32_fd0_gpio_set_pupd(uint32_t id, uint8_t pin_num, int pupd)
{
	return __set_gpio_value(id, pin_num, pupd, GPIO_PUPDR_OFFSET);
}

static int stm32_fd0_gpio_get_pupd(uint32_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_PUPDR_OFFSET);
}

/**
 * GPIO bit input/output control
 */
static int stm32_fd0_gpio_set_pin_value(uint32_t id, uint8_t pin_num, uint8_t value)
{
	return (value)
		? __set_gpio_value(id, pin_num, 1, GPIO_BSRR_OFFSET)
		: __set_gpio_value(id, pin_num + PIN_NUM_MAX, 1, GPIO_BSRR_OFFSET);
}

static int stm32_fd0_gpio_get_pin_value(uint32_t id, uint8_t pin_num)
{
	return __get_gpio_value(id, pin_num, GPIO_IDR_OFFSET);
}

struct gpio_ops stm32_fd0_gpio_ops = {
	.set_mode = stm32_fd0_gpio_set_mode,
	.get_mode = stm32_fd0_gpio_get_mode,
	.set_output_type = stm32_fd0_gpio_set_output_type,
	.get_output_type = stm32_fd0_gpio_get_output_type,
	.set_output_speed = stm32_fd0_gpio_set_output_speed,
	.get_output_speed = stm32_fd0_gpio_get_output_speed,
	.set_pupd = stm32_fd0_gpio_set_pupd,
	.get_pupd = stm32_fd0_gpio_get_pupd,
	.set_pin_value = stm32_fd0_gpio_set_pin_value,
	.get_pin_value = stm32_fd0_gpio_get_pin_value,
};

int stm32_fd0_gpio_init()
{
	return gpio_init(&stm32_fd0_gpio_ops);
}

