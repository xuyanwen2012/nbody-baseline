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
	constexpr size_t num_bodies = 10;

	// The main particle table
	std::vector<std::shared_ptr<body>> bodies;

	std::array<vec2, num_bodies> forces_n_squared;
	std::array<vec2, num_bodies> forces_n_log_n;

	// Initialization of positions/masses
	for (size_t i = 0; i < num_bodies; ++i)
	{
		const auto& pos = vec2{ my_rand(), my_rand() };
		const auto& mass = my_rand() * 1.5;

		bodies.push_back(std::make_shared<body>(pos, mass));
	}

	// Do the N squared
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

	// Do the NlogN
	auto qt = adaptive::quadtree();
	qt.allocate_node_for_particle(bodies.at(0));
	return EXIT_SUCCESS;
}