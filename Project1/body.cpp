#include "Body.h"

vec2 kernel_func(const vec2& i, const vec2& j)
{
	return log(abs(i - j));
}