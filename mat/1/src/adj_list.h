#ifndef ADJ_LIST_H_1HP8Q3GS
#define ADJ_LIST_H_1HP8Q3GS

#include <tribble-2.0/trb-types.h>
#include <tribble-2.0/trb-vector.h>

typedef struct _AdjListNode AdjListNode;

struct _AdjListNode {
	usize index;
	f64 dist;
};

typedef struct _AdjListVertex AdjListVertex;

struct _AdjListVertex {
	TrbVector edges;
	usize index;
};

typedef struct _AdjList AdjList;

struct _AdjList {
	TrbVector vertices;
	usize size;
};

AdjList *adj_list_init(AdjList *self, usize size);
bool adj_list_add(AdjList *self, usize u, usize v, f64 dist);
void adj_list_destroy(AdjList *self);
void adj_list_free(AdjList *self);

#define adj_list_vertex(self, index) (trb_vector_ptr(&(self)->vertices, AdjListVertex, index))
#define adj_list_node(vertex, index) (trb_vector_ptr(&(vertex)->edges, AdjListNode, index))

#endif /* end of include guard: ADJ_LIST_H_1HP8Q3GS */
