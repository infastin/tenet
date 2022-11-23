#include "disjoint_set.h"

#include <tribble-2.0/tribble.h>

DisjointSet *disjoint_set_init(DisjointSet *self, usize size)
{
	trb_return_val_if_fail(size != 0, NULL);

	usize allocated = 0;

	if (self == NULL) {
		self = trb_talloc(DisjointSet, 1);

		if (self == NULL) {
			trb_msg_error("couln't allocate memory for disjoint set!");
			return NULL;
		}

		allocated |= 1;
	}

	self->roots = calloc(size, sizeof(usize));
	if (self->roots == NULL) {
		trb_msg_error("couln't allocate memory for roots");
		goto fail;
	} else {
		allocated |= 2;
	}

	self->ranks = calloc(size, sizeof(usize));
	if (self->ranks == NULL) {
		trb_msg_error("couln't allocate memory for roots");
		goto fail;
	} else {
		allocated |= 4;
	}

	self->size = size;

	for (usize i = 0; i < size; ++i) {
		self->roots[i] = i;
		self->ranks[i] = 0;
	}

	return self;

fail:
	if (allocated & 4) {
		free(self->ranks);
	}

	if (allocated & 2) {
		free(self->roots);
	}

	if (allocated & 1) {
		free(self);
	}

	return NULL;
}

usize disjoint_set_find(DisjointSet *self, usize v)
{
	trb_return_val_if_fail(self != NULL, -1);
	trb_return_val_if_fail(self->size > v, -1);

	TrbVector stack;
	trb_vector_init(&stack, sizeof(usize));

	while (self->roots[v] != v) {
		trb_vector_push_back(&stack, &v);
		v = self->roots[v];
	}

	while (stack.len != 0) {
		usize u;
		trb_vector_pop_back(&stack, &u);
		self->roots[u] = v;
	}

	trb_vector_destroy(&stack, trb_free_func_null);

	return v;
}

void disjoint_set_unite(DisjointSet *self, usize u, usize v)
{
	trb_return_if_fail(self != NULL);
	trb_return_if_fail(self->size > v);
	trb_return_if_fail(self->size > u);

	usize u_root = disjoint_set_find(self, u);
	usize v_root = disjoint_set_find(self, v);

	if (self->ranks[u_root] > self->ranks[v_root]) {
		self->roots[v_root] = u_root;
	} else {
		self->roots[u_root] = v_root;

		if (self->ranks[u_root] == self->ranks[v_root]) {
			self->ranks[v_root] = self->ranks[v_root] + 1;
		}
	}
}

void disjoint_set_destroy(DisjointSet *self)
{
	trb_return_if_fail(self != NULL);

	if (self->ranks != NULL) {
		free(self->ranks);
	}

	if (self->roots != NULL) {
		free(self->roots);
	}

	self->ranks = NULL;
	self->roots = NULL;
	self->size = 0;
}

void disjoint_set_free(DisjointSet *self)
{
	trb_return_if_fail(self != NULL);
	disjoint_set_destroy(self);
	free(self);
}
