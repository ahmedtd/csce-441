
#include <GL/glut.h>

#include <iostream>
using std::cout;
using std::endl;
#include <set>
using std::set;

#include <armadillo>
using arma::fvec;
using arma::vec;
using arma::mat;
using arma::solve;
using arma::cross;
using arma::norm;
using arma::math;

#include "framebuffer.hpp"
#include "renderable.hpp"
#include "sphere.hpp"
#include "viewport.hpp"
#include "ray.hpp"
#include "scene.hpp"
#include "material_phong.hpp"
#include "dirlight.hpp"
#include "pointlight.hpp"

framebuffer renderbuffer(800, 800);

void render_to_buffer()
{
    scene testscene;
    
    testscene.voidcolor() = fvec("0 0.1 0.5 1");

    dirlight light_a;
    
    testscene.lights().insert(&light_a);

    viewport view(vec("0 0 0"),
                  vec("1 0 0"),
                  vec("0 0 1"),
                  math::pi()/3.0,
                  math::pi()/3.0);

    testscene.viewports().insert(&view);

    material_phong mat_a(fvec("0.5 0.5 0.5"),
                         fvec("0.5 0.5 0.5"),
                         100.0,
                         1.0,
                         1.0,
                         3.0);

    material_phong mat_b(fvec("0.5 0.2 0.2"),
                         fvec("0.5 0.2 0.2"),
                         100.0,
                         0.0,
                         0.0,
                         2.0);
                         

    sphere sphere_a((vec("10 0 2")), 1.0);
    sphere sphere_b((vec("12 3 3")), 2.0);

    sphere sphere_c(vec("9 5 5"), 0.1);

    sphere sphere_d(vec("100 -10 10"), 40);

    testscene.renderables().insert(&sphere_a);
    testscene.materials()[&sphere_a] = &mat_a;
    
    testscene.renderables().insert(&sphere_b);
    testscene.materials()[&sphere_b] = &mat_a;
    
    testscene.renderables().insert(&sphere_c);
    testscene.materials()[&sphere_c] = &mat_a;

    testscene.renderables().insert(&sphere_d);
    testscene.materials()[&sphere_d] = &mat_a;

    testscene.render(renderbuffer);
}

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawPixels(renderbuffer.width(),
                 renderbuffer.height(),
                 GL_RGBA,
                 GL_FLOAT,
                 renderbuffer.pointer());
    
    glFlush();
}

void reshapeCallback(int new_width, int new_height)
{
    renderbuffer.reshape(new_width, new_height);

    render_to_buffer();
    
    glViewport(0, 0, new_width, new_height);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(renderbuffer.width(), renderbuffer.height());
    
    glutCreateWindow("Taahir Ahmed - Homework 5");
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Register callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);

    render_to_buffer();

    glutMainLoop();
    return 0;
}
