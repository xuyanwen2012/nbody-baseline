#include "AdaptiveQuadtree.h"

#include <iostream>
#include <queue>

adaptive::quadtree::quadtree()
{
	num_particles_ = 0;
	root_ = tree_node(0, rect{ 0.5, 0.5, 1.0, 1.0 }, 0);
}

void adaptive::quadtree::allocate_node_for_particle(const std::shared_ptr<body>& body_ptr)
{
	++num_particles_;
	tree_node* cur_node = &root_;

	while (!cur_node->is_leaf())
	{
		const auto quadrant = static_cast<size_t>(determine_quadrant(cur_node, body_ptr));

		const auto child = cur_node->children->at(quadrant);
		cur_node = child;
	}

	if (cur_node->content == nullptr)
	{
		cur_node->content = body_ptr;
	}
	else
	{
		split_node(cur_node, cur_node->content);
		cur_node->content.reset();
	}

	//root_.bounding_box.center
}

void adaptive::quadtree::compute_center_of_mass()
{
	std::queue<tree_node*> queue;

	queue.push(&root_);
	while (!queue.empty())
	{
		auto cur = queue.front();
		queue.pop();

		if (!cur->is_leaf())
		{
			for (auto child : cur->children.value())
			{
				queue.push(child);
			}
		}

		std::cout << cur->uid;
		if (cur->content != nullptr)
		{
			std::cout << " - " << cur->content->uid;
		}
		std::cout << std::endl;
	}
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

void adaptive::quadtree::split_node(tree_node* node, const std::shared_ptr<body>& body_ptr)
{
	const auto hw = node->bounding_box.size.real() / 2.0;
	const auto hh = node->bounding_box.size.imag() / 2.0;
	const auto cx = node->bounding_box.center.real();
	const auto cy = node->bounding_box.center.imag();

	const auto next_level = node->level + 1;

	const auto my_uid = node->uid * 10;

	const auto sw = new tree_node{ my_uid + 0, rect{cx - hw / 2.0, cy - hh / 2.0, hw, hh}, next_level };
	const auto se = new tree_node{ my_uid + 1, rect{cx + hw / 2.0, cy - hh / 2.0, hw, hh}, next_level };
	const auto nw = new tree_node{ my_uid + 2, rect{cx - hw / 2.0, cy + hh / 2.0, hw, hh}, next_level };
	const auto ne = new tree_node{ my_uid + 3, rect{cx + hw / 2.0, cy + hh / 2.0, hw, hh}, next_level };

	node->children = std::optional<std::array<tree_node*, 4>>{ {sw, se, nw, ne} };

	const auto new_quadrant = static_cast<size_t>(determine_quadrant(node, body_ptr));
	node->children->at(new_quadrant)->content = body_ptr;
}