#include "AdaptiveQuadtree.h"

adaptive::quadtree::quadtree(const size_t max_depth) : max_depth_(max_depth)
{
	root_ = tree_node({ 0.5, 0.5, 1.0, 1.0 }, 0);
}