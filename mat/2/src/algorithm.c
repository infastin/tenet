#include "algorithm.h"

#include "disjoint_set.h"
#include "edge.h"
#include "math.h"

#include <tribble-2.0/trb-hash-table.h>
#include <tribble-2.0/trb-hash.h>
#include <tribble-2.0/trb-heap.h>
#include <tribble-2.0/trb-messages.h>
#include <tribble-2.0/trb-rp-heap.h>
#include <tribble-2.0/trb-slice-utils.h>
#include <tribble-2.0/trb-utils.h>
#include <tribble-2.0/trb-vector.h>

TrbVector *kruskal(AdjList *adj_list, f64 *cost, bool *coherent)
{
	trb_return_val_if_fail(adj_list != NULL, NULL);

	TrbVector *edges = adj_list_edges(adj_list);
	trb_vector_quicksort(edges, trb_cmp_func(edge_cmp));

	TrbVector *output = trb_vector_init(NULL, sizeof(Edge));

	DisjointSet set;
	disjoint_set_init(&set, adj_list->size);

	if (cost != NULL) {
		*cost = 0;
	}

	for (usize i = 0; i < edges->len; ++i) {
		Edge *e = trb_vector_ptr(edges, Edge, i);
		if (disjoint_set_find(&set, e->u) != disjoint_set_find(&set, e->v)) {
			if (cost != NULL) {
				*cost += e->dist;
			}

			trb_vector_push_back(output, e);
			disjoint_set_unite(&set, e->u, e->v);
		}
	}

	if (coherent != NULL) {
		*coherent = output->len == (adj_list->size - 1);
	}

	trb_vector_free(edges, trb_free_func_null);
	disjoint_set_destroy(&set);

	return output;
}

#define TRB_TYPENAME VertexQueue
#define TRB_METHODNAME vertex_queue
#define T usize
#include <tribble-2.0/trb-rp-heap-generic.h>

static i32 vertex_dist_cmp(const usize *a, const usize *b)
{
	f64 *dist = trb_lambda_context(f64);
	return trb_spaceship(dist[*a], dist[*b]);
}

TrbVector *prim(AdjList *adj_list, f64 *cost, bool *coherent)
{
	trb_return_val_if_fail(adj_list != NULL, NULL);

	usize n = adj_list->size;

	TrbVector *output = trb_vector_init(NULL, sizeof(Edge));
	f64 *dist = trb_talloc(f64, n);
	usize *link = trb_talloc(usize, n);

	VertexQueue queue;
	vertex_queue_init(&queue, TRB_RP_HEAP_ORDER_MIN, trb_cmp_func(vertex_dist_cmp, dist));

	TrbHashTableConfig ht_config = trb_hash_table_config(usize, VertexQueueNode *, 0xbeeeee5, trb_hash_func(trb_jhash), trb_cmp_func(trb_usizecmp));
	TrbHashTable ht;
	trb_hash_table_init(&ht, &ht_config);

	trb_array_fill(dist, n, INFINITY);
	trb_array_fill(link, n, -1);

	dist[0] = 0;

	for (usize i = 0; i < n; ++i) {
		VertexQueueNode *vertex = vertex_queue_insert(&queue, i);
		trb_hash_table_insert(&ht, &i, &vertex);
	}

	if (cost != NULL) {
		*cost = 0;
	}

	if (coherent != NULL) {
		*coherent = TRUE;
	}

	VertexQueueNode *v = vertex_queue_extract(&queue);

	while (queue.heap.len != 0) {
		trb_hash_table_remove(&ht, &v->value, NULL);

		AdjListVertex *alv = adj_list_vertex(adj_list, v->value);

		for (usize i = 0; i < alv->edges.len; ++i) {
			AdjListNode *node = adj_list_node(alv, i);

			VertexQueueNode **u = trb_hash_table_fetch(&ht, &node->index, NULL);
			if (u != NULL && node->dist < dist[(*u)->value]) {
				dist[(*u)->value] = node->dist;
				link[(*u)->value] = v->value;
				vertex_queue_restore(&queue, *u);
			}
		}

		free(v);

		v = vertex_queue_extract(&queue);
		if (dist[v->value] == INFINITY) {
			if (coherent != NULL) {
				*coherent = FALSE;
			}
			break;
		}

		trb_vector_push_back(output, trb_get_ptr(Edge, link[v->value], v->value, dist[v->value]));

		if (cost != NULL) {
			*cost += dist[v->value];
		}
	}

	free(v);

	trb_hash_table_destroy(&ht, trb_free_func_null, trb_free_func_null);
	vertex_queue_destroy(&queue, trb_free_func_null);
	free(link);
	free(dist);

	return output;
}
