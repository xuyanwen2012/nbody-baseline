#pragma once
#include <array>
#include <complex>
#include <vector>

#include "Rect.h"

struct body;

namespace balanced
{
	struct tree_node
	{
		bool is_leaf;
		size_t level;
		rect<double> bounding_box;
		std::array<tree_node*, 4> children;
		void subdivide(size_t max_depth);
	};

	class quadtree
	{
	public:
		quadtree(size_t depth);

		void allocate_node_for_particle(const std::shared_ptr<body>& body_ptr);

	private:
		tree_node root_;
		size_t max_depth_;
		std::vector<tree_node> nodes_;
	};
}
