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

typedef struct _VertexNode VertexNode;

struct _VertexNode {
	TrbRPHeapNode entry;
	usize index;
	usize min_link;
	f64 dist;
};

static VertexNode *vertex_node_init(VertexNode *self, usize index, usize min_link, f64 dist)
{
	if (self == NULL) {
		self = trb_talloc(VertexNode, 1);
	}

	trb_rp_heap_node_init(&self->entry);

	self->index = index;
	self->min_link = min_link;
	self->dist = dist;

	return self;
}

static i32 vertex_node_dist_cmp(const VertexNode *a, const VertexNode *b)
{
	return trb_f64cmp(&a->dist, &b->dist);
}

TrbVector *prim(AdjList *adj_list, f64 *cost, bool *coherent)
{
	trb_return_val_if_fail(adj_list != NULL, NULL);

	usize n = adj_list->size;

	TrbVector *output = trb_vector_init(NULL, sizeof(Edge));

	TrbRPHeap queue;
	trb_rp_heap_init(&queue, TRB_RP_HEAP_ORDER_MIN, trb_cmp_func(vertex_node_dist_cmp));

	TrbHashTableConfig ht_config = trb_hash_table_config(usize, VertexNode *, 0xbeeeee5, trb_hash_func(trb_jhash), trb_cmp_func(trb_usizecmp));
	TrbHashTable ht;
	trb_hash_table_init(&ht, &ht_config);

	for (usize i = 0; i < n; ++i) {
		VertexNode *vertex = vertex_node_init(NULL, i, -1, i == 0 ? 0 : INFINITY);
		trb_rp_heap_insert(&queue, &vertex->entry);
		trb_hash_table_insert(&ht, &i, &vertex);
	}

	if (cost != NULL) {
		*cost = 0;
	}

	if (coherent != NULL) {
		*coherent = TRUE;
	}

	VertexNode *v = (VertexNode *) trb_rp_heap_extract(&queue);

	while (1) {
		trb_hash_table_remove(&ht, &v->index, NULL);

		if (queue.len == 0) {
			free(v);
			break;
		}

		AdjListVertex *alv = adj_list_vertex(adj_list, v->index);

		for (usize i = 0; i < alv->edges.len; ++i) {
			AdjListNode *node = adj_list_node(alv, i);

			VertexNode **u = trb_hash_table_fetch(&ht, &node->index, NULL);
			if (u != NULL && node->dist < (*u)->dist) {
				(*u)->dist = node->dist;
				(*u)->min_link = v->index;
				trb_rp_heap_restore(&queue, &(*u)->entry);
			}
		}

		free(v);

		v = (VertexNode *) trb_rp_heap_extract(&queue);
		if (v->dist == INFINITY) {
			if (coherent != NULL) {
				*coherent = FALSE;
			}

			free(v);
			break;
		}

		trb_vector_push_back(output, trb_get_ptr(Edge, v->min_link, v->index, v->dist));

		if (cost != NULL) {
			*cost += v->dist;
		}
	}

	trb_hash_table_destroy(&ht, trb_free_func_null, trb_free_func_null);
	trb_rp_heap_destroy(&queue, trb_free_func(free));

	return output;
}
