
#include "brush.hpp"

#include <complex>
using std::atan;
using std::complex;
using std::exp;
#include <iostream>
using std::cout;
using std::endl;

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

brushtype brush::nextBrushType(brushtype type)
{
    if(type == square)
        return tri;
    else if(type == tri)
        return circle;
    else if(type == circle)
        return line;
    else
        return square;
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
    // Create colors for the inner and outer vertices
    color innerColor(mColor);
    color outerColor(mColor);
    outerColor.a(mAlphadecay);

    // Transform operations -- put the origin at the correct position and angle
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(mPosition.x(), mPosition.y(), 0.0);
    glRotated(mAngle, 0.0, 0.0, 1.0);
    
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
    // Create colors for the inner and outer vertices
    color innerColor(mColor);
    color outerColor(mColor);
    outerColor.a(mAlphadecay);

    // Transform ops
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(mPosition.x(), mPosition.y(), 0.0);
    glRotated(mAngle, 0.0, 0.0, 1.0);
    
    glBegin(GL_TRIANGLE_FAN);

    // Fan center
    glColor4dv(innerColor);
    glVertex2dv(vec2(0.0, 0.0));
    
    // Fan periphery
    glColor4dv(outerColor);
    glVertex2dv(vec2(-mSize, -mSize));
    glVertex2dv(vec2( mSize, -mSize));
    glVertex2dv(vec2( 0.0,    mSize));
    glVertex2dv(vec2(-mSize, -mSize));

    glEnd();
    
    // Undo transform
    glPopMatrix();
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
    // Create colors for the inner and outer vertices
    color innerColor(mColor);
    color outerColor(mColor);
    outerColor.a(mAlphadecay);

    // Transforms
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(mPosition.x(), mPosition.y(), 0.0);
    glRotated(mAngle, 0.0, 0.0, 1.0);
    
    glBegin(GL_TRIANGLE_FAN);
    
    // Fan center
    glColor4dv(innerColor);
    glVertex2dv(vec2(0.0, 0.0));
    
    // Fan periphery
    glColor4dv(outerColor);
    double pi = 4.0 * atan(1.0);
    for(int step=0; step < 40; step++)
    {
        // Use complex number operations to get sine and cosine -- depending on
        // the standard library used, could be faster than separate sine and
        // cosine calls.
        complex<double> angle(0.0, step*2.0*pi/40.0);
        complex<double> comps(mSize*exp(angle));
        
        glVertex2dv(vec2(comps.real(), comps.imag()));
    }
    // Draw the last triangle with a fixed value to eliminate angle over- or
    // undershoot
    glVertex2dv(vec2(mSize, 0.0));

    glEnd();
    
    // Undo transform
    glPopMatrix();
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
    // Create colors for the inner and outer vertices
    color innerColor(mColor);
    color outerColor(mColor);
    outerColor.a(mAlphadecay);

    // Transforms
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(mPosition.x(), mPosition.y(), 0.0);
    glRotated(mAngle, 0.0, 0.0, 1.0);
    
    // Perform the drawing as two line segments to get a sensible blending
    // pattern.
    glBegin(GL_LINE);
    
    glColor4dv(innerColor);
    glVertex2dv(vec2(0.0, 0.0));
    glColor4dv(outerColor);
    glVertex2dv(vec2(mSize, 0.0));

    glColor4dv(innerColor);
    glVertex2dv(vec2(0.0, 0.0));
    glColor4dv(outerColor);
    glVertex2dv(vec2(-mSize, 0.0));
    
    glEnd();
    
    // Undo transform
    glPopMatrix();
}
