
#ifndef BRUSH
#define BRUSH

#include "color.hpp"
#include "geometry.hpp"

enum brushtype
{
    square,
    tri,
    circle,
    line
};


class brush
{
public:
    brush(const double  size,
          const vec2   &position,
          const color  &drawcolor,
          const double  alphadecay,
          const double  angle);
    
    virtual void draw() = 0;

    static brush* dispatchConstructor(const brushtype type,
                                      const double    size,
                                      const vec2      &position,
                                      const color     &drawcolor,
                                      const double    alphadecay,
                                      const double    angle);
protected:
    double mSize;
    color  mColor;
    vec2   mPosition;
    double mAlphadecay;
    double mAngle;
};

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
