#include <stdio.h>

void map(int *buf, int size, int (*funPtr)(int))
{
	for (int i = 0; i < size; ++i) {
		buf[i] = funPtr(buf[i]);
	}
}
