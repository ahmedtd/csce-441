
#include "ray.hpp"

ray::ray()
:
    point(3),
    slope(3)
{
}

ray::ray(const vec &point_in, const vec &slope_in)
 :
    point(point_in),
    slope(slope_in)
{
}

vec ray::evaluate(const double &paramval) const
{
    return point + paramval*slope;
}
