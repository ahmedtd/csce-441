
#include <cmath>
using std::cos;
using std::sin;

#include <armadillo>
using arma::eye;
using arma::norm;
using arma::zeros;

#include "vecops.hpp"

mat crosspmatrix(vec in)
{
    mat out(3,3) = zeros(3,3);
    
    mat(0,1) = -in(2);
    mat(0,2) = in(1);

    mat(1,0) = in(2);
    mat(1,2) = -in(0);
    
    mat(2,0) = -in(1);
    mat(2,1) = in(0);
}

mat rotmat(vec axis, double angle)
{
    vec naxis = axis / norm(axis, 2);

    mat result(3,3);
    result = (1-cos(angle))*naxis*naxis.t()
             + sin(angle)*crosspmatrix(naxis)
             + eye(cos(angle));

    return result;
}
