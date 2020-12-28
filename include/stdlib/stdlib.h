#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <stdint.h>

#define BSIZE	80

struct buff {
	u8 data[BSIZE];
	size_t size;
};

void memset(void *dst, int pattern, size_t size);
size_t memcpy(void *dst, const void *src, size_t size);
size_t strlen(const char *src);
int strcmp(const char *src, const char *dst);

void printf(const char *fmt, ...);
void sprintf(char *s, const char *fmt, ...);

#endif /* _STDLIB_H_ */
