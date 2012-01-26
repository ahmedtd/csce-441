#include <GL/gl.h>

#include "color.hpp"
#include "geometry.hpp"

class brush
{
public:
    brush(double size,
          vec2   position,
          color  drawcolor,
          double alphadecay,
          double angle);
    
    virtual void draw() = 0;

private:
    double mSize;
    color  mColor;
    vec2   mPosition;
    double mAlphadecay;
    double mAngle;
};

class squarebrush : public brush
{
public:
    squarebrush(double size,
                vec2   position,
                color  drawcolor,
                double alphadecay,
                double angle);

    virtual void draw();

private:

};

class tribrush : public brush
{
public:
    tribrush(double size,
             vec2   position,
             color  drawcolor,
             double alphadecay,
             double angle);
    
    virtual void draw();
};

class circlebrush : public brush
{
public:
    circlebrush(double size,
                vec2   position,
                color  drawcolor,
                double alphadecay,
                double angle);

    virtual void draw();
};

class linebrush : public brush
{
public:
    linebrush(double size,
              vec2   position,
              color  drawcolor,
              double alphadecay,
              double angle);

    virtual void draw();
};
