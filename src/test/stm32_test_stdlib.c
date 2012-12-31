#include <_stdlib.h>

int stm32_test_stdlib(void)
{
	int len, i;
	int data_array_size;
	struct data {
		char *src;
		char *dst;
		int res;
	};
	struct data *tmp;

	struct data data_array[] = {
		{ "abcd", "abcd", 0 },
		{ "abcd", "abcde", -1 },
		{ "abcd", "efgh", -1 },
	};

	data_array_size = sizeof(data_array)/sizeof(data_array[0]);

	len = strlen(data_array[0].src);
	if (len != 4)
		return -1;

	for (i = 0; i < data_array_size; i++) {
		tmp = &data_array[i];
		if (strcmp(tmp->src, tmp->dst) != tmp->res)
			return -1;
	}

	return 0;
}
