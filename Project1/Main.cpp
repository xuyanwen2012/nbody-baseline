#include <algorithm>
#include <iostream>
#include <random>
#include <complex>
#include <cstdlib>

#include "AdaptiveQuadtree.h"
#include "Body.h"


double f_rand(const double f_min = 0.0, const double f_max = 1.0)
{
	const double f = static_cast<double>(rand()) / RAND_MAX;
	return f_min + f * (f_max - f_min);
}

void estimate_forces(const size_t num_bodies,
                     std::vector<vec2>& forces_n_log_n,
                     adaptive::quadtree& qt,
                     const std::vector<std::shared_ptr<body>>& bodies)
{
	for (size_t i = 0; i < num_bodies; ++i)
	{
		forces_n_log_n.push_back(qt.compute_force_at(bodies[i]->pos));
	}
}

int main(const int argc, char* argv[])
{
	size_t num_bodies = 1024 * 1024;
	if (argc == 2)
	{
		num_bodies = atoi(argv[1]);
	}
	constexpr bool show_rmse = false;

	// The main particle table
	std::vector<std::shared_ptr<body>> bodies;

	std::vector<vec2> forces_n_squared;
	std::vector<vec2> forces_n_log_n;

	// Initialization of positions/masses
	for (size_t i = 0; i < num_bodies; ++i)
	{
		const auto& pos = vec2{f_rand(), f_rand()};
		const auto& mass = f_rand() * 1.5;

		bodies.push_back(std::make_shared<body>(i, pos, mass));
	}

	// -------- Do the N squared --------
	if (show_rmse)
	{
		for (size_t i = 0; i < num_bodies; ++i)
		{
			forces_n_squared[i] = {0, 0};
			for (size_t j = 0; j < num_bodies; ++j)
			{
				if (i == j)
				{
					continue;
				}

				const auto force = kernel_func(
					bodies[i]->pos,
					bodies[j]->pos
				);

				const auto fm = bodies[j]->mass * force;

				forces_n_squared[i] += fm;
			}
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
	// TODO: put int a function


	// -------- Do Analysis --------

	if (show_rmse)
	{
		vec2 tmp;
		for (size_t i = 0; i < num_bodies; ++i)
		{
			tmp += pow(forces_n_squared[i] - forces_n_log_n[i], 2);
		}

		const auto n = static_cast<double>(num_bodies);
		const auto rsme = sqrt(tmp / n);
		std::cout << "RSME = " << rsme << std::endl;
	}

	return EXIT_SUCCESS;
}
