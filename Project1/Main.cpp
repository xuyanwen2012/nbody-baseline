#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <random>
#include <complex>

#include "AdaptiveQuadtree.h"
#include "Body.h"

double my_rand()
{
	static thread_local std::mt19937 generator;
	const std::uniform_real_distribution distribution(0.0, 1.0);
	return distribution(generator);
}

int main()
{
	constexpr size_t num_bodies = 50;

	// The main particle table
	std::array<std::shared_ptr<body>, num_bodies> bodies;

	std::array<vec2, num_bodies> forces_n_squared;
	std::array<vec2, num_bodies> forces_n_log_n;

	// Initialization of positions/masses
	for (size_t i = 0; i < num_bodies; ++i)
	{
		const auto& pos = vec2{ my_rand(), my_rand() };
		const auto& mass = my_rand() * 1.5;

		bodies[i] = (std::make_shared<body>(i, pos, mass));
	}

	// -------- Do the N squared --------
	for (size_t i = 0; i < num_bodies; ++i)
	{
		forces_n_squared[i] = { 0, 0 };
		for (size_t j = 0; j < num_bodies; ++j)
		{
			if (i == j)
			{
				continue;
			}
			forces_n_squared[i] += bodies[j]->mass * kernel_func(bodies[i]->pos, bodies[j]->pos);
		}
	}

	// -------- Do the NlogN --------
	auto qt = adaptive::quadtree();

	// 1) Construct the Quadtree
	for (const auto& body_ptr : bodies)
	{
		qt.allocate_node_for_particle(body_ptr);
	}

	// 2) Calculate Centers of Mass
	qt.compute_center_of_mass();

	// 3) Estimate N-Body Forces
	for (const auto& a_body : bodies)
	{
		for (const auto& b_body : bodies)
		{
			//qt.get_gravity_at(body_ptr);
		}
	}

	return EXIT_SUCCESS;
}