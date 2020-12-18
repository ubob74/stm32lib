#ifndef _CLK_H_
#define _CLK_H_

#include <stddef.h>
#include <stdint.h>
#include <atomic.h>

/* Clock flags */
#define ALWAYS_ON BIT(0)

struct clk {
	const char *name;
	const char *parent;
	atomic_t ref_count;
	uint32_t rate;
	uint32_t enable_reg;
	uint8_t enable_bit;
	uint32_t flags;
	uint32_t (*get_rate)(const char *);
	int (*set_rate)(uint32_t rate);
};

struct clk_array {
	struct clk *clk;
	int nr_clk;
};

struct clk_ops {
	struct clk * (*get)(const char *clk_name);
	int (*put)(struct clk *clk);
	struct clk * (*get_parent)(struct clk *clk);
	int (*enable)(struct clk *clk);
	int (*disable)(struct clk *clk);
	uint32_t (*get_rate)(struct clk *clk);
	int (*set_rate)(struct clk *clk, uint32_t rate);
};

int clk_init(struct clk_ops *);
struct clk * clk_get(const char *);
int clk_put(struct clk *clk);
struct clk * clk_get_parent(struct clk *clk);
int clk_enable(struct clk *clk);
int clk_disable(struct clk *clk);
uint32_t clk_get_rate(struct clk *clk);
int clk_set_rate(struct clk *clk, uint32_t rate);

#endif /* _CLK_H_ */
