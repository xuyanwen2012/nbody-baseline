#include "BalancedQuadtree.h"

std::array<tree_node, 4> tree_node::subdivide() const
{
	const auto hw = real(bounding_box.size) / 2.0;
	const auto hh = imag(bounding_box.size) / 2.0;
	const auto cx = real(bounding_box.center);
	const auto cy = imag(bounding_box.center);

	const tree_node ne{ cx + hw, cy + hh, hw, hh };
	const tree_node nw{ cx - hw, cy + hh, hw, hh };
	const tree_node se{ cx + hw, cy - hh, hw, hh };
	const tree_node sw{ cx - hw, cy - hh, hw, hh };

	return { ne, nw, se, sw };
}

balanced_quadtree::balanced_quadtree(const size_t depth) : max_depth(depth)
{
	const auto root = tree_node{ 0.5, 0.5, 1.0, 1.0 };
	nodes_.push_back(root);

	for (size_t i = 0; i < depth; ++i)
	{
	}
}