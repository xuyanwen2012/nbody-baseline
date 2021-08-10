#pragma once
#include <array>
#include <complex>
#include <optional>

#include "Rect.h"

namespace adaptive
{
	struct body;
	struct tree_node
	{
		tree_node() : is_leaf(true), level(0), bounding_box() {}
		tree_node(const rect bound, const size_t level)
			: is_leaf(true), level(level), bounding_box(bound) {}

		bool is_leaf;
		size_t level;
		rect bounding_box;
		std::shared_ptr<body> content;
		std::array<std::optional<tree_node*>, 4> children;
	};

	class quadtree
	{
	public:
		quadtree(size_t max_depth);
	private:
		size_t max_depth_;
		tree_node root_;
	};
}
