
#include "geometry.hpp"

vec2::vec2(double x, double y)
:
    mComponents[0](x),
    mComponents[1](y)
{
}

double vec2::x() const
{
    return mComponents[0];
}

double vec2::y() const
{
    return mComponents[1];
}
