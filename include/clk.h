#ifndef _CLK_H_
#define _CLK_H_

#include <stdint.h>

struct stm32_clk;
struct stm32_clk_ops;

struct clk {
	const char *name;
	struct clk *parent;
	uint32_t ref_count;
	uint32_t rate;
	uint32_t enable_reg;
	uint8_t enable_bit;
};

struct clk_array {
	struct clk *clk;
	int nr_clk;
};

struct clk_ops {
	struct clk * (*get)(const char *clk_name);
	int (*put)(struct clk *);
	int (*enable)(struct clk *);
	int (*disable)(struct clk *);
};

int clk_init(struct clk_ops *);
struct clk * clk_get(const char *);
int clk_put(struct clk *);
int clk_enable(struct clk *);
int clk_disable(struct clk *);

#endif /* _CLK_H_ */
