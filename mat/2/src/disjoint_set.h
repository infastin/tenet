#ifndef DISJOINT_SET_H_K9N4FF8U
#define DISJOINT_SET_H_K9N4FF8U

#include <tribble-2.0/trb-types.h>

typedef struct _DisjointSet DisjointSet;

struct _DisjointSet {
	usize *roots;
	usize *ranks;
	usize size;
};

DisjointSet *disjoint_set_init(DisjointSet *self, usize size);

usize disjoint_set_find(DisjointSet *self, usize v);

void disjoint_set_unite(DisjointSet *self, usize u, usize v);

void disjoint_set_destroy(DisjointSet *self);

void disjoint_set_free(DisjointSet *self);

#endif /* end of include guard: DISJOINT_SET_H_K9N4FF8U */
