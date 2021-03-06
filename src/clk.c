#include <clk.h>
//#include <io.h>

static struct clk_ops *clk_ops = NULL;

int clk_init(struct clk_ops *arch_clk_ops) {
	if (!arch_clk_ops)
		return -1;

	clk_ops = arch_clk_ops;
	return 0;
}

struct clk * clk_get(const char *name)
{
	return (clk_ops && clk_ops->get) ? clk_ops->get(name) : NULL;
}

int clk_put(struct clk *clk)
{
	return (clk_ops && clk_ops->put) ? clk_ops->put(clk) : 0;
}

struct clk * clk_get_parent(struct clk *clk)
{
	return (clk_ops && clk_ops->get_parent) ? clk_ops->get_parent(clk) : NULL;
}

int clk_enable(struct clk *clk)
{
	return (clk_ops && clk_ops->enable) ? clk_ops->enable(clk) : 0;
}

int clk_disable(struct clk *clk)
{
	return (clk_ops && clk_ops->disable) ? clk_ops->disable(clk) : 0;
}

uint32_t clk_get_rate(struct clk *clk)
{
	return (clk_ops && clk_ops->get_rate) ? clk_ops->get_rate(clk) : 0;
}

int clk_set_rate(struct clk *clk, uint32_t rate)
{
	return (clk_ops && clk_ops->set_rate) ? clk_ops->set_rate(clk, rate) : 0;
}
