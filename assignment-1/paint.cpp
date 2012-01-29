
#include <iostream>
using std::cout;
using std::endl;

#include <GL/glut.h>
#include <GL/gl.h>

#include "brush.hpp"

int window_width(800);
int window_height(600);

brushtype currentType(square);
double    currentSize(8.0);
double    currentAlphaDecay(1.0);
color     currentColor(1.0, 1.0, 1.0, 1.0);
double    currentAngle(0.0);

void displayCallback(void)
{

}

void reshapeCallback(int width, int height)
{
    // Update the global information on the window shape
    window_width = width;
    window_height = height;

    //cout << window_width << " " << window_height << endl;
    
    // Update the projection matrix to account for the window shape
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, window_width, window_height, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if(key == 'b')
    {
    }
    else if(key == 'c')
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if(key == 'a')
    {
        if(currentAlphaDecay == 1.0)
            currentAlphaDecay == 0.0;
        else
            currentAlphaDecay == 1.0;
    }
    else if(key >= '1' && key <= '7')
    {
        int packed = key - '1' - 1;
        int rcomp  = (packed & 0x1);
        int bcomp  = (packed & 0x2) >> 1;
        int gcomp  = (packed & 0x4) >> 2;
        
        currentColor = color(rcomp, bcomp, gcomp, 1.0);
    }
}

void mouseMoveCallback(int x, int y)
{
    vec2 position(x, y);
    brush *currentBrush = brush::dispatchConstructor(currentType,
                                                     currentSize,
                                                     position,
                                                     currentColor,
                                                     currentAlphaDecay,
                                                     currentAngle);
    currentBrush->draw();
    delete currentBrush;

    glFlush();

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    // Initialize glut and window
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(window_width, window_height); 
    
    glutCreateWindow ("Taahir Ahmed - Assignment 1");

    // Default to transparent black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    // Register glut callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutMotionFunc(mouseMoveCallback);
    glutKeyboardFunc(keyboardCallback);

    // Pass control to glut
    glutMainLoop();
    return 0;
}
