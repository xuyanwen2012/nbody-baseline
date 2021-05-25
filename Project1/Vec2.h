#pragma once

struct vec2
{
	alignas(16) double x;
	alignas(16) double y;

	[[nodiscard]] double norm_sqr() const
	{
		return x * x + y * y;
	}

	vec2& operator+=(const double v)
	{
		x += v;
		y += v;
		return *this;
	}

	vec2& operator+=(const vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	vec2 operator+(const double v) const
	{
		return { x + v, y + v };
	}

	vec2 operator*(const double v) const
	{
		return { x * v, y * v };
	}

	vec2 operator-(const vec2& rhs) const
	{
		return { x - rhs.x, y - rhs.y };
	}
};
