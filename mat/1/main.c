#include "adj_list.h"
#include "distance.h"

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

static TrbOptionParserSpec spec = {
	.n_opts = 4,
	.opts = (TrbOptionParserOpt[]){
		{ "dijkstra", 'd', FALSE, TRB_OPTION_PARSER_TYPE_BOOL, NULL, TRUE },
		{ "bellman-ford", 'b', FALSE, TRB_OPTION_PARSER_TYPE_BOOL, NULL, TRUE },
		{ "floyd-warshall", 'f', FALSE, TRB_OPTION_PARSER_TYPE_BOOL, NULL, TRUE },
		{ "start", 's', FALSE, TRB_OPTION_PARSER_TYPE_UINT, NULL, TRUE },
	}
};

int main(int argc, char *argv[])
{
	TrbOptionParser parser;
	trb_option_parser_init(&parser, &spec, argv, argc);

	char *input = NULL;
	char *output = NULL;

	bool is_dijkstra = FALSE;
	bool is_bellman = FALSE;
	bool is_floyd = FALSE;
	usize start = 0;

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
		case 'd':
			is_dijkstra = item->arg.b;
			break;
		case 'b':
			is_bellman = item->arg.b;
			break;
		case 'f':
			is_floyd = item->arg.b;
			break;
		case 's':
			start = item->arg.u;
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

	if (!is_dijkstra && !is_bellman && !is_floyd) {
		fprintf(stderr, "Expected algorithm!\n");
		exit(EXIT_FAILURE);
	}

	FILE *ifile = fopen(input, "r");
	FILE *ofile = fopen(output, "w+");

	AdjList *list = read_matrix(ifile);
	usize n = list->size;

	if (is_dijkstra) {
		f64 *dist = dijkstra(list, start);
		for (usize i = 0; i < n; ++i) {
			fprintf(ofile, "%4.0f ", dist[i]);
		}
		free(dist);
	} else if (is_bellman) {
		bool negative;
		f64 *dist = ford_bellman(list, start, &negative);

		if (negative) {
			fprintf(ofile, "-");
		} else {
			for (usize i = 0; i < n; ++i) {
				fprintf(ofile, "%4.0f ", dist[i]);
			}
		}

		free(dist);
	} else if (is_floyd) {
		f64(*dist)[n][n] = (f64(*)[n][n]) floyd_warshall(list);

		for (usize i = 0; i < n; ++i) {
			for (usize j = 0; j < n; ++j) {
				fprintf(ofile, "%4.0f ", (*dist)[i][j]);
			}

			fprintf(ofile, "\n");
		}

		free(dist);
	} else {
		__builtin_unreachable();
	}

	adj_list_free(list);
	fclose(ofile);
	fclose(ifile);

	return 0;
}
