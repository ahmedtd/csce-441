
#include <GL/glut.h>

#include <deque>
using std::deque;
#include <iostream>
using std::cout;
using std::endl;
#include <map>
using std::map;
using std::pair;
#include <vector>
using std::vector;

#include <armadillo>
using arma::accu;
using arma::endr;
using arma::eye;
using arma::inv;
using arma::norm;
using arma::mat;
using arma::span;
using arma::vec;

vector<mat> atList;
vector<vec> polygon;

vector<vector<vec> > diagnostics;

int depth_setting(3);

enum eProgramMode
{
    render,
    base_polygon,
    transform_define
} programMode(basePolygon);

mat genArbMat(vector<vec> src, vector<vec> dst)
{
    mat orig(src[0].n_rows, 3);
    orig.col(0) = src[0];
    orig.col(1) = src[1];
    orig.col(2) = src[2];
  
    mat hat(src[0].n_rows, 3);
    hat.col(0) = dst[0];
    hat.col(1) = dst[1];
    hat.col(2) = dst[2];
    
    return hat * inv(orig);
}

// Takes homogeneous coordinates
mat genScaleMat(double scaleFactor, const vec &origin, const vec &direction)
{
    // The output matrix will be a dim*dim square matrix
    int dim = origin.n_rows;
    
    mat multComponent;
    mat addComponent;
    
    if(norm(direction,2) == 0.0)
    {
        cout << "Uniform scale" << endl;
        
        multComponent = eye<mat>(dim-1, dim-1) * scaleFactor;
        addComponent = (1-scaleFactor) * origin;
    }
    else
    {
        cout << "Nonuniform scale" << endl;

        vec ndir = direction / norm(direction, 2);

        multComponent = (scaleFactor-1) * ndir * ndir.t();
        multComponent += eye<mat>(dim, dim);
        addComponent =
            (1-scaleFactor)*ndir*ndir.t()*origin;
    }

    mat scaleMat = eye<mat>(dim, dim);
    scaleMat.submat(span(0, 1), span(0,1))
        = multComponent.submat(span(0,1), span(0,1));
    scaleMat.submat(span(0, 1), span(2))
        = addComponent.submat(span(0,1), span(0));

    cout << scaleMat << endl;
   
    return scaleMat;
}

void fractal_render(int maxDepth)
{
    glColor4d(1.0, 1.0, 1.0, 1.0);

    // The int is the depth of evaluation, the matrix the transform at that
    // level.
    deque<pair<int, mat> > atQueue;
    
    // Prime the affine transform queue
    vector<mat>::const_iterator at_it = atList.begin();
    for(; at_it != atList.end(); at_it++)
    {
        pair<int, mat> newframe;
        newframe.first = 0;
        newframe.second = *at_it;
        atQueue.push_back(newframe);
    }
    
    while(atQueue.size() != 0)
    {
        pair<int, mat> frame = atQueue.front();
        atQueue.pop_front();
        
        if(frame.first < maxDepth)
        {
            at_it = atList.begin();
            for(; at_it != atList.end(); at_it++)
            {
                pair<int, mat> newframe;
                newframe.first = frame.first + 1;
                newframe.second = (*at_it) * frame.second;
                atQueue.push_back(newframe);
            }
        }
        else
        {
            glBegin(GL_TRIANGLE_STRIP);
            vector<vec>::const_iterator pt_it = polygon.begin();
            for(; pt_it != polygon.end(); pt_it++)
            {
                vec newpt = frame.second * (*pt_it);
                glVertex2d(newpt(0,0), newpt(1,0));
            }
            glEnd();
        }
    }
}

void displayCallback()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    switch(programMode)
    {
    case render:
        fractalRender(depth_setting);
        break;
    case base_polygon:
        drawBasePolygon(0);
        break;
    case transform_define:
        break;
    }

    glColor4f(1.0, 0.0, 0.0, 0.3);

    vector<vector<vec> >::const_iterator diag_it = diagnostics.begin();
    for(; diag_it != diagnostics.end(); diag_it++)
    {
        glBegin(GL_TRIANGLES);
    
        vector<vec>::const_iterator pt_it = diag_it->begin();
        for(; pt_it != diag_it->end(); pt_it++)
        {
            glVertex2d((*pt_it)(0,0), (*pt_it)(1,0));
        }
        
        glEnd();
    }
    
    glutSwapBuffers();
}

