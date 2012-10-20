#include <gpio.h>
#include <io.h>

static struct gpio_ops *gpio_ops = NULL;

int gpio_init(struct gpio_ops *arch_gpio_ops)
{
	if (!arch_gpio_ops)
		return -1;

	gpio_ops = arch_gpio_ops;
	return 0;
}

int gpio_set_mode(uint32_t id, uint8_t pin_num, int mode)
{
	return (gpio_ops && gpio_ops->set_mode) ? gpio_ops->set_mode(id, pin_num, mode) : -1;
}

int gpio_get_mode(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_mode) ? gpio_ops->get_mode(id, pin_num) : -1;
}

int gpio_set_output_type(uint32_t id, uint8_t pin_num, int type)
{
	return (gpio_ops && gpio_ops->set_output_type) ? gpio_ops->set_output_type(id, pin_num, type) : -1;
}

int gpio_get_output_type(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_output_type) ? gpio_ops->get_output_type(id, pin_num) : -1;
}

int gpio_set_output_speed(uint32_t id, uint8_t pin_num, int speed)
{
	return (gpio_ops && gpio_ops->set_output_speed) ? gpio_ops->set_output_speed(id, pin_num, speed) : -1;
}

int gpio_get_output_speed(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_output_speed) ? gpio_ops->get_output_speed(id, pin_num) : -1;
}

int gpio_set_pupd(uint32_t id, uint8_t pin_num, int pupd)
{
	return (gpio_ops && gpio_ops->set_pupd) ? gpio_ops->set_pupd(id, pin_num, pupd) : -1;
}

int gpio_get_pupd(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_pupd) ? gpio_ops->get_pupd(id, pin_num) : -1;
}

int gpio_set_pin_value(uint32_t id, uint8_t pin_num, uint8_t value)
{
	return (gpio_ops && gpio_ops->set_pin_value) ? gpio_ops->set_pin_value(id, pin_num, value) : -1;
}

int gpio_get_pin_value(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_pin_value) ? gpio_ops->get_pin_value(id, pin_num) : -1;
}

