
#include <cmath>
using std::sin;
using std::cos;
using std::sqrt;
#include <deque>
using std::deque;
#include <map>
using std::pair;

#include <GL/glut.h>
#include "assignment3.h"
#include "init.h"

#include "mat.hpp"

// Global variables
vector<floatmat> defaultCondensation;
vector<floatmat> defaultTransforms;
int defaultDepth = 8;

Matrix translate ( Vec v )
{
    floatmat result(floatmat::eye(3));

    result.at(0,2) = v.x;
    result.at(1,2) = v.y;
    
	return result.asMatrix();
}

Matrix rotate ( Pt p, float theta )
{
	float sinv = sin(theta);
    float cosv = cos(theta);

    floatmat result = floatmat::eye(3);
    floatmat pmat(p, false);

    floatmat rotate(2,2);
    rotate.at(0,0) = cosv;
    rotate.at(0,1) = sinv;
    rotate.at(1,0) = -sinv;
    rotate.at(1,1) = cosv;

    floatmat offset = (rotate * -1.0f) * pmat;
    
    result.at(0,0) = rotate.at(0,0);
    result.at(0,1) = rotate.at(0,1);
    result.at(1,0) = rotate.at(1,0);
    result.at(1,1) = rotate.at(1,1);

    result.at(0,2) = offset.at(0,0);
    result.at(1,2) = offset.at(1,0);

	return result.asMatrix();
}

Matrix scale ( Pt p, float alpha )
{
	floatmat result = floatmat::eye(3);
    
    result.at(0,0) = alpha;
    result.at(1,1) = alpha;
    
    result.at(0,2) = (1.0-alpha) * p.x;
    result.at(1,2) = (1.0-alpha) * p.y;

    return result.asMatrix();
}

Matrix nscale ( Pt p, Vec v, float alpha )
{
	floatmat ndir(v, false);
    ndir = ndir * (1.0f / ndir.norm());

    floatmat pmat(p, false);

    floatmat multcomponent = ndir * ndir.transpose() * (alpha - 1);
    multcomponent = multcomponent + floatmat::eye(2);

    floatmat addcomponent = ndir * ndir.transpose() * pmat * (1-alpha);
    
    floatmat result = floatmat::eye(3);
    result.at(0,0) = multcomponent.at(0,0);
    result.at(0,1) = multcomponent.at(0,1);
    result.at(1,0) = multcomponent.at(1,0);
    result.at(1,1) = multcomponent.at(1,1);
    
    result.at(0,2) = addcomponent.at(0,0);
    result.at(1,2) = addcomponent.at(1,2);
    
	return result.asMatrix();
}

Matrix image ( Pt p1, Pt p2, Pt p3, Pt q1, Pt q2, Pt q3 )
{
	floatmat src(3,3);
    src.at(0,0) = p1.x;
    src.at(0,1) = p2.x;
    src.at(0,2) = p3.x;
    src.at(1,0) = p1.y;
    src.at(1,1) = p2.y;
    src.at(1,2) = p3.y;
    src.at(2,0) = 1.0f;
    src.at(2,1) = 1.0f;
    src.at(2,2) = 1.0f;

    floatmat hat(3,3);
    hat.at(0,0) = q1.x;
    hat.at(0,1) = q2.x;
    hat.at(0,2) = q3.x;
    hat.at(1,0) = q1.y;
    hat.at(1,1) = q2.y;
    hat.at(1,2) = q3.y;
    hat.at(2,0) = 1.0f;
    hat.at(2,1) = 1.0f;
    hat.at(2,2) = 1.0f;

	return (hat * src.inverse3()).asMatrix();
}

Matrix compose ( Matrix m2, Matrix m1 )
{
	floatmat mat2(m2);
    floatmat mat1(m1);

	return (mat2 * mat1).asMatrix();
}

void setCondensationSet ( vector<Pt> pts )
{
    defaultCondensation.clear();

    vector<Pt>::iterator pt_it = pts.begin();
    for(; pt_it != pts.end(); pt_it++)
    {
        floatmat ptmat(*pt_it, true);
        defaultCondensation.push_back(ptmat);
    }
}

void setIATTransformations ( vector<Matrix> transformations )
{
    defaultTransforms.clear();
    
    vector<Matrix>::iterator at_it = transformations.begin();
    for(; at_it != transformations.end(); at_it++)
    {
        floatmat transmat(*at_it);
        defaultTransforms.push_back(transmat);
    }
}

