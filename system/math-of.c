#include <place/system/math-of.h>
#include <stdlib.h>

MATH_OF_CALL vi2d MATH_OF_TYPE vi2d_start(int x, int y)
{
	return ((vi2d){x, y});
}

MATH_OF_CALL vi2d * MATH_OF_TYPE vi2d_set(int x, int y)
{
	vi2d	*set;

	set = malloc(sizeof(vi2d));
	set->x = x;
	set->y = y;
	return (set);
}

MATH_OF_CALL vf2d MATH_OF_TYPE vf2d_start(float x, float y)
{
	return ((vf2d){x, y});
}

MATH_OF_CALL vf2d * MATH_OF_TYPE vf2d_set(float x, float y)
{
	vf2d	*set;

	set = malloc(sizeof(vf2d));
	set->x = x;
	set->y = y;
	return (set);
}

MATH_OF_CALL vf2d MATH_OF_TYPE vf2d_normalize(vf2d f, vf2d s, float x, float y)
{
	return ((vf2d){(f.x * x)/s.x,(f.y * y)/s.y});
}
