#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

/**
 * GPIO data:
 * id				- identifier
 * addr			- base address
 */
struct gpio_data {
	uint32_t id;
	uint32_t addr;
};

/**
 * GPIO data array
 */
struct gpio_data_array {
	struct gpio_data *gpio_data;
	int nr_gpio_data;
};

/**
 * GPIO descriptor
 */
struct gpio_ops {
	int (*set_mode)(uint32_t id, uint8_t pin_num, int mode);
	int (*get_mode)(uint32_t id, uint8_t pin_num);
	int (*set_output_type)(uint32_t id, uint8_t pin_num, int type);
	int (*get_output_type)(uint32_t id, uint8_t pin_num);
	int (*set_output_speed)(uint32_t id, uint8_t pin_num, int speed);
	int (*get_output_speed)(uint32_t id, uint8_t pin_num);
	int (*set_pupd)(uint32_t id, uint8_t pin_num, int pupd);
	int (*get_pupd)(uint32_t id, uint8_t pin_num);
	int (*set_pin_value)(uint32_t id, uint8_t pin_num, uint8_t value);
	int (*get_pin_value)(uint32_t id, uint8_t pin_num);
};

int gpio_init(struct gpio_ops *);
int gpio_set_mode(uint32_t id, uint8_t pin_num, int mode);
int gpio_get_mode(uint32_t id, uint8_t pin_num);
int gpio_set_output_type(uint32_t id, uint8_t pin_num, int type);
int gpio_get_output_type(uint32_t id, uint8_t pin_num);
int gpio_set_output_speed(uint32_t id, uint8_t pin_num, int speed);
int gpio_get_output_speed(uint32_t id, uint8_t pin_num);
int gpio_set_pupd(uint32_t id, uint8_t pin_num, int pupd);
int gpio_get_pupd(uint32_t id, uint8_t pin_num);
int gpio_set_pin_value(uint32_t id, uint8_t pin_num, uint8_t value);
int gpio_get_pin_value(uint32_t id, uint8_t pin_num);

#endif /* _GPIO_H_ */

