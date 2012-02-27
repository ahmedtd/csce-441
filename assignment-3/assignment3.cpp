#include <GL/glut.h>
#include "assignment3.h"
#include "init.h"

Matrix translate ( Vec v )
{
	Matrix rvalue;

	return rvalue;
}

Matrix rotate ( Pt p, float theta )
{
	Matrix rvalue;

	return rvalue;
}

Matrix scale ( Pt p, float alpha )
{
	Matrix rvalue;

	return rvalue;
}

Matrix nscale ( Pt p, Vec v, float alpha )
{
	Matrix rvalue;

	return rvalue;
}

Matrix image ( Pt p1, Pt p2, Pt p3, Pt q1, Pt q2, Pt q3 )
{
	Matrix rvalue;

	return rvalue;
}

Matrix compose ( Matrix m2, Matrix m1 )
{
	Matrix rvalue;

	return rvalue;
}

void setCondensationSet ( vector<Pt> pts )
{
}

void setIATTransformations ( vector<Matrix> transformations )
{
}


// Draws the current IAT
void display ( void )
{
	glFlush ( );
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

int main ( int argc, char** argv )
{
	glutInit ( &argc, argv );
	glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize ( 500, 500 );
	glutInitWindowPosition ( 100, 100 );
	glutCreateWindow ( "<Your name> - Homework 3" );
	init ( );	
	glutDisplayFunc ( display );
	glutReshapeFunc ( reshape );
	glutMainLoop ( );
	return 0;
}
