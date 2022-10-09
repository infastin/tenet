#include <stdio.h>

void fcopy(const char *dst, const char *src)
{
	FILE *fdst = fopen(dst, "w+");
	FILE *fsrc = fopen(src, "r");

	char buf[512];
	int n;

	while ((n = fread(buf, 1, 512, fsrc)) != 0) {
		fwrite(buf, 1, n, fdst);
	}

	fclose(fsrc);
	fclose(fdst);
}
