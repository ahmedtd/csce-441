#ifndef ASSIGNMENT_3_H
#define ASSIGNMENT_3_H

#include <vector>

using namespace std;

/**
 * You should not change any of the definitions in this file.
 * We will be providing new init.cpp files to test your program
 * on new factals.  These functions define the interface between
 * the two files!
 */



class Matrix
{
public:
	float data [ 3 ] [ 3 ];

	Matrix ( void )
	{
		int i, j;

		for ( i = 0; i < 3; i++ )
		{
			for ( j = 0; j < 3; j++ )
			{
				data [ i ] [ j ] = 0;
			}
		}
	}
};

class Pt
{
public:
	float x, y;

	Pt ( float newX, float newY ) 
	{ 
		x = newX;
		y = newY;
	}

	Pt ( void ) 
	{ 
		x = y = 0;
	}
};

typedef Pt Vec;

/**
 * Constructs a matrix to translate by the vector "v"
 */
Matrix translate ( Vec v );

/**
 * Constructs a matrix to rotate about the point "p" by the angle "theta"
 */
Matrix rotate ( Pt p, float theta );

/**
 * Constructs a matrix to uniformly scale about 
 * the point "p" by the scalar "alpha"
 */
Matrix scale ( Pt p, float alpha );

/**
 * Constructs a matrix to non-uniformly scale about 
 * the point "p" in the direction "v" by the scalar "alpha"
 */
Matrix nscale ( Pt p, Vec v, float alpha );

/**
 * Constructs a matrix representing the affine transformation that takes the
 * points "p1", "p2", "p3" to the deformed points "q1", "q2", "q3"
 */
Matrix image ( Pt p1, Pt p2, Pt p3, Pt q1, Pt q2, Pt q3 );

/**
 * Multiplies the matrices "m2", "m1" to produce 
 * the product m2 * m1 (in that order).
 */
Matrix compose ( Matrix m2, Matrix m1 );

/**
 * Sets the condensation set for this IAT.
 * If pts.size ( ) == 0, then there is no condensation set for this fractal
 * If pts.size ( ) == 1, then the set is a point
 * If pts.size ( ) == 2, then the set is a line
 * If pts.size ( ) > 2, then the set should be a 
 *    poly-line (line with first and last points connected)
 */
void setCondensationSet ( vector<Pt> pts );

/**
 * Sets the transformations for the current IAT.
 */
void setIATTransformations ( vector<Matrix> transformations );

#endif
