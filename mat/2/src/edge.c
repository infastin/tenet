#include "edge.h"

#include <tribble-2.0/trb-utils.h>

i32 edge_cmp(const Edge *a, const Edge *b)
{
	return trb_f64cmp(&a->dist, &b->dist);
}
