
#include <cmath>
using std::sin;
using std::cos;

#include <GL/glut.h>
#include "assignment3.hpp"
#include "init.h"

#include "mat.hpp"

// Global variables
int progmode = 0;
vector<floatmat> defaultCondensation;
vector<floatmat> defaultTransforms;
int defaultDepth;

Matrix translate ( Vec v )
{
    floatmat result(floatmat::eye(3));

    result(0,2) = v.x;
    result(1,2) = v.y;
    
	return result;
}

Matrix rotate ( Pt p, float theta )
{
	float sinv = sin(theta);
    float cosv = cos(theta);

    floatmat result = floatmat::eye(3);
    floatmat pmat(p, false);

    floatmat rotate(2,2);
    rotate(0,0) = cosv;
    rotate(0,1) = sinv;
    rotate(1,0) = -sinv;
    rotate(1,1) = cosv;

    floatmat offset = (rotate * -1.0f) * pmat;
    
    result(0,0) = rotate(0,0);
    result(0,1) = rotate(0,1);
    result(1,0) = rotate(1,0);
    result(1,1) = rotate(1,1);

    result(0,2) = offset(0,0);
    result(1,2) = offset(1,0);

	return result;
}

Matrix scale ( Pt p, float alpha )
{
	floatmat result = floatmat::eye(3);
    
    floatmat(0,0) = alpha;
    floatmat(1,1) = alpha;
    
    floatmat(0,2) = -alpha * p.x;
    floatmat(0,2) = -alpha * p.y;

	return result;
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
    result(0,0) = multcomponent(0,0);
    result(0,1) = multcomponent(0,1);
    result(1,0) = multcomponent(1,0);
    result(1,1) = multcomponent(1,1);
    
    result(0,2) = addcomponent(0,0);
    result(1,2) = addcomponent(1,2);
    
	return result;
}

Matrix image ( Pt p1, Pt p2, Pt p3, Pt q1, Pt q2, Pt q3 )
{
	floatmat src(3,3);
    src(0,0) = p1.x;
    src(0,1) = p2.x;
    src(0,2) = p3.x;
    src(1,0) = p1.y;
    src(1,1) = p2.y;
    src(1,2) = p3.y;
    src(2,0) = 1.0f;
    src(2,1) = 1.0f;
    src(2,2) = 1.0f;

    floatmat hat(3,3);
    hat(0,0) = q1.x;
    hat(0,1) = q2.x;
    hat(0,2) = q3.x;
    hat(1,0) = q1.y;
    hat(1,1) = q2.y;
    hat(1,2) = q3.y;
    hat(2,0) = 1.0f;
    hat(2,1) = 1.0f;
    hat(2,2) = 1.0f;

	return hat * src.inverse3();
}

Matrix compose ( Matrix m2, Matrix m1 )
{
	floatmat mat2(m2);
    floatmat mat1(m1);

	return mat2 * mat1;
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
    color_comps(0,0) = 0.9f;
    color_comps(1,0) = 0.9f;
    color_comps(2,0) = 0.0f;

    floatmat origin(3,1);
    origin(0,0) = 0.0f;
    origin(1,0) = 0.0f;
    origin(2,0) = 1.0f;

    // The int is the depth of evaluation, the matrix is the transform at that
    // level.
    deque<pair<int, floatmat> > atQueue;

    // Prime the affine transform queue
    vector<floatmat>::const_iterator at_it = transforms.begin();
    for(; at_it != transforms.end(); at_it++)
    {
        pair<int, floatmat> newframe;
        newframe.first = 0;
        newframe.second = *at_it;
        atQueue.push_back(newframe);
    }

    // Traverse the fractal
    while(atQueue.size() != 0)
    {
        pair<int, floatmat> frame = qtQueue.front();
        atQueue.pop_front();

        // Get the color at this depth
        floatmat here_color = frame.second * color_comps;
        glColor3fv(here_color);

        // Render the condensation set
        vector<floatmat>::const_iterator cond_it = condensation_set.begin();
        if(condensation_set.length() == 1)
        {
            glBegin(GL_POINTS);
            glVertex2fv(*cond_it);
            glEnd();
        }
        else
        {
            glBegin(GL_LINE_STRIP);
            for(; cond_it != condensation_set.end(); cond_it++)
            {
                glVertex2fv(*cond_it);
            }
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
        break;
    case '2':
        break;
    case '3':
        break;
    case '4':
        break;
    case '5':
        break;
    case '6':
        break;
    case '=':
    case '+':
        break;
    case '-':
    case '_':
        break;
    }   
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
    glutKeyBoardFunc(keyboard);

    glClearColor(1.0, 1.0, 1.0, 1.0);

	glutMainLoop ( );
	return 0;
}