void keyboardCallback(unsigned char key, int x, int y)
{
    if(key == 'd')
        depth_setting++;
    if(key == 's')
        depth_setting--;

    if(key == 'b')
    {
        cout << "switch to base polygon mode" << endl;
        programMode = base_polygon;
    }
    if(key == 'r')
    {
        cout << "switch to render mode" << endl;
        programMode = render;
    }
    if(key == 't')
    {
        cout << "switch to transform mode" << endl;
        programMode = transform_define;
    }

    if(depth_setting < 0)
        depth_setting = 0;
    if(depth_setting > 10)
        depth_setting = 10;

    glutPostRedisplay();
}

void reshapeCallback(int newWidth, int newHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glViewport(0, 0, newWidth, newHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
    glutInitWindowSize(500, 500);
    
    glutCreateWindow("Fractal Drawer");

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Register glut callbacks
    glutDisplayFunc(displayCallback);
    glutReshapeFunc(reshapeCallback);
    glutKeyboardFunc(keyboardCallback);

    // Test inverse transform ability w/ fractal fern
    vector<vec> main_frond;
    vec main_frond1(3), main_frond2(3), main_frond3(3);
    main_frond1 << 0.0 << endr << -1.0 << endr << 1.0;
    main_frond2 << 0.0 << endr <<  1.0 << endr << 1.0;
    main_frond3 << 0.7 << endr <<  -0.9 << endr << 1.0;
    main_frond.push_back(main_frond1);
    main_frond.push_back(main_frond2);
    main_frond.push_back(main_frond3);

    vector<vec> r_frond;
    vec r_frond1(3), r_frond2(3), r_frond3(3);
    r_frond1 << 0.0 << endr << -0.9 << endr << 1.0;
    r_frond2 << 0.7 << endr << -0.8 << endr << 1.0;
    r_frond3 << 0.0 << endr << -0.7 << endr << 1.0;
    r_frond.push_back(r_frond1);
    r_frond.push_back(r_frond2);
    r_frond.push_back(r_frond3);

    vector<vec> l_frond;
    vec l_frond1(3), l_frond2(3), l_frond3(3);
    l_frond1 <<  0.0 << endr << -0.9 << endr << 1.0;
    l_frond2 << -0.7 << endr << -0.8 << endr << 1.0;
    l_frond3 <<  0.0 << endr << -0.7 << endr << 1.0;
    l_frond.push_back(l_frond1);
    l_frond.push_back(l_frond2);
    l_frond.push_back(l_frond3);

    vector<vec> m_frond;
    vec m_frond1(3), m_frond2(3), m_frond3(3);
    m_frond1 << 0.0 << endr << -0.8 << endr << 1.0;
    m_frond2 << 0.1 << endr << 1.0 << endr << 1.0;
    m_frond3 << 0.6 << endr << -0.8 << endr << 1.0;
    m_frond.push_back(m_frond1);
    m_frond.push_back(m_frond2);
    m_frond.push_back(m_frond3);

    diagnostics.push_back(main_frond);
    diagnostics.push_back(r_frond);
    diagnostics.push_back(l_frond);
    diagnostics.push_back(m_frond);

    atList.push_back(genArbMat(main_frond, r_frond));
    atList.push_back(genArbMat(main_frond, l_frond));
    atList.push_back(genArbMat(main_frond, m_frond));
        
    vec p1(3);
    vec p2(3);
    vec p3(3);
    vec p4(3);
    p1 << 0.0 << endr << 0.0 << endr << 1.0;
    p2 << 0.7 << endr << -0.9 << endr << 1.0;
    p3 << -0.7 << endr << -0.9 << endr << 1.0;
    p4 << 0.0 << endr << 1.0 << endr << 1.0;

    polygon.push_back(p1);
    polygon.push_back(p2);
    polygon.push_back(p3);
    polygon.push_back(p4);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    glutMainLoop();
    return 0;
}
