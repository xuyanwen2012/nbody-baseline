#pragma once
#include <array>
#include <complex>
#include <vector>

struct body;

struct rect
{
	std::complex<double> center;
	std::complex<double> size;
};

struct tree_node
{
	bool is_leaf;
	size_t level;
	rect bounding_box;
	std::array<tree_node*, 4> children;

	void subdivide(size_t max_depth);
};

class balanced_quadtree
{
public:
	balanced_quadtree(size_t depth);

	void allocate_node_for_particle(const std::shared_ptr<body>& body_ptr);

private:
	tree_node root_;
	size_t max_depth_;
	std::vector<tree_node> nodes_;
};
