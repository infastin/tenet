#include "adj_list.h"
#include "algorithm.h"
#include "edge.h"

#include <errno.h>
#include <stddef.h>
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

static TrbOptionParserSpec spec = {
	.n_opts = 2,
	.opts = (TrbOptionParserOpt[]){
		{ "kruskal", 'k', FALSE, TRB_OPTION_PARSER_TYPE_BOOL, NULL, TRUE },
		{ "prim", 'p', FALSE, TRB_OPTION_PARSER_TYPE_BOOL, NULL, TRUE },
	}
};

int main(int argc, char *argv[])
{
	TrbOptionParser parser;
	trb_option_parser_init(&parser, &spec, argv, argc);

	char *input = NULL;
	char *output = NULL;

	bool is_prim = FALSE;
	bool is_kruskal = FALSE;

	TrbOptionParserResult parsed;
	while (trb_option_parser_parse(&parser, &parsed)) {
		TrbOptionParserError *err = &parsed.error;
		TrbOptionParserArgument *arg = &parsed.arg;
		TrbOptionParserItem *item = &parsed.item;
		char *error = NULL;

		switch (parsed.shortopt) {
		case '!':
			error = trb_option_parser_error_str(err, NULL);
			fprintf(stderr, "%s\n", error);
			free(error);
			break;
		case '?':
			if (input == NULL) {
				input = arg->value;
			} else if (output == NULL) {
				output = arg->value;
			}
			break;
		case 'k':
			is_kruskal = item->arg.b;
			break;
		case 'p':
			is_prim = item->arg.b;
			break;
		default:
			__builtin_unreachable();
		}
	}

	if (input == NULL) {
		fprintf(stderr, "Expected input filename!\n");
		exit(EXIT_FAILURE);
	}

	if (output == NULL) {
		fprintf(stderr, "Expected output filename!\n");
		exit(EXIT_FAILURE);
	}

	if (!is_kruskal && !is_prim) {
		fprintf(stderr, "Expected algorithm!\n");
		exit(EXIT_FAILURE);
	}

	FILE *ifile = fopen(input, "r");
	FILE *ofile = fopen(output, "w+");

	AdjList *list = read_matrix(ifile);

	f64 cost;
	bool coherent;
	TrbVector *edges;

	if (is_kruskal) {
		edges = kruskal(list, &cost, &coherent);
	} else if (is_prim) {
		edges = prim(list, &cost, &coherent);
	} else {
		__builtin_unreachable();
	}

	printf("%zu %lf %d\n", edges->len, cost, coherent);

	trb_vector_free(edges, trb_free_func_null);
	adj_list_free(list);
	fclose(ofile);
	fclose(ifile);

	return 0;
}
