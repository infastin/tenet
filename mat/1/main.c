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
	if (argc < 3) {
		printf("Expected filename and start index\n");
		exit(EXIT_FAILURE);
	}

	FILE *file = fopen(argv[1], "r");
	AdjList *list = read_matrix(file);
	usize n = list->size;
	usize start = strtol(argv[2], NULL, 10);

	f64 *dist_dijkstra = dijkstra(list, start);

	if (n < 15) {
		printf("Алгоритм Дейкстры:\n");
		for (usize i = 0; i < n; ++i) {
			printf("%4.0f ", dist_dijkstra[i]);
		}
		printf("\n");
	}


	bool negative;
	f64 *fb_dist = ford_bellman(list, start, &negative);

	if (n < 15) {
		printf("Алгоритм Форда-Беллмана:\n");
		if (negative) {
			printf("Negative");
		} else {
			for (usize i = 0; i < n; ++i) {
				printf("%4.0f ", fb_dist[i]);
			}
		}
		printf("\n");
	}


	f64 (*fw_dist)[n][n] =  (f64 (*)[n][n]) floyd_warshall(list);

	if (n < 15) {
		printf("Алгоритм Флойда-Уоршелла:\n");
		for (usize i = 0; i < n; ++i) {
			for (usize j = 0; j < n; ++j) {
				printf("%4.0f ", (*fw_dist)[i][j]);
			}

			printf("\n");
		}
	}

	free(fw_dist);
	free(fb_dist);
	free(dist_dijkstra);
	adj_list_free(list);
	fclose(file);

	return 0;
}
