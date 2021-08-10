#pragma once
#include <array>
#include <complex>
#include <optional>

#include "Body.h"
#include "Rect.h"

namespace adaptive
{
	enum class direction { sw = 0, se, nw, ne };

	struct tree_node
	{
		tree_node() : is_leaf(true), level(0) {}

		tree_node(const rect<double> bound, const size_t level)
			: is_leaf(true), level(level), bounding_box(bound) {}

		bool is_leaf;
		size_t level;
		rect<double> bounding_box;
		std::shared_ptr<body> content;
		/// <summary>
		///	 2 | 3
		/// ---+---
		///	 0 | 1
		/// </summary>
		std::array<std::optional<tree_node*>, 4> children;
	};

	class quadtree
	{
	public:
		quadtree();
		void allocate_node_for_particle(std::shared_ptr<body> body_ptr);
	private:
		tree_node root_;

		static direction determine_quadrant(const tree_node* node, const std::shared_ptr<body>& body);
	};
}
