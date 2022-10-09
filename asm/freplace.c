#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t *preprocess(const char *pattern, size_t len)
{
	size_t *table = calloc(len + 1, sizeof(size_t));
	table[0] = -1;

	size_t i;
	size_t j;

	for (i = 1, j = 0; i < len; ++i, ++j) {
		if (pattern[i] == pattern[j]) {
			table[i] = table[j];
		} else {
			table[i] = j;
			while (j != -1 && pattern[i] != pattern[j]) {
				j = table[j];
			}
		}
	}

	table[i] = j;
	return table;
}

void freplace(const char *dst, const char *src, const char *needle, const char *replace)
{
	size_t nlen = strlen(needle);
	size_t rlen = strlen(replace);
	size_t *table = preprocess(needle, nlen);

	FILE *fdst = fopen(dst, "w+");
	FILE *fsrc = fopen(src, "r");

	char buf[512];

	size_t n = 512;
	size_t i = 512;
	size_t j = 0;

	while (1) {
		if (i == n) {
			n = fread(buf, 1, 512, fsrc);
			i = 0;
		}

		if (needle[j] == '\0') {
			fwrite(replace, 1, rlen, fdst);
			if (n == 0 || buf[i] == '\0') {
				break;
			} else {
				j = 0;
			}
		} else if (n == 0 || buf[i] == '\0') {
			fwrite(needle, 1, j, fdst);
			break;
		} else if (buf[i] == needle[j]) {
			++i;
			++j;
		} else {
			size_t k = table[j];

			if (k == -1) {
				fwrite(needle, 1, j, fdst);
				fputc(buf[i], fdst);

				j = 0;
				++i;
			} else {
				fwrite(needle, 1, j - k, fdst);
				j = k;
			}
		}
	}

	fclose(fsrc);
	fclose(fdst);
	free(table);
}
