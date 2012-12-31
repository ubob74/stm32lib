#include <_stdlib.h>

/* TODO */

size_t memcpy(void *dst, const void *src, size_t size)
{
	int i;

	if (!dst || !src)
		return 0;

	for (i = 0; i < size; i++)
		*((uint8_t *)dst + i) = *((uint8_t *)src + i);

	return size;
}

int strlen(const char *src)
{
	int i = 0;

	if (src)
		for (; src[i] != 0x00; i++);

	return i;
}

int strcmp(const char *src, const char *dst)
{
	int i, len;

	if (!src || !dst)
		return -1;

	len = strlen(src);
	if (strlen(dst) != len)
		return -1;

	for (i = 0; i < len; i++)
		if (src[i] != dst[i])
			return -1;

	return 0;
}
