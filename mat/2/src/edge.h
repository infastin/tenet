#ifndef EDGE_H_S24NZCQY
#define EDGE_H_S24NZCQY

#include <tribble-2.0/trb-types.h>

typedef struct _Edge Edge;

struct _Edge {
	usize u;
	usize v;
	f64 dist;
};

i32 edge_cmp(const Edge *a, const Edge *b);

#endif /* end of include guard: EDGE_H_S24NZCQY */
