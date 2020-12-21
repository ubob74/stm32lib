#ifndef _GPIO_H_
#define _GPIO_H_

#include <stddef.h>
#include <stdint.h>

/*
 * GPIO data:
 * id			- identifier
 * addr			- base address
 */
struct gpio_data {
	uint32_t id;
	uint32_t addr;
};

/*
 * GPIO data array
 */
struct gpio_data_array {
	struct gpio_data *gpio_data;
	int nr_gpio_data;
};

/*
 * GPIO operations
 */
struct gpio_ops {
	int (*request)(uint8_t id, uint8_t pin_num);
	int (*free)(uint8_t id, uint8_t pin_num);
	int (*set_mode)(uint8_t id, uint8_t pin_num, int mode);
	int (*get_mode)(uint8_t id, uint8_t pin_num);
	int (*set_type)(uint8_t id, uint8_t pin_num, int type);
	int (*get_type)(uint8_t id, uint8_t pin_num);
	int (*set_speed)(uint8_t id, uint8_t pin_num, int speed);
	int (*get_speed)(uint8_t id, uint8_t pin_num);
	int (*set_pupd)(uint8_t id, uint8_t pin_num, int pupd);
	int (*get_pupd)(uint8_t id, uint8_t pin_num);
	int (*set_pin_value)(uint8_t id, uint8_t pin_num, uint8_t value);
	int (*get_pin_value)(uint8_t id, uint8_t pin_num);
	int (*mux)(uint8_t id, uint8_t pin_num, int mode);
	int (*gpio_to_irq)(uint8_t id, uint8_t pin_num, int exti_num);
};

int gpio_init(struct gpio_ops *);
int gpio_request(uint8_t id, uint8_t pin_num);
int gpio_free(uint8_t id, uint8_t pin_num);
int gpio_set_mode(uint8_t id, uint8_t pin_num, int mode);
int gpio_get_mode(uint8_t id, uint8_t pin_num);
int gpio_set_type(uint8_t id, uint8_t pin_num, int type);
int gpio_get_type(uint8_t id, uint8_t pin_num);
int gpio_set_speed(uint8_t id, uint8_t pin_num, int speed);
int gpio_get_speed(uint8_t id, uint8_t pin_num);
int gpio_set_pupd(uint8_t id, uint8_t pin_num, int pupd);
int gpio_get_pupd(uint8_t id, uint8_t pin_num);
int gpio_set_pin_value(uint8_t id, uint8_t pin_num, uint8_t value);
int gpio_get_pin_value(uint8_t id, uint8_t pin_num);
int gpio_mux(uint8_t id, uint8_t pin_num, int mode);
int gpio_to_irq(uint8_t id, uint8_t pin_num, int exti_num);

#endif /* _GPIO_H_ */
