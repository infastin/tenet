#include "adj_list.h"

#include <tribble-2.0/trb-messages.h>

AdjList *adj_list_init(AdjList *self, usize size)
{
	trb_return_val_if_fail(size != 0, NULL);

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(AdjList, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for AdjList");
			return NULL;
		}

		was_allocated = TRUE;
	}

	self->size = size;
	if (!trb_vector_init_sized(&self->vertices, sizeof(AdjListVertex), size)) {
		if (was_allocated) {
			free(self);
		}

		trb_msg_error("couldn't allocate memory for vertices");
		return NULL;
	}

	for (usize i = 0; i < size; ++i) {
		AdjListVertex *vertex = adj_list_vertex(self, i);
		vertex->index = i;
		trb_vector_init(&vertex->edges, sizeof(AdjListNode));
	}

	return self;
}

bool adj_list_add(AdjList *self, usize u, usize v, f64 dist)
{
	trb_return_val_if_fail(self != NULL, NULL);
	trb_return_val_if_fail(u != v, NULL);

	if (self->size <= u) {
		trb_msg_warn("there is no such vector u: %zu", u);
		return FALSE;
	}

	if (self->size <= v) {
		trb_msg_warn("there is no such vector v: %zu", v);
		return FALSE;
	}

	AdjListVertex *vertex = adj_list_vertex(self, u);
	if (!trb_vector_push_back(&vertex->edges, trb_get_ptr(AdjListNode, v, dist))) {
		trb_msg_warn("couldn't add edge u -> v");
		return FALSE;
	}

	return TRUE;
}

void adj_list_destroy(AdjList *self)
{
	trb_return_if_fail(self != NULL);

	for (usize i = 0; i < self->size; ++i) {
		AdjListVertex *vertex = adj_list_vertex(self, i);
		trb_vector_destroy(&vertex->edges, trb_free_func_null);
	}

	trb_vector_destroy(&self->vertices, trb_free_func_null);
}

void adj_list_free(AdjList *self)
{
	trb_return_if_fail(self != NULL);
	adj_list_destroy(self);
	free(self);
}
