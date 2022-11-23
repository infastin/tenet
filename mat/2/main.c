#include "adj_list.h"
#include "algorithm.h"
#include "edge.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tribble-2.0/tribble.h>

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
	char *filename = "/home/infastin/Downloads/SST/test1.txt";
	if (argc == 2) {
		filename = argv[1];
	}

	FILE *file = fopen(filename, "r");
	AdjList *list = read_matrix(file);

	f64 cost;
	bool coherent;

	TrbVector *edges_kr = kruskal(list, &cost, &coherent);
	printf("Алгоритм Краскала:\n");
	printf("%zu %lf %d\n", edges_kr->len, cost, coherent);

	TrbVector *edges_pr = prim(list, &cost, &coherent);
	printf("Алгоритм Прима:\n");
	printf("%zu %lf %d\n", edges_pr->len, cost, coherent);

	trb_vector_free(edges_pr, trb_free_func_null);
	trb_vector_free(edges_kr, trb_free_func_null);
	adj_list_free(list);
	fclose(file);

	return 0;
}
