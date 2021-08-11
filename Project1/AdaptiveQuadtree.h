#pragma once

#include <array>
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
		tree_node() : uid(-1), level(0), node_mass(0)
		{
		}

		tree_node(const int uid, const rect<double> bound, const size_t level)
			: uid(uid), level(level), bounding_box(bound), node_mass(0)
		{
		}

		int uid;
		size_t level;

		/// <summary>
		/// I used center point as the position.
		///	Also the entire boarder of the whole universe is between [0..1]
		/// </summary>
		rect<double> bounding_box;

		/// <summary>
		///
		/// </summary>
		std::shared_ptr<body> content;

		/// <summary>
		///	 2 | 3
		/// ---+---
		///	 0 | 1
		/// </summary>
		std::optional<std::array<tree_node*, 4>> children;

		/// <summary>
		///
		/// </summary>
		double node_mass;

		/// <summary>
		///
		/// </summary>
		bool is_leaf() const { return !children.has_value(); }

		/// <summary>
		///
		/// </summary>
		bool is_empty() const { return content == nullptr; }

		void insert_body(const std::shared_ptr<body>& body_ptr);

	private:
		direction determine_quadrant(const std::shared_ptr<body>& body) const;
		void split();
	};

	class quadtree
	{
	public:
		quadtree();
		void allocate_node_for_particle(const std::shared_ptr<body>& body_ptr);
		void compute_center_of_mass();
		std::complex<double> get_gravity_at(const vec2& pos);

	private:
		tree_node root_;
		size_t num_particles_;
	};
}
