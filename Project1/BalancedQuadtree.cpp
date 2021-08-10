#include "BalancedQuadtree.h"

void balanced::tree_node::subdivide(const size_t max_depth)
{
	is_leaf = false;
	const auto next_level = level + 1;

	if (next_level >= max_depth)
	{
		is_leaf = true;
		return;
	}

	const auto hw = real(bounding_box.size) / 2.0;
	const auto hh = imag(bounding_box.size) / 2.0;
	const auto cx = real(bounding_box.center);
	const auto cy = imag(bounding_box.center);

	const auto sw = new tree_node{};
	sw->bounding_box = rect{ cx - hw / 2.0, cy - hh / 2.0, hw, hh };
	sw->level = next_level;

	const auto se = new tree_node{};
	se->bounding_box = rect{ cx + hw / 2.0, cy - hh / 2.0, hw, hh };
	se->level = next_level;

	const auto nw = new tree_node{};
	nw->bounding_box = rect{ cx - hw / 2.0, cy + hh / 2.0, hw, hh };
	nw->level = next_level;

	const auto ne = new tree_node{};
	ne->bounding_box = rect{ cx + hw / 2.0, cy + hh / 2.0, hw, hh };
	ne->level = next_level;

	children = std::array<tree_node*, 4>{ {sw, se, nw, ne}};

	for (const auto child : children)
	{
		child->subdivide(max_depth);
	}
}

balanced::quadtree::quadtree(const size_t depth) : max_depth_(depth)
{
	root_ = tree_node{};
	root_.bounding_box = { 0.5, 0.5, 1.0, 1.0 };
	root_.level = 0;
	root_.subdivide(max_depth_);
}

void balanced::quadtree::allocate_node_for_particle(const std::shared_ptr<body>& body_ptr)
{
}