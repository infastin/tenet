#include <stdio.h>

void fcopy(const char *dst, const char *src)
{
	FILE *fdst = fopen(dst, "w+");
	FILE *fsrc = fopen(src, "r");

	char buf[512];
	int n;

	while ((n = fread(buf, 1, 512, fsrc)) > 0) {
		fwrite(buf, 1, n, fdst);
	}

	fclose(fsrc);
	fclose(fdst);
}

int main(int argc, char *argv[])
{
	fcopy(argv[1], argv[2]);
	return 0;
}
