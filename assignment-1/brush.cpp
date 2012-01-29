
#include "brush.hpp"

#include <GL/gl.h>

brush::brush(const double  size,
             const vec2   &position,
             const color  &drawcolor,
             const double  alphadecay,
             const double  angle)
:
    mSize(size),
    mPosition(position),
    mColor(drawcolor),
    mAlphadecay(alphadecay),
    mAngle(angle)
{
    
}

brush* brush::dispatchConstructor(const brushtype type,
                                  const double    size,
                                  const vec2      &position,
                                  const color     &drawcolor,
                                  const double    alphadecay,
                                  const double    angle)
{
    if(type == square)
    {
        return new squarebrush(size, position, drawcolor, alphadecay, angle);
    }
    else if(type == tri)
    {
        return new tribrush(size, position, drawcolor, alphadecay, angle);
    }
    else if(type == line)
    {
        return new linebrush(size, position, drawcolor, alphadecay, angle);
    }
    else if(type == circle)
    {
        return new circlebrush(size, position, drawcolor, alphadecay, angle);
    }
}

squarebrush::squarebrush(const double  size,
                         const vec2   &position,
                         const color  &drawcolor,
                         const double  alphadecay,
                         const double  angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
    
}

void squarebrush::draw()
{
    color innerColor(mColor);
    color outerColor(mColor);
    outerColor.a(mAlphadecay);

    // Transform operations
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotated(mAngle, 0.0, 0.0, 1.0);
    glTranslated(mPosition.x(), mPosition.y(), 0.0);

    glBegin(GL_TRIANGLE_FAN);
    
    // Fan center
    glColor4dv(innerColor);
    glVertex2dv(vec2(0.0, 0.0));
    
    // Fan periphery
    glColor4dv(outerColor);
    glVertex2dv(vec2( mSize,  mSize));
    glVertex2dv(vec2( mSize, -mSize));
    glVertex2dv(vec2(-mSize, -mSize));
    glVertex2dv(vec2(-mSize,  mSize));
    glVertex2dv(vec2( mSize,  mSize));

    glEnd();

    // Undo transform
    glPopMatrix();
}

tribrush::tribrush(const double  size,
                   const vec2   &position,
                   const color  &drawcolor,
                   const double  alphadecay,
                   const double  angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
}

void tribrush::draw()
{
}

circlebrush::circlebrush(const double  size,
                         const vec2   &position,
                         const color  &drawcolor,
                         const double  alphadecay,
                         const double  angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
}

void circlebrush::draw()
{
}

linebrush::linebrush(const double  size,
                     const vec2   &position,
                     const color  &drawcolor,
                     const double  alphadecay,
                     const double  angle)
:
    brush(size, position, drawcolor, alphadecay, angle)
{
}

void linebrush::draw()
{
}
