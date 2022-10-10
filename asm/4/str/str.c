#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *rand_str()
{
	char *result = malloc(32);
	for (int i = 0; i < 32; ++i) {
		result[i] = 32 + rand() % 95;
	}
	return result;
}

void inssort(char **array, int len)
{
	if (len <= 1)
		return;

	for (int i = 1; i < len; ++i) {
		int cur = i;

		for (int j = i - 1;; --j) {
			if (strcmp(array[j], array[cur]) <= 0)
				break;

			char *tmp = array[i];
			array[i] = array[j];
			array[j] = tmp;

			cur = j;

			if (j == 0)
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	char *strings[10];

	for (int i = 0; i < 10; ++i) {
		strings[i] = rand_str();
		printf("%s\n", strings[i]);
	}

	printf("\n");
	inssort(strings, 10);

	for (int i = 0; i < 10; ++i) {
		printf("%s\n", strings[i]);
	}

	return 0;
}
