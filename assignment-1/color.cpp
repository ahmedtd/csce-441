
#include "color.hpp"

color::color(double r, double g, double b, double a)
{
    mComponents[0] = r;
    mComponents[1] = g;
    mComponents[2] = b;
    mComponents[3] = a;
}

color::color(const color &other)
{
    mComponents[0] = other.r();
    mComponents[1] = other.g();
    mComponents[2] = other.b();
    mComponents[3] = other.a();
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

void color::r(const double newComp)
{
    mComponents[0] = newComp;
}

void color::g(const double newComp)
{
    mComponents[1] = newComp;
}

void color::b(const double newComp)
{
    mComponents[2] = newComp;
}

void color::a(const double newComp)
{
    mComponents[3] = newComp;
}

color::operator const GLdouble*() const
{
    return static_cast<const GLdouble*>(mComponents);
}
