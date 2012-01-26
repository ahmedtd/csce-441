
#include "color.hpp"

color::color(double r, double g, double b, double a)
:
    mComponents[0](r),
    mComponents[1](g),
    mComponents[2](b),
    mComponents[3](a)
{
    
}

double color::r() const
{
    return mComponents[0];
}

double color::g() const
{
    return mComponents[1];
}

double color::b() const
{
    return mComponents[2];
}

double color::a() const
{
    return mComponents[3];
}
