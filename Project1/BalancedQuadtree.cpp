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

balanced_quadtree::balanced_quadtree(const size_t depth) : max_depth_(depth)
{
	//const auto root = tree_node{ 0.5, 0.5, 1.0, 1.0 };
	//nodes_.push_back(root);

	for (size_t cur_level = 0; cur_level < depth; ++cur_level)
	{
		const auto num_nodes_level = static_cast<size_t>(pow(4, cur_level));

		for (int i = 0; i < num_nodes_level; ++i)
		{
			const auto node = tree_node{ 0.5, 0.5, 1.0, 1.0 };
			nodes_.push_back(node);
		}
	}
}

std::pair<size_t, size_t> balanced_quadtree::get_nodes_at_level(const size_t level) const
{
	if (level == 0)
	{
		return std::make_pair(0, 1);
	}
	auto [start, num] = get_nodes_at_level(level - 1);

	auto new_num = static_cast<size_t>(pow(4, level));

	return std::make_pair(start + num, new_num);
}