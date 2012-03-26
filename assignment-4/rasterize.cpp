// CSCE 441 Assignment 4
// Polygon Rasterization

// Taahir Ahmed

// The Eigen library is used for matrix operations


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
using Eigen::Matrix;
#include "Eigen/StdVector"
EIGEN_DEFINE_STL_VECTOR_SPECIALIZATION(Vector4f)

#include <GL/glut.h>

int window_width(400);
int window_height(400);

Vector4f zero_vector(0.0, 0.0, 0.0, 0.0);
Vector4f neginf_vector(-std::numeric_limits<double>::infinity(),
                       -std::numeric_limits<double>::infinity(),
                       -std::numeric_limits<double>::infinity(),
                       -std::numeric_limits<double>::infinity());

vector<Vector4f> framebuffer(window_width*window_height, zero_vector);
vector<Vector4f> auxbuffer(window_width*window_height, neginf_vector);

Vector4f amblight_color(0.5, 0.5, 0.5, 1.0);

Vector4f dirlight_color(1.0, 1.0, 1.0, 1.0);
Vector4f dirlight_dir(-1.0, -1.0, 1.0, 0.0);

Vector4f mat_ambient(0.1, 0.0, 0.0, 0.0);
Vector4f mat_diffuse(0.7, 0.0, 0.0, 0.0);
Vector4f mat_specular(0.5, 0.5, 0.5, 0.5);
double   mat_exponent(5.0);

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT);

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewPort(0, 0, window_width, window_height);

    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(window_width, window_height);

    glutCreateWindow("Taahir Ahmed - Assignment 4");

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    //glutKeyboardFunc(keyboardCallbac);

    glutMainLoop();
    return 0;
}

