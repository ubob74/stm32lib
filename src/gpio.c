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

int gpio_request(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->request) ? gpio_ops->request(id, pin_num) : -1;
}

int gpio_free(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->free) ? gpio_ops->free(id, pin_num) : -1;
}

int gpio_set_mode(uint32_t id, uint8_t pin_num, int mode)
{
	return (gpio_ops && gpio_ops->set_mode) ? gpio_ops->set_mode(id, pin_num, mode) : -1;
}

int gpio_get_mode(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_mode) ? gpio_ops->get_mode(id, pin_num) : -1;
}

int gpio_set_type(uint32_t id, uint8_t pin_num, int type)
{
	return (gpio_ops && gpio_ops->set_type) ? gpio_ops->set_type(id, pin_num, type) : -1;
}

int gpio_get_type(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_type) ? gpio_ops->get_type(id, pin_num) : -1;
}

int gpio_set_speed(uint32_t id, uint8_t pin_num, int speed)
{
	return (gpio_ops && gpio_ops->set_speed) ? gpio_ops->set_speed(id, pin_num, speed) : -1;
}

int gpio_get_speed(uint32_t id, uint8_t pin_num)
{
	return (gpio_ops && gpio_ops->get_speed) ? gpio_ops->get_speed(id, pin_num) : -1;
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

int gpio_mux(uint32_t id, uint8_t pin_num, int mode)
{
	return (gpio_ops && gpio_ops->mux) ? gpio_ops->mux(id, pin_num, mode) : -1;
}

int gpio_to_irq(uint32_t id, uint8_t pin_num, int irq_num)
{
	return (gpio_ops && gpio_ops->gpio_to_irq) ? gpio_ops->gpio_to_irq(id, pin_num, irq_num) : -1;
}
