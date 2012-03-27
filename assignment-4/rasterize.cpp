// CSCE 441 Assignment 4
// Polygon Rasterization

// Taahir Ahmed

// The Eigen library is used for matrix operations

#include <algorithm>
using std::fill;
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cout;
using std::endl;
#include <limits>
using std::numeric_limits;
#include <vector>
using std::vector;

#include "Eigen/Dense"
using Eigen::Array;
using Eigen::Vector4f;
using Eigen::Vector3f;
using Eigen::Matrix;

#include "eigendef.hpp"

#include <GL/glut.h>

#include "model.hpp"

int window_width(400);
int window_height(400);

Vector4f opaque_black(0.0, 0.0, 0.0, 1.0);
Vector4f opaque_red(1.0, 0.0, 0.0, 1.0);

vector<Vector4f> framebuffer(window_width*window_height, opaque_black);
vector<float> auxbuffer(window_width*window_height,
                        -std::numeric_limits<float>::infinity());

model *themodel;

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT);

    fill(framebuffer.begin(), framebuffer.end(), opaque_black);
    fill(auxbuffer.begin(),
         auxbuffer.end(),
         std::numeric_limits<float>::infinity());

    themodel->render(framebuffer, auxbuffer);

    glDrawPixels(window_width,
                 window_height,
                 GL_RGBA,
                 GL_FLOAT,
                 &(framebuffer[0]));

    glFlush();
}

void reshapeCallback(int new_width, int new_height)
{
    window_width = new_width;
    window_height = new_height;

    framebuffer.resize(window_width*window_height,
                       opaque_black);
    auxbuffer.resize(window_width*window_height,
                     std::numeric_limits<float>::infinity());

    glFlush();
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if(key == '1')
        themodel->mRenderMode = flat;
    else if(key == '2')
        themodel->mRenderMode = pervertex;
    else if(key == '3')
        themodel->mRenderMode = perpixel;

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(window_width, window_height);

    glutCreateWindow("Taahir Ahmed - Assignment 4");

    glClearColor(0.0, 1.0, 0.0, 0.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);

    ifstream modelstream(argv[1]);
    themodel = new model(modelstream);

    glutMainLoop();
    return 0;
}

