#include <stdio.h>
#include <stdlib.h>

extern int *preprocess(const char *needle, int len);

int main(int argc, char *argv[])
{
	const char *pattern = "ABCDABD";
	int *table = preprocess(pattern, 7);

	for (int i = 0; i < 8; ++i) {
		printf("%c: %d\n", pattern[i], table[i]);
	}

	free(table);

	return 0;
}
