#include "BalancedQuadtree.h"

tree_node::~tree_node()
{
	for (const auto child : children)
	{
		delete child;
	}
}

void tree_node::subdivide(const size_t max_depth)
{
	is_leaf = false;
	const auto next_level = level + 1;

	const auto hw = real(bounding_box.size) / 2.0;
	const auto hh = imag(bounding_box.size) / 2.0;
	const auto cx = real(bounding_box.center);
	const auto cy = imag(bounding_box.center);

	const auto sw = new tree_node(cx - hw, cy - hh, hw, hh, next_level);
	const auto se = new tree_node(cx + hw, cy - hh, hw, hh, next_level);
	const auto nw = new tree_node(cx - hw, cy + hh, hw, hh, next_level);
	const auto ne = new tree_node(cx + hw, cy + hh, hw, hh, next_level);

	children = std::array<tree_node*, 4>{ {sw, se, nw, ne } };

	if (next_level >= max_depth)
	{
		is_leaf = true;
		return;
	}

	for (const auto child : children)
	{
		child->subdivide(max_depth);
	}
}

balanced_quadtree::balanced_quadtree(const size_t depth) : max_depth_(depth)
{
	auto root = tree_node{ 0.5, 0.5, 1.0, 1.0, 0 };
	root.subdivide(depth);
}

void balanced_quadtree::allocate_node_for_particle(const std::shared_ptr<body>& body_ptr)
{
}