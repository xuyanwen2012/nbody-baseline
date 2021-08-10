#include "AdaptiveQuadtree.h"

adaptive::quadtree::quadtree()
{
	root_ = tree_node(rect{ 0.5, 0.5, 1.0, 1.0 }, 0);
}

void adaptive::quadtree::allocate_node_for_particle(std::shared_ptr<body> body_ptr)
{
	auto cur_node = root_;

	while (!cur_node.is_leaf && cur_node.content != nullptr)
	{
		const auto index = static_cast<size_t>(determine_quadrant(&cur_node, body_ptr));

		auto child = cur_node.children.at(index);
		if (child.has_value())
		{
			cur_node = *child.value();
		}
		else
		{
			// should sub divide this node
			//child = new tree_node(rect{})
		}
	}

	//root_.bounding_box.center
}

adaptive::direction adaptive::quadtree::determine_quadrant(const tree_node* node, const std::shared_ptr<body>& body)
{
	const auto cx = node->bounding_box.center.real();
	const auto cy = node->bounding_box.center.imag();
	const auto x = body->pos.real();
	const auto y = body->pos.imag();

	if (x < cx)
	{
		if (y < cy)
		{
			return direction::sw;
		}
		return direction::nw;
	}
	if (y < cy)
	{
		return direction::se;
	}
	return direction::ne;
}

void adaptive::quadtree::split_node(tree_node* node)
{
}