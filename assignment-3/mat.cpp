
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;

#include <stdexcept>
using std::runtime_error;

#include "mat.hpp"

floatmat::floatmat(int r, int c)
    :
    mR(r),
    mC(c),
    mFloats(r*c, 0.0f)
{
    
}

floatmat::floatmat(const floatmat &other)
    :
    mR(other.mR),
    mC(other.mC),
    mFloats(other.mFloats)
{
    
}
    
floatmat::floatmat(const Matrix &raw)
    :
    mR(3),
    mC(3),
    mFloats( (float*)raw.data, ((float*)raw.data)+9)
{
}

floatmat::floatmat(const Pt &raw, bool homog, float hcoord)
    :
    mR(homog ? 3 : 2),
    mC(1),
    mFloats(homog ? 3 : 2)
{
    mFloats[0] = raw.x;
    mFloats[1] = raw.y;
    if(homog)
        mFloats[2] = hcoord;
}

floatmat floatmat::eye(int dim)
{
    floatmat eyemat(dim, dim);
    for(int count=0; count<dim; count++)
    {
        eyemat.at(count, count) = 1.0f;
    }
    
    return eyemat;
}

// float& floatmat::operator()(int r, int c)
// {
//     return mFloats[r*mC + c];
// }

// float floatmat::operator()(int r, int c) const
// {
//     return mFloats[r*mC + c];
// }

float& floatmat::at(int r, int c)
{
    return mFloats[r*mC + c];
}

float floatmat::at(int r, int c) const
{
    return mFloats[r*mC + c];
}

floatmat floatmat::operator*(const floatmat &rop) const
{
    if(this->c() != rop.r())
        throw new runtime_error("Matrix multiplication mismatch");
    
    floatmat resultmat(this->r(), rop.c());
    for(int rr=0; rr<resultmat.r(); rr++)
    {
        for(int rc=0; rc<resultmat.c(); rc++)
        {
            for(int si=0; si<this->c(); si++)
            {
                resultmat.at(rr, rc) += (*this).at(rr, si)*rop.at(si, rc);
            }
        }
    }
    
    return resultmat;
}

floatmat floatmat::operator+(const floatmat &rop) const
{
    if(this->c() != rop.c() || this->r() != rop.r())
        throw new runtime_error("Matrix addition mismatch");

    floatmat resultmat(this->r(), this->c());
    for(int r=0; r<this->r(); r++)
        for(int c=0; r<this->c(); c++)
        {
            resultmat.at(r,c) = (*this).at(r,c) + rop.at(r,c);
        }

    return resultmat;
}

floatmat floatmat::operator*(float rop) const
{
    floatmat resultmat(this->r(), this->c());
    for(int r=0; r<this->r(); r++)
        for(int c=0; c<this->c(); c++)
        {
            resultmat.at(r,c) = (*this).at(r,c) * rop;
        }

    return resultmat;
}

floatmat floatmat::transpose() const
{
    floatmat resultmat(this->c(), this->r());
    for(int r=0; r<this->r(); r++)
        for(int c=0; c<this->c(); c++)
        {
            resultmat.at(c,r) = (*this).at(r,c);
        }

    return resultmat;
}

double floatmat::norm() const
{
    return (this->transpose() * (*this)).at(0,0);
}

int floatmat::r() const
{
    return mR;
}

int floatmat::c() const
{
    return mC;
}

floatmat::operator const GLfloat*() const
{
    return (const GLfloat*) &(mFloats[0]);
}

Matrix floatmat::asMatrix() const
{
    Matrix result;
    
    for(int r=0; r < mR; r++)
    {
        for(int c=0; c<mC; c++)
        {
            result.data[r][c] = (*this).at(r,c);
        }
    }

    return result;
}

floatmat floatmat::inverse3() const
{
    floatmat resultmat(*this);

    const float &a = (*this).at(0,0);
    const float &b = (*this).at(0,1);
    const float &c = (*this).at(0,2);
    const float &d = (*this).at(1,0);
    const float &e = (*this).at(1,1);
    const float &f = (*this).at(1,2);
    const float &g = (*this).at(2,0);
    const float &h = (*this).at(2,1);
    const float &k = (*this).at(2,2);

    float det = a*(e*k - f*h) - b*(d*k - f*g) + c*(d*h - e*g);
    
    resultmat.at(0,0) = e*k - f*h; 
    resultmat.at(1,0) = f*g - d*k;
    resultmat.at(2,0) = d*h - e*g;
    resultmat.at(0,1) = c*h - b*k;
    resultmat.at(1,1) = a*k - c*g;
    resultmat.at(2,1) = b*g - a*h;
    resultmat.at(0,2) = b*f - c*e;
    resultmat.at(1,2) = c*d - a*f;
    resultmat.at(2,2) = a*e - b*d;

    return resultmat * (1.0f / det);
}

ostream& operator<<(ostream &out, const floatmat &mat)
{
    for(int r=0; r<mat.r(); r++)
    {
        for(int c=0; c<mat.c(); c++)
        {
            out << setw(4) << mat.at(r,c) << " ";
        }

        cout << endl;
    }
    
    return out;
}

ostream& operator<<(ostream &out, const Matrix &mat)
{
    for(int r=0; r<3; r++)
    {
        for(int c=0; c<3; c++)
        {
            out << setw(4) << mat.data[r][c] << " ";
        }

        cout << endl;
    }
    
    return out;
}
