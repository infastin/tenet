#ifndef ALGORITHM_H_XJHLWOSY
#define ALGORITHM_H_XJHLWOSY

#include "adj_list.h"
#include <tribble-2.0/trb-vector.h>

TrbVector *kruskal(AdjList *adj_list, f64 *cost, bool *coherent);

TrbVector *prim(AdjList *adj_list, f64 *cost, bool *coherent);

TrbVector *prim2(AdjList *adj_list, f64 *cost, bool *coherent);

#endif /* end of include guard: ALGORITHM_H_XJHLWOSY */
