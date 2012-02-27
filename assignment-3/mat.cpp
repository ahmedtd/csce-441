
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
    mFloats(raw.data, raw.data+9)
{
}

floatmat::floatmat(const Pt &raw, bool homog)
    :
    mR(homog ? 3 : 2),
    mC(1),
    mFloats(homog ? 3 : 2)
{
    mFloats[0] = raw.x;
    mFloats[1] = raw.y;
    if(homog)
        mFloats[2] = 1.0f;
}

floatmat::floatmat(const Vec &raw, bool homog)
    :
    mR(homog ? 3 : 2),
    mC(1),
    mFloats(homog ? 3: 2)
{
    mFloats[0] = raw.x;
    mFloats[1] = raw.y;
    if(homog)
        mFloats[2] = 0.0f;
}

floatmat floatmat::eye(int dim)
{
    floatmat eyemat(dim, dim);
    for(int count=0; count<dim; count++)
    {
        eyemat(dim, dim) = 1.0;
    }
    
    return eyemat;
}

float& floatmat::operator()(int r, int c)
{
    return mFloats[r*mC + c];
}

const float& floatmat::operator()(int r, int c) const
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
            for(int si=0; sc<this->c(); sc++)
            {
                resultmat(rr, rc) += (*this)(rr, si)*rop(si, rc);
            }
        }
    }
    
    return resultmat;
}

floatmat floatmat::operator+(const floatmat &rop) const
{
    if(this-c() != rop.c() || this->r() != rop.r())
        throw new runtime_error("Matrix addition mismatch");

    floatmat resultmat(this->r(), this->c());
    for(int r=0; r<this->r(); r++)
        for(int c=0; r<this->c(); c++)
        {
            resultmat(r,c) = (*this)(r,c) + rop(r,c);
        }

    return resultmat;
}

floatmat floatmat::operator*(float rop) const
{
    floatmat resultmat(this->r(), this->c());
    for(int r=0; r<this->r(); r++)
        for(int c=0; c<this->c(); c++)
        {
            resultmat(r,c) = (*this)(r,c) * rop;
        }

    return resultmat;
}

floatmat floatmat::transpose() const
{
    floatmat resultmat(this->c(), this->r());
    for(int r=0; r<this->r(); r++)
        for(int c=0; c<this->c(); c++)
        {
            resultmat(c,r) = (*this)(r,c);
        }

    return resultmat;
}

double floatmat::norm() const
{
    return this->transpose() * (*this);
}

int floatmat::r() const
{
    return mR;
}

int floatmat::c() const
{
    return mC;
}

floatmat::operator const GLdouble*() const
{
    return &(mFloats[0]);
}

floatmat::operator const Matrix() const
{
    Matrix result;
    
    for(int count=0; count < mR * mC; count++)
    {
        result.data[count] = mFloats[count];
    }

    return result;
}

floatmat floatmat::inverse3() const
{
    floatmat resultmat(*this);

    float &a = (*this)(0,0);
    float &b = (*this)(0,1);
    float &c = (*this)(0,2);
    float &d = (*this)(1,0);
    float &e = (*this)(1,1);
    float &f = (*this)(1,2);
    float &g = (*this)(3,0);
    float &h = (*this)(3,1);
    float &k = (*this)(3,2);

    float det = a*(e*k - f*h) - b*(d*k - f*g) + c*(d*h - e*g);
    
    resultmat(0,0) = e*k - f*h; 
    resultmat(1,0) = f*g - d*k;
    resultmat(2,0) = d*h - e*g;
    resultmat(0,1) = c*h - b*k;
    resultmat(1,1) = a*k - c*g;
    resultmat(2,1) = b*g - a*h;
    resultmat(0,2) = b*f - c*e;
    resultmat(1,2) = c*d - a*f;
    resultmat(2,2) = a*e - b*d;

    return resultmat;
}

    
