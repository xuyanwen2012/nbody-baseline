#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <random>
#include <chrono>
#include <complex>

#include "Vec2.h"

using namespace std::chrono;

constexpr double delta_t = 0.00005;

struct body
{
	vec2 position;
	vec2 velocity;
	alignas(16) double mass;
};

constexpr int num_particles = 65536;

std::array<body, num_particles> bodies;

vec2 gravity_func(const vec2 distance)
{
	const double l2 = distance.norm_sqr() + 1e-3;
	return distance * pow(l2, -3.0 / 2.0);
}

vec2 get_raw_gravity_at(const vec2 position)
{
	vec2 acc{};
	std::for_each(bodies.begin(),
		bodies.end(),
		[&](const body& body)
		{
			acc += gravity_func(body.position - position) * body.mass;
		});
	return acc;
}

void sub_step()
{
	std::for_each(std::execution::par_unseq,
		bodies.begin(),
		bodies.end(),
		[&](body& body)
		{
			const auto acceleration = get_raw_gravity_at(body.position);
			body.velocity += acceleration * delta_t;
		});

	std::for_each(std::execution::par_unseq,
		bodies.begin(),
		bodies.end(),
		[&](body& body)
		{
			body.position += body.velocity * delta_t;
		});
}

void initialization()
{
	std::random_device rd;
	std::mt19937 e2(rd());
	const std::uniform_real_distribution<> dist(0, 1);

	std::for_each(std::execution::par_unseq,
		bodies.begin(),
		bodies.end(),
		[&](body& body)
		{
			body.position.x = dist(e2);
			body.position.y = dist(e2);
			body.mass = dist(e2);
		});
}

int main()
{
	const vec2 x1{ 0.25, 0.5 };
	const vec2 x2{ 0.55, 0.75 };
	const std::complex<double> z1(0.25, 0.5);
	const std::complex<double> z2(0.55, 0.75);

	std::cout << log(sqrt((x1 - x2).norm_sqr())) << std::endl;
	std::cout << std::real(log(abs(z1 - z2))) << std::endl;

	return EXIT_SUCCESS;
}