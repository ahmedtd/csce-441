
#include "brush.hpp"

brush::brush(double size,
             vec2   position,
             color  drawcolor,
             double alphadecay,
             double angle)
:
    mSize(size),
    mPosition(position),
    mColor(color),
    mAlphadecay(alphadecay),
    mAngle(angle)
{
    
}

squarebrush::squarebrush(double size,
                         vec2   position,
                         color  drawcolor,
                         double alphadecay,
                         double angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
    
}

void squarebrush::draw()
{
}

tribrush::tribrush(double size,
                   vec2   position,
                   color  drawcolor,
                   double alphadecay,
                   double angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
}

circlebrush::circlebrush(double size,
                         vec2   position,
                         color  drawcolor,
                         double alphadecay,
                         double angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
}

linebrush::linebrush(double size,
                   vec2   position,
                   color  drawcolor,
                   double alphadecay,
                   double angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
}
