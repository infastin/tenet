#ifndef DISTANCE_H_6DNJUZGI
#define DISTANCE_H_6DNJUZGI

#include "adj_list.h"

f64 *dijkstra(const AdjList *adj_list, usize start);

f64 *ford_bellman(const AdjList *adj_list, usize start, bool *negative);

f64 *floyd_warshall(const AdjList *adj_list);

#endif /* end of include guard: DISTANCE_H_6DNJUZGI */
