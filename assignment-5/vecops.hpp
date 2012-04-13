
#ifndef VECOPS
#define VECOPS

#include <armadillo>
using arma::vec;
using arma::mat;

mat crosspmatrix(vec in);

mat rotmat(const vec &axis, double angle);

#endif
