
#include "color.hpp"

color::color()
{
    mComponents[0] = 0.0;
    mComponents[1] = 0.0;
    mComponents[2] = 0.0;
    mComponents[3] = 0.0;
}

color::color(float r, float g, float b, float a)
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

float color::r() const
{
    return mComponents[0];
}

float color::g() const
{
    return mComponents[1];
}

float color::b() const
{
    return mComponents[2];
}

float color::a() const
{
    return mComponents[3];
}

void color::r(const float newComp)
{
    mComponents[0] = newComp;
}

void color::g(const float newComp)
{
    mComponents[1] = newComp;
}

void color::b(const float newComp)
{
    mComponents[2] = newComp;
}

void color::a(const float newComp)
{
    mComponents[3] = newComp;
}

color::operator const GLfloat*() const
{
    return static_cast<const GLfloat*>(mComponents);
}

color color::white()
{
    return color(1.0,1.0,1.0,1.0);
}

ostream& operator<<(ostream &out, const color &rop)
{
    out << "[r" << rop.r()
        << " g" << rop.g()
        << " b" << rop.b()
        << " a" << rop.a()
        << "]";
    return out;
}
