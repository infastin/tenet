#include "adj_list.h"
#include "distance.h"

#include <tribble-2.0/tribble.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AdjList *read_matrix(FILE *file)
{
	AdjList *list = NULL;
	char *line = NULL;
	usize linesize = 0;

	if (getline(&line, &linesize, file) == -1) {
		trb_msg_error("couldn't read number of vertices: %s", strerror(errno));
		goto failure;
	}

	usize n = strtol(line, NULL, 10);
	list = adj_list_init(NULL, n);

	for (usize i = 0; i < n; ++i) {
		if (getline(&line, &linesize, file) == -1) {
			trb_msg_error("couldn't read matrix line: %s", strerror(errno));
			goto failure;
		}

		char *start = line;
		for (usize j = 0; j < n; ++j) {
			f64 dist = strtold(start, &start);
			if (dist != 0) {
				adj_list_add(list, i, j, dist);
			}
		}
	}

	free(line);
	return list;

failure:
	if (list != NULL) {
		adj_list_free(list);
	}

	free(line);
	return NULL;
}

int main(int argc, char *argv[])
{
	FILE *file = fopen(argv[2], "r");
	AdjList *list = read_matrix(file);
	usize n = list->size;

	if (argv[1][0] == 'd') {
		usize start = strtol(argv[3], NULL, 10);
		f64 *dist = dijkstra(list, start);
		
		FILE *out = fopen(argv[4], "w+");
		for (usize i = 0; i < n; ++i) {
			fprintf(out, "%4.0f ", dist[i]);
		}

		free(dist);
		fclose(out);
	} else if (argv[1][0] == 'b') {
		usize start = strtol(argv[3], NULL, 10);
		FILE *out = fopen(argv[4], "w+");

		bool negative;
		f64 *dist = ford_bellman(list, start, &negative);

		if (negative) {
			fprintf(out, "-");
		} else {
			for (usize i = 0; i < n; ++i) {
				fprintf(out, "%4.0f ", dist[i]);
			}
		}

		free(dist);
		fclose(out);
	} else {
		FILE *out = fopen(argv[3], "w+");
		f64 (*dist)[n][n] =  (f64 (*)[n][n]) floyd_warshall(list);

		for (usize i = 0; i < n; ++i) {
			for (usize j = 0; j < n; ++j) {
				fprintf(out, "%4.0f ", (*dist)[i][j]);
			}

			fprintf(out, "\n");
		}

		fclose(out);
		free(dist);
	}
	
	adj_list_free(list);
	fclose(file);

	return 0;
}
