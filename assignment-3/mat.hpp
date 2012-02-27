
#ifndef MAT
#define MAT

#include <vector>
using std::vector;

#include <GL/glut.h>

#include "assignment3.hpp"

class floatmat
{
public:
    // Construct a zero-filled matrix of dimension r*c
    floatmat(int r = 1, int c = 1);

    floatmat(const floatmat &other);

    floatmat(const Matrix &raw);

    floatmat(const Pt &raw, bool homog=true);

    floatmat(const Vec &raw, bool homog=true);
  
    // Construct an identity matrix of dimension dim*dim
    static floatmat eye(int dim);
  
    // Access the element at r*c
    float& operator()(int r, int c);

    // Constant access
    const float& operator()(int r, int c) const;

    // Matrix multiplication operator.
    floatmat operator*(const floatmat &rop) const;
    
    // Matrix adition 
    floatmat operator+(const floatmat &rop) const;
    
    // Scalar multiplication
    floatmat operator*(float rop) const;

    // Transposition operator
    floatmat transpose() const;

    // Get the norm of this matrix
    double norm() const;

    // Get number of rows
    int r() const;

    // Get number of columns
    int c() const;

    // Allows interpretation as a pointer to GLdouble array
    operator const GLdouble*() const;

    // Allows interpretation as a Matrix class
    operator const Matrix() const;
    
    // Get inverse of matrix
    floatmat inverse3() const;

private:
    vector<float> mFloats;
    int    mR;
    int    mC;
};
