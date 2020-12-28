#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <usart.h>

static struct buff buff;

void memset(void *dst, int pattern, size_t size)
{
	int i;

	if (!dst)
		return;

	for (i = 0; i < size; i++)
		*((uint8_t *)dst + i) = (uint8_t)pattern;
}

size_t memcpy(void *dst, const void *src, size_t size)
{
	int i;

	if (!dst || !src)
		return 0;

	for (i = 0; i < size; i++)
		*((uint8_t *)dst + i) = *((uint8_t *)src + i);

	return size;
}

size_t strlen(const char *src)
{
	size_t i = 0;

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

	for (i = 0; i < len; i++) {
		if (src[i] != dst[i])
			return -1;
	}

	if (dst[i] != 0x00)
		return -1;

	return 0;
}

static void putchar(char c)
{
	if (buff.size == BSIZE)
		return;
	buff.data[buff.size++] = c;
}

static void puthexd(u8 digit)
{
	const char table[]="0123456789ABCDEF";
	putchar(table[digit]);
}

static void puthex(u8 byte)
{
	puthexd(byte >> 4);
	puthexd(byte & 0x0F);
}

static void putdec(u32 n)
{
	u32 b[BSIZE];
	u32 i = 0;
	struct qr qr;

	for (;;) {
		udiv(n, 10, &qr);
		b[i++] = qr.r;
		if (!qr.q)
			break;
		n = qr.q;
		if (i >= BSIZE)
			break;
	}

	for (n = i - 1;;n--) {
		puthexd(b[n]);
		if (!n)
			break;
	}
}

static void puthexi(uint32_t dword)
{
	puthex((dword & 0xFF000000) >> 24);
	puthex((dword & 0x00FF0000) >> 16);
	puthex((dword & 0x0000FF00) >> 8);
	puthex((dword & 0x000000FF));
}

static void puts(const char *str)
{
	while (*str)
		putchar(*str++);
}

void dvprintf(const char *fmt, va_list args)
{
	while (*fmt) {
		switch (*fmt) {
		case '%':
			fmt++;
			switch (*fmt) {
			case 's':
				puts(va_arg(args, const char *));
				break;
			case 'c':
				putchar(va_arg(args, u32));
				break;
			case 'd':
				putdec(va_arg(args, u32));
				break;
			case 'x':
				puthex(va_arg(args, u32));
				break;
			case 'X':
				puthexi(va_arg(args, u32));
				break;
			}
			break;
		default:
			putchar(*fmt);
			break;
		}
		fmt++;
	}
}

void printf(const char *fmt, ...)
{
	va_list args;

	memset(buff.data, 0, sizeof(buff.data));
	buff.size = 0;

	va_start(args, fmt);
	dvprintf(fmt, args);
	va_end(args);

	usart_start_tx((const u8 *)buff.data, buff.size);
}

void sprintf(char *s, const char *fmt, ...)
{
	va_list args;

	memset(buff.data, 0, sizeof(buff.data));
	buff.size = 0;

	va_start(args, fmt);
	dvprintf(fmt, args);
	va_end(args);

	memcpy((void *)s, (const void *)buff.data, buff.size);
}
