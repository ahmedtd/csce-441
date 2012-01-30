
/// \file paint.cpp
/// Main application file.

#include <iostream>
using std::cout;
using std::endl;

#include <GL/glut.h>

#include "brush.hpp"

int window_width(800);
int window_height(600);

// Current brush settings
brushtype currentType(square);
double    currentSize(8.0);
double    currentAlphaDecay(1.0);
color     currentColor(1.0, 1.0, 1.0, 1.0);
double    currentAngle(0.0);

/// This is the glut-registered display callback.  It doesn't do anything, but
/// I'm keeping it around because it seems weird to have a graphics program
/// without a display function.
void displayCallback(void)
{

}

/// This is the glut-registered window reshape callback.
void reshapeCallback(int width, int height)
{
    // Update the global information on the window shape
    window_width = width;
    window_height = height;

    // Update the projection matrix to account for the window shape
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, window_width, window_height, 0, -1.0, 1.0);

    // Change the viewport shape to prevent distortion due to non-square windows
    glViewport(0, 0, window_width, window_height);

    // Clear the buffer to black and flush our changes to screen.
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

/// This is the glut-registered keyboard callback function.
void keyboardCallback(unsigned char key, int x, int y)
{
    // Should we change the current brush type?
    if(key == 'b')
    {
        currentType = brush::nextBrushType(currentType);
    }

    // Should we clear the screen?
    else if(key == 'c')
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }

    // Should we enable spray paint mode?
    else if(key == 'a')
    {
        if(currentAlphaDecay == 1.0)
            currentAlphaDecay = 0.0;
        else
            currentAlphaDecay = 1.0;
    }

    // Should we rotate the current brush?
    else if(key == 'r')
    {
        currentAngle += 10.0;
    }

    // Should we increase the current brush size?
    else if(key == '+' || key == '=')
    {
        currentSize *= 2.0;
        if(currentSize > 128.0)
            currentSize = 128.0;
    }

    // Should we decrease the current brush size?
    else if(key == '-')
    {
        currentSize /= 2.0;
        if(currentSize < 1.0)
            currentSize = 1.0;
    }

    // Should we change the current brush color?
    else if(key >= '1' && key <= '7')
    {
        int packed = key - '1' + 1;
        int rcomp  = (packed & 0x1);
        int bcomp  = (packed & 0x2) >> 1;
        int gcomp  = (packed & 0x4) >> 2;
        
        currentColor = color(rcomp, bcomp, gcomp, 1.0);
    }
}

/// This is the glut-registered callback function for mouse motion.  It draws a
/// brush with the current specifications at the given location on-screen.
void mouseMoveCallback(int x, int y)
{
    // Build a brush from the current specifications and paint it to the buffer.
    brush *currentBrush = brush::dispatchConstructor(currentType,
                                                     currentSize,
                                                     vec2(x, y),
                                                     currentColor,
                                                     currentAlphaDecay,
                                                     currentAngle);
    currentBrush->draw();
    delete currentBrush;

    // Ensure that our draws are written to the screen.
    glFlush();
}

/// Main function takes care of some basic, one-time initialization.  Code that
/// should be run every time the window resizes is pushed to the window resize
/// callback
int main(int argc, char **argv)
{
    // Initialize glut and window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(window_width, window_height); 
        
    glutCreateWindow ("Taahir Ahmed - Assignment 1");

    // The clear color is transparent black
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    // Enable blending to allow the spray paint effect
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Register glut callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutMotionFunc(mouseMoveCallback);
    glutKeyboardFunc(keyboardCallback);

    // Pass control to glut
    glutMainLoop();
    return 0;
}
