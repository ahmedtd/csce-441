
#ifndef COLOR
#define COLOR

#include <ostream>
using std::ostream;

#include <GL/gl.h>

/// A class that represents an RGBA color, that can be implicitly interpreted as
/// an array of four GLdoubles.
class color
{
public:
    /// Component constructor.
    color(double r, double g, double b, double a);
    
    /// Copy constructor.
    color(const color &other);

    /// Extract the red component of this color.
    double r() const;
    
    /// Extract the green component of this color.
    double g() const;

    /// Extract the blue component of this color.
    double b() const;
    
    /// Extract the alpha component of this color.
    double a() const;

    /// Set the red component of this color.
    void r(const double newComp);

    /// Set the green component of this color.
    void g(const double newComp);

    /// Set the blue component of this color.
    void b(const double newComp);

    /// Set the alpha component of this color.
    void a(const double newComp);

    /// Conversion operator that allows this color to be interpreted as a vector
    /// of 4 GLdoubles.
    operator const GLdouble*() const;

private:
    double mComponents [4];
};

/// Stream insertion operator for color objects
ostream& operator<<(ostream &out, const color &rop);

#endif
