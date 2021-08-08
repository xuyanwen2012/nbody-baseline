#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <random>
#include <complex>

using vec2 = std::complex<double>;

double my_rand()
{
	static thread_local std::mt19937 generator;
	const std::uniform_real_distribution distribution(0.0, 1.0);
	return distribution(generator);
}

struct body
{
	body(const vec2 pos, const double mass) : pos(pos), mass(mass)
	{
	}

	vec2 pos;
	double mass;
};

vec2 kernel_func(const vec2& i, const vec2& j)
{
	return log(abs(i - j));
}

int main()
{
	constexpr size_t num_bodies = 10;
	std::vector<std::unique_ptr<body>> bodies;
	std::array<vec2, num_bodies> forces;

	for (size_t i = 0; i < num_bodies; ++i)
	{
		const auto& pos = vec2{ my_rand(), my_rand() };
		const auto& mass = my_rand() * 1.5;

		bodies.push_back(std::make_unique<body>(pos, mass));
	}

	//std::cout << log(abs(z1 - z2)) << std::endl;

	for (size_t i = 0; i < num_bodies; ++i)
	{
		forces[i] = { 0, 0 };
		for (size_t j = 0; j < num_bodies; ++j)
		{
			if (i == j)
			{
				continue;
			}
			forces[i] += bodies[j]->mass * kernel_func(bodies[i]->pos, bodies[j]->pos);
		}
	}

	return EXIT_SUCCESS;
}