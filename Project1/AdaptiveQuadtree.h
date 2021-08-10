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
		tree_node() : uid(-1), level(0)
		{
		}

		tree_node(const int uid, const rect<double> bound, const size_t level)
			: uid(uid), level(level), bounding_box(bound)
		{
		}

		int uid;
		size_t level;
		rect<double> bounding_box;
		std::shared_ptr<body> content;
		/// <summary>
		///	 2 | 3
		/// ---+---
		///	 0 | 1
		/// </summary>
		std::optional<std::array<tree_node*, 4>> children;

		[[nodiscard]] bool is_leaf() const { return !children.has_value(); }
	};

	class quadtree
	{
	public:
		quadtree();
		void allocate_node_for_particle(const std::shared_ptr<body>& body_ptr);
	private:
		tree_node root_;

		void compute_center_of_mass();

		static direction determine_quadrant(const tree_node* node, const std::shared_ptr<body>& body);
		static void split_node(tree_node* node, const std::shared_ptr<body>& body_ptr);
	};
}
