#ifndef _MATH_H_
#define _MATH_H_

#include <stdint.h>

struct qr {
	u32 q;
	u32 r;
};

void udiv(u32 n, u32 m, struct qr *);

#endif
