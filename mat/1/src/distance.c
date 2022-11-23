#include "distance.h"

#include <tribble-2.0/trb-heap.h>
#include <tribble-2.0/trb-messages.h>
#include <tribble-2.0/trb-utils.h>

#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

i32 dijkstra_cmp(const void *a, const void *b)
{
	usize i = *(const usize *) a;
	usize j = *(const usize *) b;

	f64 *dist = trb_lambda_context(f64);

	return trb_f64cmp(&dist[i], &dist[j]);
}

f64 *dijkstra(const AdjList *adj_list, usize start)
{
	usize n = adj_list->size;

	bool *visited = trb_talloc0(bool, n);
	trb_exit_if_fail(visited != NULL);

	f64 *dist = trb_talloc(f64, n);
	trb_exit_if_fail(dist != NULL);

	for (usize i = 0; i < n; ++i) {
		dist[i] = INFINITY;
	}
	dist[start] = 0;

	TrbHeap pqueue;
	trb_heap_init(&pqueue, TRB_HEAP_ORDER_MIN, sizeof(usize), trb_cmp_func(dijkstra_cmp, dist));
	trb_heap_insert(&pqueue, trb_get_ptr(usize, start));

	while (pqueue.vector.len != 0) {
		usize i;
		trb_heap_pop_front(&pqueue, &i);

		if (visited[i]) {
			continue;
		}

		visited[i] = TRUE;

		AdjListVertex *vertex = adj_list_vertex(adj_list, i);
		for (usize j = 0; j < vertex->edges.len; ++j) {
			AdjListNode *node = adj_list_node(vertex, j);
			f64 new_dist = dist[i] + node->dist;

			if (new_dist < dist[node->index]) {
				dist[node->index] = new_dist;
				trb_heap_insert(&pqueue, trb_get_ptr(usize, node->index));
			}
		}
	}

	trb_heap_destroy(&pqueue, trb_free_func_null);
	free(visited);

	return dist;
}

f64 *ford_bellman(const AdjList *adj_list, usize start, bool *negative)
{
	usize n = adj_list->size;

	f64 *dist = trb_talloc(f64, n);
	trb_exit_if_fail(dist != NULL);

	for (usize i = 0; i < n; ++i) {
		dist[i] = INFINITY;
	}
	dist[start] = 0;

	usize k = 0;
	for (bool ok = true; ok && k < n; ++k) {
		ok = false;

		for (usize i = 0; i < n; ++i) {
			if (dist[i] == INFINITY) {
				continue;
			}

			AdjListVertex *vertex = adj_list_vertex(adj_list, i);

			for (usize j = 0; j < vertex->edges.len; ++j) {
				AdjListNode *node = adj_list_node(vertex, j);
				f64 new_dist = dist[i] + node->dist;

				if (new_dist < dist[node->index]) {
					dist[node->index] = new_dist;
					ok = true;
				}
			}
		}
	}

	if (k > n - 1) {
		*negative = true;
	} else {
		*negative = false;
	}

	return dist;
}

f64 *floyd_warshall(const AdjList *adj_list)
{
	usize n = adj_list->size;
	
	f64 (*dist)[n][n] = malloc(sizeof *dist);

	for (usize i = 0; i < n; ++i) {
		for (usize j = 0; j < n; ++j) {
			if (i == j) {
				(*dist)[i][j] = 0;
			} else {
				(*dist)[i][j] = INFINITY;
			}
		}
	}

	for (usize i = 0; i < n; ++i) {
		AdjListVertex *vertex = adj_list_vertex(adj_list, i);

		for (usize j = 0; j < vertex->edges.len; ++j) {
			AdjListNode *node = adj_list_node(vertex, j);
			(*dist)[i][node->index] = node->dist;
		}
	}

	for (usize k = 0; k < n; ++k) {
		for (usize i = 0; i < n; ++i) {
			for (usize j = 0; j < n; ++j) {
				if ((*dist)[i][k] == INFINITY || (*dist)[k][j] == INFINITY) {
					continue;
				}

				usize new_dist = (*dist)[i][k] + (*dist)[k][j];
				if (new_dist < (*dist)[i][j]) {
					(*dist)[i][j] = new_dist;
				}
			}
		}
	}

	return (f64 *) dist;
}
