
#ifndef BRUSH
#define BRUSH

#include "color.hpp"
#include "geometry.hpp"

/// An enum that represents the type of a brush.  It's used in the brush
/// dispatch constructor.
enum brushtype
{
    square = 0,
    tri    = 1,
    circle = 2,
    line   = 3
};

/// An abstract base class for the concept of a brush, which draws something to
/// the screen according to the parameters in the constructor.
class brush
{
public:
    /// Draw the brush onscreen.  Not implemented in the base class.
    virtual void draw() = 0;

    /// Static method to construct a brush of the given type, with the given
    /// parameters.
    ///
    /// \param type
    /// The type of brush to make.
    static brush* dispatchConstructor(const brushtype type,
                                      const double    size,
                                      const vec2      &position,
                                      const color     &drawcolor,
                                      const double    alphadecay,
                                      const double    angle);

    /// Get the next brushtype after this brushtype.
    static brushtype nextBrushType(brushtype type);

protected:
    /// Constructor.  Note that it is protected, so that this object can not be
    /// arbitrarily constructed.
    ///
    /// \param size
    /// The size (nominally in pixels) of the brush.
    /// 
    /// \param position
    /// The position of the brush onscreen
    /// 
    /// \param drawcolor
    /// The drawing color of the brush.
    ///
    /// \param alphadecay
    /// The alpha value of the outer edges of the brush.  What this means is up
    /// to the specific brush implementation
    ///    
    /// \param angle
    /// The angle of the brush in degrees.
    brush(const double  size,
          const vec2   &position,
          const color  &drawcolor,
          const double  alphadecay,
          const double  angle);

    double mSize;
    color  mColor;
    vec2   mPosition;
    double mAlphadecay;
    double mAngle;
};

/// A brush implementation that draws a square onscreen, with possible alpha
/// blending.
class squarebrush : public brush
{
public:
    squarebrush(const double  size,
                const vec2   &position,
                const color  &drawcolor,
                const double  alphadecay,
                const double  angle);

    virtual void draw();
};

/// A brush implementation that draws a triangle onscreen, with possible alpha
/// blending.
class tribrush : public brush
{
public:
    tribrush(const double  size,
             const vec2   &position,
             const color  &drawcolor,
             const double  alphadecay,
             const double  angle);
    
    virtual void draw();
};

/// A brush implementation that draws a circle onscreen, with possible alpha
/// blending.
class circlebrush : public brush
{
public:
    circlebrush(const double  size,
                const vec2   &position,
                const color  &drawcolor,
                const double  alphadecay,
                const double  angle);

    virtual void draw();
};

/// A brush implementation that draws a line onscreen, with possible alpha
/// blending.
class linebrush : public brush
{
public:
    linebrush(const double  size,
              const vec2   &position,
              const color  &drawcolor,
              const double  alphadecay,
              const double  angle);

    virtual void draw();
};

#endif
