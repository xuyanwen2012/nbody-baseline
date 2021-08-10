#pragma once
#include <complex>

struct rect
{
	rect() = default;
	rect(const double cx, const double cy, const double w, const double h)
		: center(cx, cy), size(w, h) {}

	std::complex<double> center;
	std::complex<double> size;
};
