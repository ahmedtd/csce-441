
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
#include "infinite_plane.hpp"
#include "affine_renderable.hpp"
#include "vecops.hpp"

framebuffer renderbuffer(800, 800);

void render_to_buffer()
{
    scene testscene;
    
    testscene.voidcolor() = fvec("0 0.4 1 1");

    dirlight light_a(vec("0 0 -1"),
                     fvec("1 1 1"));
    
    testscene.lights().insert(&light_a);

    viewport view(vec("-20 0 1"),
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
                         1.0,
                         0.0,
                         2.0);

    material_phong mat_c(fvec("0.2 0.5 0.2"),
                         fvec("0.2 0.5 0.2"),
                         100.0,
                         0.0,
                         0.0,
                         2.0);

    material_phong mat_d(fvec("0.7 0.7 0.7"),
                         fvec("0.7 0.7 0.7"),
                         10.0,
                         0.0,
                         0.0,
                         2.0);
                         

    sphere sphere_a((vec("10 0 2")), 1.0);
    sphere sphere_b((vec("12 3 3")), 2.0);

    sphere sphere_c(vec("9 5 5"), 0.1);

    sphere sphere_d(vec("100 -10 10"), 40);

    sphere unit_sphere(vec("0 0 0"),
                       1.0);

    set<renderable*> ellipse_child;
    ellipse_child.insert(&unit_sphere);

    mat trans = rotmat(vec("0 0 1"), math::pi()/6);
    affine_renderable ellipse(trans,
                              vec("0 0 0"),
                              ellipse_child);

    infinite_plane plane_a(vec("0 0 0"),
                           vec("0.1 0.2 1"));
                           

    //testscene.renderables().insert(&sphere_a);
    testscene.materials()[&sphere_a] = &mat_a;
    
    //testscene.renderables().insert(&sphere_b);
    testscene.materials()[&sphere_b] = &mat_b;
    
    //testscene.renderables().insert(&sphere_c);
    testscene.materials()[&sphere_c] = &mat_a;

    //testscene.renderables().insert(&sphere_d);
    testscene.materials()[&sphere_d] = &mat_a;

    //testscene.renderables().insert(&plane_a);
    testscene.materials()[&plane_a] = &mat_a;

    //testscene.renderables().insert(&unit_sphere)
    testscene.renderables().insert(&ellipse);
    testscene.materials()[&unit_sphere] = &mat_d;

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

    glutMainLoop();
    return 0;
}
