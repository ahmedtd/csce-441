
#ifndef COLOR
#define COLOR

#include <GL/gl.h>

class color
{
public:
    color(double r, double g, double b, double a);
    color(const color &other);

    double r() const;
    double g() const;
    double b() const;
    double a() const;

    void r(const double newComp);
    void g(const double newComp);
    void b(const double newComp);
    void a(const double newComp);

    operator const GLdouble*() const;
private:
    double mComponents [4];
};

#endif
