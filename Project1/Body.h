#pragma once

#include <complex>

using vec2 = std::complex<double>;

struct body
{
	body(const vec2 pos, const double mass) : pos(pos), mass(mass)
	{
	}

	vec2 pos;
	double mass;
};

vec2 kernel_func(const vec2& i, const vec2& j);