// Perform a breadth-first rendering of the given fractal
void drawfractal( const vector<floatmat> &condensation_set,
                  const vector<floatmat> &transforms,
                  int maxDepth)
{
    floatmat color_comps(3,1);
    color_comps.at(0,0) = 0.5f;
    color_comps.at(1,0) = 0.5f;
    color_comps.at(2,0) = 1.0f;

    floatmat origin(3,1);
    origin.at(0,0) = 0.0f;
    origin.at(1,0) = 0.0f;
    origin.at(2,0) = 1.0f;

    // The int is the depth of evaluation, the matrix is the transform at that
    // level.
    deque<pair<int, floatmat> > atQueue;

    // Prime the affine transform queue
    vector<floatmat>::const_iterator at_it = transforms.begin();
    // for(; at_it != transforms.end(); at_it++)
    // {
    //     pair<int, floatmat> newframe;
    //     newframe.first = 0;
    //     newframe.second = *at_it;
    //     atQueue.push_back(newframe);
    // }

    pair<int, floatmat> baseframe;
    baseframe.first = 0;
    baseframe.second = floatmat::eye(3);
    atQueue.push_back(baseframe);

    // Traverse the fractal
    while(atQueue.size() != 0)
    {
        pair<int, floatmat> frame = atQueue.front();
        atQueue.pop_front();

        // Get the color at this depth
        floatmat here_color = frame.second * color_comps;
        glColor3fv(here_color);

        // Render the condensation set
        vector<floatmat>::const_iterator cond_it = condensation_set.begin();
        if(condensation_set.size() == 1)
        {
            glBegin(GL_POINTS);
            glVertex2fv(frame.second*(*cond_it));
            glEnd();
        }
        else
        {
            glBegin(GL_LINE_STRIP);
            for(; cond_it != condensation_set.end(); cond_it++)
            {
                glVertex2fv(frame.second*(*cond_it));
            }
            glVertex2fv(frame.second*condensation_set[0]);
            glEnd();
        }
        
        // If we haven't reached the maximum depth yet
        if(frame.first < maxDepth)
        {
            at_it = transforms.begin();
            for(; at_it != transforms.end(); at_it++)
            {
                pair<int, floatmat> newframe;
                newframe.first = frame.first + 1;
                newframe.second = (*at_it) * frame.second;
                atQueue.push_back(newframe);
            }
        }
        // If we have reached the maximum depth, draw the fractal
        else
        {
            glBegin(GL_POINTS);
            floatmat newpt = frame.second * origin;
            glVertex2fv(newpt);
            glEnd();
        }
    }
}

// Draws the current IAT
void display ( void )
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawfractal(defaultCondensation, defaultTransforms, defaultDepth);

	glFlush();
}


/* do not modify the reshape function */
void reshape ( int width, int height )
{
	glViewport ( 0, 0, width, height );
	glMatrixMode ( GL_PROJECTION );
	glLoadIdentity ( );    
	gluOrtho2D (-1, 1, -1, 1);
	glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ( );
}

