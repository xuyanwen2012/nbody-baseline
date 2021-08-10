#include "AdaptiveQuadtree.h"

#include <iostream>
#include <list>
#include <queue>

void adaptive::tree_node::insert_body(const std::shared_ptr<body>& body_ptr)
{
	if (is_leaf())
	{
		if (content == nullptr)
		{
			content = body_ptr;
			return;
		}

		// more than 1 particle are allocated into this node, need to split,
		// then re-insert the current content to the deeper levels
		split();
		const auto quadrant = static_cast<size_t>(determine_quadrant(content));
		children->at(quadrant)->insert_body(content);

		content.reset();
	}

	const auto new_quadrant = static_cast<size_t>(determine_quadrant(body_ptr));
	children->at(new_quadrant)->insert_body(body_ptr);
}

adaptive::direction adaptive::tree_node::determine_quadrant(const std::shared_ptr<body>& body) const
{
	const auto cx = bounding_box.center.real();
	const auto cy = bounding_box.center.imag();
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

void adaptive::tree_node::split()
{
	const auto hw = bounding_box.size.real() / 2.0;
	const auto hh = bounding_box.size.imag() / 2.0;
	const auto cx = bounding_box.center.real();
	const auto cy = bounding_box.center.imag();

	const auto next_level = level + 1;

	const auto my_uid = uid * 10;

	const auto sw = new tree_node{ my_uid + 0, rect{cx - hw / 2.0, cy - hh / 2.0, hw, hh}, next_level };
	const auto se = new tree_node{ my_uid + 1, rect{cx + hw / 2.0, cy - hh / 2.0, hw, hh}, next_level };
	const auto nw = new tree_node{ my_uid + 2, rect{cx - hw / 2.0, cy + hh / 2.0, hw, hh}, next_level };
	const auto ne = new tree_node{ my_uid + 3, rect{cx + hw / 2.0, cy + hh / 2.0, hw, hh}, next_level };

	children = std::optional<std::array<tree_node*, 4>>{ {sw, se, nw, ne} };
}

adaptive::quadtree::quadtree()
{
	num_particles_ = 0;
	root_ = tree_node(1, rect{ 0.5, 0.5, 1.0, 1.0 }, 0);
}

void adaptive::quadtree::allocate_node_for_particle(const std::shared_ptr<body>& body_ptr)
{
	++num_particles_;
	root_.insert_body(body_ptr);
}

void adaptive::quadtree::compute_center_of_mass()
{
	std::queue<tree_node*> queue;
	std::vector<tree_node*> list;

	queue.push(&root_);
	while (!queue.empty())
	{
		const auto cur = queue.front();
		queue.pop();

		if (!cur->is_leaf())
		{
			for (auto child : cur->children.value())
			{
				queue.push(child);
			}
		}

		list.push_back(cur);
	}

	std::for_each(list.rbegin(), list.rend(),
		[&](tree_node* node)
		{
			// sum the masses
			double sum = 0.0;
			if (node->is_leaf())
			{
				if (node->content != nullptr)
				{
					sum = node->content->mass;
				}
			}
			else
			{
				for (const tree_node* child : node->children.value())
				{
					sum += child->node_mass;
				}
			}

			node->node_mass = sum;

			std::cout << node->uid;
			if (node->content != nullptr)
			{
				std::cout << " - " << node->content->uid;
			}
			std::cout << std::endl;
		});
}