
#ifndef COLOR
#define COLOR

#include <ostream>
using std::ostream;

#include <GL/glut.h>

/// A class that represents an RGBA color, that can be implicitly interpreted as
/// an array of four GLfloats.
class color
{
public:
    /// Default constructor -- transparent black
    color();

    /// Component constructor.
    color(float r, float g, float b, float a);
    
    /// Copy constructor.
    color(const color &other);

    /// Extract the red component of this color.
    float r() const;
    
    /// Extract the green component of this color.
    float g() const;

    /// Extract the blue component of this color.
    float b() const;
    
    /// Extract the alpha component of this color.
    float a() const;

    /// Set the red component of this color.
    void r(const float newComp);

    /// Set the green component of this color.
    void g(const float newComp);

    /// Set the blue component of this color.
    void b(const float newComp);

    /// Set the alpha component of this color.
    void a(const float newComp);

    /// Conversion operator that allows this color to be interpreted as a vector
    /// of 4 GLfloats.
    operator const GLfloat*() const;

private:
    float mComponents [4];
};

/// Stream insertion operator for color objects
ostream& operator<<(ostream &out, const color &rop);

#endif