void keyboard( unsigned char key, int x, int y)
{
    switch(key)
    {
    case '1':
    {
        // Set up fractal hangman

        defaultDepth = 10;
        defaultTransforms.clear();
        defaultCondensation.clear();

        Pt A(0.9, 0.9);
        Pt B(0.9, -0.9);
        Pt C(-0.9, -0.9);

        Pt a1(0.0, 0.9);
        Pt b1(0.9, 0.9);
        Pt c1(0.9, 0.0);

        Pt a2(0.9, 0.0);
        Pt b2(0.9, -0.9);
        Pt c2(0.0, -0.9);

        Pt a3(0.0, 0.0);
        Pt b3(0.0, -0.9);
        Pt c3(-0.9, -0.9);

        defaultTransforms.push_back(image(A, B, C, a1, b1, c1));
        defaultTransforms.push_back(image(A, B, C, a2, b2, c2));
        defaultTransforms.push_back(image(A, B, C, a3, b3, c3));

        break;
    }
    case '2':
    {
        // Set up fractal staircase

        defaultDepth = 10;

        defaultTransforms.clear();
        defaultCondensation.clear();

        Pt a(-0.9, 0.9);
        Pt b(-0.9, 0.0);
        Pt c(-0.9, -0.9);

        Pt d(0.0, 0.0);
        Pt e(0.0, -0.9);
        Pt f(0.9, -0.9);
        
        defaultTransforms.push_back(image(a, c, f, a, b, d));
        defaultTransforms.push_back(image(a, c, f, d, e, f));

        defaultCondensation.push_back(b);
        defaultCondensation.push_back(c);
        defaultCondensation.push_back(e);
        defaultCondensation.push_back(d);
        
        break;
    }
    case '3':
    {
        // Set up fractal snowflake
        
        defaultTransforms.clear();
        defaultCondensation.clear();
        
        // Unit pentagonal coordinates from Wolfram Mathworld

        float pi  = 4*atan(1);
        float phi = 0.5 * (1.0 + sqrt(5.0));
        float c1 = 0.9*cos(2.0*pi/5.0);
        float c2 = 0.9*cos(pi / 5.0);
        float s1 = 0.9*sin(2.0*pi/5.0);
        float s2 = 0.9*sin(4.0*pi/5.0);

        Pt a(  0,  0.9);
        Pt b(-s1,  c1);
        Pt c(-s2, -c2);
        Pt d( s2, -c2);
        Pt e( s1,  c1);
        Pt f( 0.0, 0.0);

        defaultDepth = 7;

        float scaleFactor = 1.0/(1.0+phi);

        defaultTransforms.push_back(scale(a, scaleFactor));
        defaultTransforms.push_back(scale(b, scaleFactor));
        defaultTransforms.push_back(scale(c, scaleFactor));
        defaultTransforms.push_back(scale(d, scaleFactor));
        defaultTransforms.push_back(scale(e, scaleFactor));
        defaultTransforms.push_back(scale(f, -scaleFactor));
                
        break;
    }
    case '4':
    {
        // Set up hex flower
        
        float pi = 4*atan(1);
        float fc = 0.9*cos(pi/3.0);
        float fs = 0.9*sin(pi/3.0);

        Pt a(0.9, 0.0);
        Pt b(fc, fs);
        Pt c(-fc, fs);
        Pt d(-0.9, 0.0);
        Pt e(-fc, -fs);
        Pt f(fc, -fs);

        float scaleFactor = 1.0/3.0;

        defaultDepth = 7;

        defaultTransforms.clear();
        defaultCondensation.clear();

        defaultTransforms.push_back(scale(a, scaleFactor));
        defaultTransforms.push_back(scale(b, scaleFactor));
        defaultTransforms.push_back(scale(c, scaleFactor));
        defaultTransforms.push_back(scale(d, scaleFactor));
        defaultTransforms.push_back(scale(e, scaleFactor));
        defaultTransforms.push_back(scale(f, scaleFactor));
                
        break;
    }
    case '5':
    {
        Pt pta(-0.8,  0.4);
        Pt ptb(-0.8, -0.4);
        
        Pt ptc(-0.4,  0.8);
        Pt ptd(-0.4,  0.4);
        Pt pte(-0.4,  0.0);
        Pt ptf(-0.4, -0.4);
        Pt ptg(-0.4, -0.8);
        
        Pt pth(0.0,  0.4);
        Pt pti(0.0, -0.4);
        
        Pt ptj(0.4,  0.8);
        Pt ptk(0.4,  0.4);
        Pt ptl(0.4,  0.0);
        Pt ptm(0.4, -0.4);
        Pt ptn(0.4, -0.8);

        Pt pto(0.8,  0.4);
        Pt ptp(0.8, -0.4);

        defaultDepth = 5;
        
        defaultTransforms.clear();
        defaultCondensation.clear();

        defaultTransforms.push_back(scale(pta, 0.2));
        defaultTransforms.push_back(scale(ptb, 0.2));
        defaultTransforms.push_back(scale(ptc, 0.2));
        defaultTransforms.push_back(scale(ptd, 0.2));
        defaultTransforms.push_back(scale(pte, 0.2));
        defaultTransforms.push_back(scale(ptf, 0.2));
        defaultTransforms.push_back(scale(ptg, 0.2));
        defaultTransforms.push_back(scale(pth, 0.2));
        defaultTransforms.push_back(scale(pti, 0.2));
        defaultTransforms.push_back(scale(ptj, 0.2));
        defaultTransforms.push_back(scale(ptk, 0.2));
        defaultTransforms.push_back(scale(ptl, 0.2));
        defaultTransforms.push_back(scale(ptm, 0.2));
        defaultTransforms.push_back(scale(ptn, 0.2));
        defaultTransforms.push_back(scale(pto, 0.2));
        defaultTransforms.push_back(scale(ptp, 0.2));

        break;
    }
    case '6':
    {
        defaultDepth = 8;
        defaultTransforms.clear();
        defaultCondensation.clear();

        float pi  = 4*atan(1);
        Pt origin(0.0, 0.0);

        int numsectors = 5;
        
        for(int sector = 0; sector < numsectors; sector++)
        {
            floatmat rotmat = rotate(origin, sector*2.0*pi/numsectors);
            floatmat scalmat = scale(origin, 0.5);
            floatmat tranmat = translate(Vec(sector*1.75/numsectors, 0.0));

            defaultTransforms.push_back(scalmat*rotmat*tranmat);
        }

        break;
    }
    case '=':
    case '+':
        defaultDepth++;
        defaultDepth = (defaultDepth > 10) ? 10 : defaultDepth;
        break;
    case '-':
    case '_':
        defaultDepth--;
        defaultDepth = (defaultDepth < 0) ? 0 : defaultDepth;
        break;
    }

    glutPostRedisplay();
}

int main ( int argc, char** argv )
{
    glutInit ( &argc, argv );
	glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition ( 100, 100 );
	glutCreateWindow ( "Taahir Ahmed - Homework 3" );
	init ( );	
	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
    glutKeyboardFunc(keyboard);

    glClearColor(1.0, 1.0, 1.0, 1.0);

	glutMainLoop ( );
	return 0;
}
