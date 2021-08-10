#pragma once
#include <array>
#include <complex>
#include <vector>

struct rect
{
	std::complex<double> center;
	std::complex<double> size;
};

struct tree_node
{
	tree_node(double x, double y, double w, double h)
	{
		bounding_box = { {x, y}, {w, h} };
	}

	rect bounding_box;

	std::array<tree_node, 4> subdivide() const;
};

class balanced_quadtree
{
public:
	balanced_quadtree(const size_t depth);

private:
	size_t max_depth_;
	std::vector<tree_node> nodes_;

	std::pair<size_t, size_t> get_nodes_at_level(size_t level) const;
};
