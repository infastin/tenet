#include <stdio.h>
#include <stdlib.h>

extern int *preprocess(const char *needle, int len);
extern void freplace(const char *dst, const char *src, const char *needle, const char *replace);

int main(int argc, char *argv[])
{
	freplace("result.txt", "lorem.txt", "ABCDABD", "123456");

	return 0;
}
