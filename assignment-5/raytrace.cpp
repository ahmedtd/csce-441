
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
#include "cylinder.hpp"

framebuffer renderbuffer(800, 800);

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

void keyboardCallback(unsigned char key, int x, int y)
{
    if(key == '1')
    {
        cout << "Composing and rendering scene 1..." << endl;

        scene scene1;
        scene1.voidcolor() = fvec("0 0 0 1");

        // Set up the viewport
        viewport view(vec("-15 0 10"),
                  vec("1 0 -0.2"),
                  vec("0 0 1"),
                  math::pi()/3.0,
                  math::pi()/3.0);
        scene1.viewports().insert(&view);
        
        // Insert an ellipsoid

        material_phong ellipse_mat(fvec("0.7 0 0.7"),
                                   fvec("0.7 0 0.7"),
                                   3.0,
                                   0.0,
                                   0.0,
                                   4.0);

        sphere ellipse_base(vec("0 0 0"),
                            1.0);
        set<renderable*> ellipse_children;
        ellipse_children.insert(&ellipse_base);
        affine_renderable ellipse(vec("0 0 1"),
                                  1.0,
                                  vec("1 2 3"),
                                  vec("5 -1 5"),
                                  ellipse_children);

        scene1.renderables().insert(&ellipse);
        scene1.materials()[&ellipse_base] = &ellipse_mat;

        // Insert a sphere
        
        sphere sphere_a(vec("5 2 10"),
                        2.0);
        
        material_phong sphere_a_mat(fvec("0 0.5 0.5"),
                                    fvec("0 0.5 0.5"),
                                    10.0,
                                    0.0,
                                    0.0,
                                    4.0);
        
        scene1.renderables().insert(&sphere_a);
        scene1.materials()[&sphere_a] = &sphere_a_mat;

        // Insert a plane
        
        infinite_plane plane_a(vec("0 0 0"),
                               vec("0 0 1"));

        material_phong plane_a_mat(fvec("0.2 0.7 0.1"),
                                   fvec("0.2 0.7 0.1"),
                                   12.0,
                                   0.0,
                                   0.0,
                                   3.0);
        
        scene1.renderables().insert(&plane_a);
        scene1.materials()[&plane_a] = &plane_a_mat;

        // Insert lights
        pointlight light_a(fvec("1 1 1"),
                           vec("5 3 20"));
        scene1.lights().insert(&light_a);

        pointlight light_b(fvec("1 1 1"),
                           vec("-5 -3 16"));
        scene1.lights().insert(&light_b);

        scene1.render(renderbuffer);
    }
    else if(key == '2')
    {
        cout << "Composing and rendering scene 2..." << endl;

        scene scene2;
        scene2.voidcolor() = fvec("0 0 0 1");

        // Set up the viewport
        viewport view(vec("-20 0 10"),
                  vec("1 0 -0.2"),
                  vec("0 0 1"),
                  math::pi()/3.0,
                  math::pi()/3.0);
        scene2.viewports().insert(&view);

        // Insert a plane
        
        infinite_plane plane_a(vec("0 0 0"),
                               vec("0 0 1"));

        material_phong plane_a_mat(fvec("0.2 0.7 0.1"),
                                   fvec("0.2 0.7 0.1"),
                                   12.0,
                                   0.0,
                                   0.0,
                                   3.0);
        
        scene2.renderables().insert(&plane_a);
        scene2.materials()[&plane_a] = &plane_a_mat;

        // Insert a sphere
        
        sphere sphere_a(vec("0 -5 3"),
                        3.0);

        material_phong mat_sphere_a(fvec("0.7 0.3 0.2"),
                                    fvec("0.7 0.3 0.2"),
                                    35.0,
                                    0.0,
                                    0.0,
                                    4.0);

        scene2.renderables().insert(&sphere_a);
        scene2.materials()[&sphere_a] = &mat_sphere_a;

        // Insert a cylinder

        cylinder cyl_a(vec("0 5 5"),
                       vec("1 1 1"),
                       2.0,
                       -2.0,
                       2.0);

        material_phong mat_cyl_a(fvec("0.3 0.2 0.7"),
                                 fvec("0.3 0.2 0.7"),
                                 10.0,
                                 0.0,
                                 0.0,
                                 3.0);
        
        scene2.renderables().insert(&cyl_a);
        scene2.materials()[&cyl_a] = &mat_cyl_a;

        // Insert lights
        
        pointlight light_a(fvec("1 1 1"),
                           vec("0 0 6"));
        scene2.lights().insert(&light_a);

        pointlight light_b(fvec("1 0.1 0.1"),
                           vec("5 10 10"));
        scene2.lights().insert(&light_b);

        scene2.render(renderbuffer);
        
    }
    else if(key == '3')
    {
        cout << "Composing and rendering scene 3..." << endl;

        scene scene3;
        scene3.voidcolor() = fvec("0 0 0 1");

        // Set up the viewport
        
        viewport view(vec("-20 0 10"),
                  vec("1 0 -0.2"),
                  vec("0 0 1"),
                  math::pi()/3.0,
                  math::pi()/3.0);
        scene3.viewports().insert(&view);

        // Insert a green ground plane
        infinite_plane plane_a(vec("0 0 0"),
                               vec("0 0 1"));

        material_phong plane_a_mat(fvec("0.2 0.7 0.1"),
                                   fvec("0.2 0.7 0.1"),
                                   12.0,
                                   0.0,
                                   0.0,
                                   3.0);
        
        scene3.renderables().insert(&plane_a);
        scene3.materials()[&plane_a] = &plane_a_mat;

        // Insert a reflective cylinder
        
        cylinder cyl_a(vec("5 5 5"),
                       vec("1 1 1"),
                       2.0,
                       -2.0,
                       2.0);

        material_phong mat_cyl_a(fvec("0.3 0.2 0.7"),
                                 fvec("0.3 0.2 0.7"),
                                 10.0,
                                 1.0,
                                 0.0,
                                 3.0);
        
        scene3.renderables().insert(&cyl_a);
        scene3.materials()[&cyl_a] = &mat_cyl_a;

        // Insert a nearby reflective sphere
        sphere sphere_a(vec("5 0 2"),
                        2.0);
        
        material_phong mat_sphere_a(fvec("0.2 0.2 0.2"),
                                    fvec("0.2 0.2 0.2"),
                                    100,
                                    1.0,
                                    0,
                                    2);

        scene3.renderables().insert(&sphere_a);
        scene3.materials()[&sphere_a] = &mat_sphere_a;

        sphere sphere_b(vec("0 0 2"),
                        1.0);
        
        material_phong mat_sphere_b(fvec("1.0 0.5 0.5"),
                                    fvec("1.0 0.5 0.5"),
                                    50,
                                    0.7,
                                    0.0,
                                    1);
        
        scene3.renderables().insert(&sphere_b);
        scene3.materials()[&sphere_b] = &mat_sphere_b;

        // Insert a huge sphere, far away
        sphere sphere_c(vec("40 -10 20"),
                        20);

        material_phong mat_sphere_c(fvec("0.5 0.5 0.5"),
                                    fvec("0.5 0.5 0.5"),
                                    35,
                                    1.0,
                                    0.0,
                                    3.0);
        
        scene3.renderables().insert(&sphere_c);
        scene3.materials()[&sphere_c] = &mat_sphere_c;

        // Insert lights

        pointlight light_a(fvec("1 1 1"),
                            vec("5 -1 10"));
        scene3.lights().insert(&light_a);

        scene3.render(renderbuffer);
        
    }
    else if(key == '4')
    {
        cout << "Composing and rendering scene 4.  This scene has many"
                " reflections, so please be patient..."
             << endl;

        scene scene4;
        scene4.voidcolor() = fvec("0 0 0 1");

        // Set up the viewport
        viewport view(vec("-20 0 10"),
                  vec("1 0 -0.2"),
                  vec("0 0 1"),
                  math::pi()/3.0,
                  math::pi()/3.0);
        scene4.viewports().insert(&view);

        material_phong mat_silver(fvec("0.2 0.2 0.2"),
                                  fvec("0.2 0.2 0.2"),
                                  50.0,
                                  1.0,
                                  0.0,
                                  3.0);

        vec center1(" 2  2 2");
        vec center2("-2  2 2");
        vec center3("-2 -2 2");
        vec center4(" 2 -2 2");
        vec center5(" 0  0 4.828");

        sphere sphere_1(center1, 2.0);
        sphere sphere_2(center2, 2.0);
        sphere sphere_3(center3, 2.0);
        sphere sphere_4(center4, 2.0);
        sphere sphere_5(center5, 2.0);

        scene4.renderables().insert(&sphere_1);
        scene4.renderables().insert(&sphere_2);
        scene4.renderables().insert(&sphere_3);
        scene4.renderables().insert(&sphere_4);
        scene4.renderables().insert(&sphere_5);

        scene4.materials()[&sphere_1] = &mat_silver;
        scene4.materials()[&sphere_2] = &mat_silver;
        scene4.materials()[&sphere_3] = &mat_silver;
        scene4.materials()[&sphere_4] = &mat_silver;
        scene4.materials()[&sphere_5] = &mat_silver;

        vec offset_a("10 10 0");

        sphere sphere_a_1(center1 + offset_a, 2.0);
        sphere sphere_a_2(center2 + offset_a, 2.0);
        sphere sphere_a_3(center3 + offset_a, 2.0);
        sphere sphere_a_4(center4 + offset_a, 2.0);
        sphere sphere_a_5(center5 + offset_a, 2.0);

        scene4.renderables().insert(&sphere_a_1);
        scene4.renderables().insert(&sphere_a_2);
        scene4.renderables().insert(&sphere_a_3);
        scene4.renderables().insert(&sphere_a_4);
        scene4.renderables().insert(&sphere_a_5);

        scene4.materials()[&sphere_a_1] = &mat_silver;
        scene4.materials()[&sphere_a_2] = &mat_silver;
        scene4.materials()[&sphere_a_3] = &mat_silver;
        scene4.materials()[&sphere_a_4] = &mat_silver;
        scene4.materials()[&sphere_a_5] = &mat_silver;

        vec offset_b("10 -10 0");
        
        sphere sphere_b_1(center1 + offset_b, 2.0);
        sphere sphere_b_2(center2 + offset_b, 2.0);
        sphere sphere_b_3(center3 + offset_b, 2.0);
        sphere sphere_b_4(center4 + offset_b, 2.0);
        sphere sphere_b_5(center5 + offset_b, 2.0);

        scene4.renderables().insert(&sphere_b_1);
        scene4.renderables().insert(&sphere_b_2);
        scene4.renderables().insert(&sphere_b_3);
        scene4.renderables().insert(&sphere_b_4);
        scene4.renderables().insert(&sphere_b_5);

        scene4.materials()[&sphere_b_1] = &mat_silver;
        scene4.materials()[&sphere_b_2] = &mat_silver;
        scene4.materials()[&sphere_b_3] = &mat_silver;
        scene4.materials()[&sphere_b_4] = &mat_silver;
        scene4.materials()[&sphere_b_5] = &mat_silver;

        vec offset_c("30 0 0");
        
        sphere sphere_c_1(center1 + offset_c, 2.0);
        sphere sphere_c_2(center2 + offset_c, 2.0);
        sphere sphere_c_3(center3 + offset_c, 2.0);
        sphere sphere_c_4(center4 + offset_c, 2.0);
        sphere sphere_c_5(center5 + offset_c, 2.0);

        scene4.renderables().insert(&sphere_c_1);
        scene4.renderables().insert(&sphere_c_2);
        scene4.renderables().insert(&sphere_c_3);
        scene4.renderables().insert(&sphere_c_4);
        scene4.renderables().insert(&sphere_c_5);

        scene4.materials()[&sphere_c_1] = &mat_silver;
        scene4.materials()[&sphere_c_2] = &mat_silver;
        scene4.materials()[&sphere_c_3] = &mat_silver;
        scene4.materials()[&sphere_c_4] = &mat_silver;
        scene4.materials()[&sphere_c_5] = &mat_silver;

        // Insert a ground plane
        infinite_plane plane_a(vec("0 0 0"),
                               vec("0 0 1"));

        material_phong plane_a_mat(fvec("0.5 0.5 0.5"),
                                   fvec("0.5 0.5 0.5"),
                                   12.0,
                                   0.7,
                                   0.0,
                                   3.0);
        
        scene4.renderables().insert(&plane_a);
        scene4.materials()[&plane_a] = &plane_a_mat;
        
        infinite_plane plane_b(vec("0 0 15"),
                               vec("0 0 -1"));

        material_phong plane_b_mat(fvec("0.5 0.5 0.5"),
                                   fvec("0.5 0.5 0.5"),
                                   12.0,
                                   0.0,
                                   0.0,
                                   3.0);
        
        scene4.renderables().insert(&plane_b);
        scene4.materials()[&plane_b] = &plane_b_mat;


        vec light_up("0 0 7.5");
        
        pointlight light_a(fvec("1 1 1 1"), light_up);
        scene4.lights().insert(&light_a);

        pointlight light_b(fvec("1 0 0 1"), light_up + offset_a);
        scene4.lights().insert(&light_b);

        pointlight light_c(fvec("0 1 0 1"), light_up + offset_b);
        scene4.lights().insert(&light_c);

        pointlight light_d(fvec("0 0 1 1"), light_up + offset_c);
        scene4.lights().insert(&light_d);

        scene4.render(renderbuffer);
    }

    glutPostRedisplay();
}

void reshapeCallback(int new_width, int new_height)
{
    renderbuffer.reshape(new_width, new_height);

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
    glutKeyboardFunc(keyboardCallback);

    glutMainLoop();
    return 0;
}
