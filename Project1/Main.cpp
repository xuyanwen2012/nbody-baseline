#include <algorithm>
#include <array>
#include <execution>
#include <iostream>
#include <random>
#include <chrono>

#include "Vec2.h"

using namespace std::chrono;

constexpr double delta_t = 0.00005;

struct body
{
	vec2 position;
	vec2 velocity;
	alignas(16) double mass;
};

constexpr size_t num_particles = 1024;

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
		[&](body& body)
		{
			acc += gravity_func(body.position - position) * body.mass;
		});
	return acc;
}

void sub_step()
{
	std::for_each(std::execution::par,
		bodies.begin(),
		bodies.end(),
		[&](body& body)
		{
			const auto acceleration = get_raw_gravity_at(body.position);
			body.velocity += acceleration * delta_t;
		});

	std::for_each(std::execution::par,
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
	std::uniform_real_distribution<> dist(0, 1);

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

void print_results(const char* const tag,
	const high_resolution_clock::time_point start_time,
	const high_resolution_clock::time_point end_time)
{
	std::cout << tag << ": "
		<< "Time: " << duration_cast<duration<double>>(end_time - start_time).count() << "s"
		<< std::endl;
}

int main()
{
	initialization();
	const auto t1 = high_resolution_clock::now();

	for (int i = 0; i < 100; ++i)
	{
		sub_step();
	}

	const auto t2 = high_resolution_clock::now();

	print_results("par", t1, t2);

	return EXIT_SUCCESS;
}